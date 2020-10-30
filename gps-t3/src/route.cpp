#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "geometry.h"
#include "xml/element.h"
#include "xml/parser.h"
#include "route.h"

using namespace GPS;

std::string Route::name() const
{
    return routeName.empty() ? "Unnamed Route" : routeName;
}

unsigned int Route::numPositions() const
{
    return positions.size();
}

metres Route::totalLength() const
{
    return routeLength;
}

metres Route::netLength() const
{
    assert(! positions.empty());

    const Position & start  = positions.front();
    const Position & finish = positions.back();

    metres deltaH = Position::distanceBetween(start,finish);
    metres deltaV = start.elevation() - finish.elevation();
    return std::sqrt(deltaH*deltaH + deltaV*deltaV);
}

metres Route::totalHeightGain() const
{
    assert(! positions.empty());

    metres total = 0.0;
    for (unsigned int i = 1; i < numPositions(); ++i)
    {
        metres deltaV = positions[i].elevation() - positions[i-1].elevation();
        if (deltaV > 0.0) total += deltaV; // ignore negative height differences
    }
    return total;
}

metres Route::netHeightGain() const
{
    assert(! positions.empty());

    metres deltaV = positions.back().elevation() - positions.front().elevation();
    return std::max(deltaV,0.0); // ignore negative height differences
}

degrees Route::minLatitude() const
{
    assert(! positions.empty());

    degrees minLat = positions.front().latitude();
    for (const Position& pos : positions)
    {
        minLat = std::min(minLat,pos.latitude());
    }
    return minLat;
}

degrees Route::maxLatitude() const
{
    assert(! positions.empty());

    degrees maxLat = positions.front().latitude();
    for (const Position& pos : positions)
    {
        maxLat = std::max(maxLat,pos.latitude());
    }
    return maxLat;
}

degrees Route::minLongitude() const
{
    assert(! positions.empty());

    degrees minLon = positions.front().longitude();
    for (const Position& pos : positions)
    {
        minLon = std::min(minLon,pos.longitude());
    }
    return minLon;
}

degrees Route::maxLongitude() const
{
    assert(! positions.empty());

    degrees maxLon = positions.front().longitude();
    for (const Position& pos : positions)
    {
        maxLon = std::max(maxLon,pos.longitude());
    }
    return maxLon;
}

metres Route::minElevation() const
{
    assert(! positions.empty());

    degrees minEle = positions.front().elevation();
    for (const Position& pos : positions)
    {
        minEle = std::min(minEle,pos.elevation());
    }
    return minEle;
}

metres Route::maxElevation() const
{
    assert(! positions.empty());

    degrees maxEle = positions.front().elevation();
    for (const Position& pos : positions)
    {
        maxEle = std::max(maxEle,pos.elevation());
    }
    return maxEle;
}

degrees Route::maxGradient() const
{
    assert(! positions.empty());

    if (positions.size() == 1) throw std::domain_error("Cannot compute gradients on a single-point route.");

    degrees maxGrad = -halfRotation/2; // minimum possible value
    for (unsigned int i = 1; i < positions.size(); ++i)
    {
        metres deltaH = Position::distanceBetween(positions[i],positions[i-1]);
        metres deltaV = positions[i].elevation() - positions[i-1].elevation();
        degrees grad = radToDeg(std::atan(deltaV/deltaH));
        maxGrad = std::max(maxGrad,grad);
    }
    return maxGrad;
}

degrees Route::minGradient() const
{
    assert(! positions.empty());

    if (positions.size() == 1) throw std::domain_error("Cannot compute gradients on a single-point route.");

    degrees minGrad = halfRotation/2; // maximum possible value
    for (unsigned int i = 1; i < positions.size(); ++i)
    {
        metres deltaH = Position::distanceBetween(positions[i],positions[i-1]);
        metres deltaV = positions[i].elevation() - positions[i-1].elevation();
        degrees grad = radToDeg(std::atan(deltaV/deltaH));
        minGrad = std::min(minGrad,grad);
    }
    return minGrad;
}

degrees Route::steepestGradient() const
{
    assert(! positions.empty());

    if (positions.size() == 1) throw std::domain_error("Cannot compute gradients on a single-point route.");

    degrees steepestGrad = 0; // minimum possible value
    for (unsigned int i = 1; i < positions.size(); ++i)
    {
        metres deltaH = Position::distanceBetween(positions[i],positions[i-1]);
        metres deltaV = positions[i].elevation() - positions[i-1].elevation();
        degrees grad = radToDeg(std::atan(deltaV/deltaH));
        if (std::abs(grad) > std::abs(steepestGrad)) steepestGrad = grad;
    }
    return steepestGrad;
}

Position Route::operator[](unsigned int idx) const
{
    if (idx >= positions.size())
    {
        throw std::out_of_range("Position index out-of-range.");
    }
    return positions[idx];
}

Position Route::findPosition(std::string soughtName) const
{
    if (soughtName.empty()) throw std::invalid_argument("Cannot find the position of an empty name.");

    auto nameIt = std::find(positionNames.begin(), positionNames.end(), soughtName);

    if (nameIt == positionNames.end())
    {
        throw std::domain_error("No position with that name found in the route.");
    }
    else
    {
        return positions[std::distance(positionNames.begin(),nameIt)];
    }
}

std::string Route::findNameOf(Position soughtPos) const
{
    auto posIt = std::find_if(positions.begin(), positions.end(),
                              [&] (const Position& pos) {return areSameLocation(pos,soughtPos);});

    if (posIt == positions.end())
    {
        throw std::domain_error("Position not found in route.");
    }
    else
    {
        std::string name = positionNames[std::distance(positions.begin(),posIt)];
        return name.empty() ? "Unnamed Position" : name;
    }
}

