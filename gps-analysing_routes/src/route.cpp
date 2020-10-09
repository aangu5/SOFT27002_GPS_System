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
    // Stub definition, TODO
    return 0;
}

metres Route::totalLength() const
{
    return routeLength;
}

metres Route::netLength() const
{
    // Stub definition, TODO
    return 0;
}

metres Route::totalHeightGain() const
{
    // Stub definition, TODO
    return 0;
}

metres Route::netHeightGain() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::minLatitude() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::maxLatitude() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::minLongitude() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::maxLongitude() const
{
    // Stub definition, TODO
    return 0;
}

metres Route::minElevation() const
{
    // Stub definition, TODO
    return 0;
}

metres Route::maxElevation() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::maxGradient() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::minGradient() const
{
    // Stub definition, TODO
    return 0;
}

degrees Route::steepestGradient() const
{
    // Stub definition, TODO
    return 0;
}

Position Route::operator[](unsigned int idx) const
{
    // Stub definition, TODO
    return Position(0,0);
}

Position Route::findPosition(std::string soughtName) const
{
    // Stub definition, TODO
    return Position(0,0);
}

std::string Route::findNameOf(Position soughtPos) const
{
    // Stub definition, TODO
    return "";
}

unsigned int Route::timesVisited(std::string soughtName) const
{
    // Stub definition, TODO
    return 0;
}

unsigned int Route::timesVisited(Position soughtPos) const
{
    // Stub definition, TODO
    return 0;
}

bool Route::containsCycles() const
{
    // Stub definition, TODO
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