unsigned int Route::timesVisited(std::string soughtName) const
{
    if (soughtName.empty()) throw std::invalid_argument("Cannot find the position of an empty name.");

    return std::count_if(positionNames.begin(),positionNames.end(),
                         [&] (std::string name) {return name == soughtName;});
}

unsigned int Route::timesVisited(Position soughtPos) const
{
    return std::count_if(positions.begin(),positions.end(),
                         [&] (const Position& pos) {return areSameLocation(pos,soughtPos);});
}

bool Route::containsCycles() const
{
    for (auto currentPos = positions.begin(); currentPos != positions.end(); ++currentPos)
    {
        for (auto possibleMatchPos = std::next(currentPos); possibleMatchPos != positions.end(); ++possibleMatchPos)
        {
            if (areSameLocation(*currentPos,*possibleMatchPos)) return true;
        }
    }
    return false;
}


Route::Route(std::string source, bool isFileName, metres granularity)
{
    using namespace std;
    using namespace XML;
    ostringstream oss,oss2;
    Element ele = SelfClosingElement("",{}), temp = ele, temp2 = ele; // Work-around because there's no public constructor in Element.
    string lat,lon,el,name;
    metres deltaH,deltaV;
    int num,i,j,total,skipped;
    this->granularity = granularity;
    if (isFileName) {
        ifstream fs(source);
        if (! fs.good()) throw invalid_argument("Error opening source file '" + source + "'.");
        oss << "Source file '" << source << "' opened okay." << endl;
        while (fs.good()) {
            getline(fs, name); // Using name as temporary variable as we don't need it until later
            oss2 << name << endl;
        }
        source = oss2.str();
    }
    ele = Parser(source).parseRootElement();
    if (ele.getName() != "gpx") throw domain_error("Missing 'gpx' element.");
    if (! ele.containsSubElement("rte")) throw domain_error("Missing 'rte' element.");
    ele = ele.getSubElement("rte");
    if (ele.containsSubElement("name")) {
        temp = ele.getSubElement("name");
        name = temp.getLeafContent();
        i = name.find_first_not_of(' ');
        j = name.find_last_not_of(' ');
        // if (i == string::npos)
        // {
        //    name = "";
        // }
        // else
        // {
        //   name.erase(0,i);
        //   j = name.find_last_not_of(' ');
        //   name.erase(j+1);
        // }
        // routeName = name;
        routeName = (i == -1) ? "" : name.substr(i,j-i+1); // So much shorter than Ken's version :)
        oss << "Route name is: " << routeName << endl;
    }
    num = 0;
    if (! ele.containsSubElement("rtept")) throw domain_error("Missing 'rtept' element.");
    total = ele.countSubElements("rtept");
    temp = ele.getSubElement("rtept");
    if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
    if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
    lat = temp.getAttribute("lat");
    lon = temp.getAttribute("lon");
    if (temp.containsSubElement("ele")) {
        temp2 = temp.getSubElement("ele");
        el = temp2.getLeafContent();
        Position startPos = Position(lat,lon,el);
        positions.push_back(startPos);
        oss << "Position added: " << startPos.toString() << endl;
        ++num;
    } else {
        Position startPos = Position(lat,lon);
        positions.push_back(startPos);
        oss << "Position added: " << startPos.toString() << endl;
        ++num;
    }
    if (temp.containsSubElement("name")) {
        temp2 = temp.getSubElement("name");
        name = temp2.getLeafContent();
        i = name.find_first_not_of(' ');
        j = name.find_last_not_of(' ');
        name = (i == -1) ? "" : name.substr(i,j-i+1);
    } else name = ""; // Fixed bug by adding this.
    positionNames.push_back(name);
    Position prevPos = positions.back(), nextPos = positions.back();
    skipped = 0;
    while (num+skipped < total) {
        temp = ele.getSubElement("rtept",num+skipped);
        if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
        if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
        lat = temp.getAttribute("lat");
        lon = temp.getAttribute("lon");
        if (temp.containsSubElement("ele")) {
            temp2 = temp.getSubElement("ele");
            el = temp2.getLeafContent();
            nextPos = Position(lat,lon,el);
        } else nextPos = Position(lat,lon);
        if (areSameLocation(nextPos, prevPos))
        {
            oss << "Position ignored: " << nextPos.toString() << endl;
            ++skipped;
        }
        else {
            if (temp.containsSubElement("name")) {
                temp2 = temp.getSubElement("name");
                name = temp2.getLeafContent();
                i = name.find_first_not_of(' ');
                j = name.find_last_not_of(' ');
                name = (i == -1) ? "" : name.substr(i,j-i+1);
            } else name = ""; // Fixed bug by adding this.
            positions.push_back(nextPos);
            positionNames.push_back(name);
            oss << "Position added: " << nextPos.toString() << endl;
            ++num;
            prevPos = nextPos;
        }
    }
    oss << num << " positions added." << endl;
    routeLength = 0;
    for (int i = 1; i < num; ++i ) {
        deltaH = Position::distanceBetween(positions[i-1], positions[i]);
        deltaV = positions[i-1].elevation() - positions[i].elevation();
        routeLength += sqrt(pow(deltaH,2) + pow(deltaV,2));
    }
    // buildReport = oss.str();
}

bool Route::areSameLocation(Position p1, Position p2) const
{
    return (Position::distanceBetween(p1,p2) < granularity);
}

void Route::setGranularity(metres)
{
    // TODO: Unimplemented
}

