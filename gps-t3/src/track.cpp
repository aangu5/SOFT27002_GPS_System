#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "geometry.h"
#include "xml/element.h"
#include "xml/parser.h"
#include "track.h"

using namespace GPS;

// Note: The implementation should exploit the relationship:
//   totalTime() == restingTime() + travellingTime()

seconds Track::totalTime() const
{
    // Stub definition, TODO
    return 0;
}

seconds Track::restingTime() const
{
    // Stub definition, TODO
    return 0;
}

seconds Track::travellingTime() const
{
    // Stub definition, TODO
    return 0;
}

seconds Track::longestRest() const
{
    // Stub definition, TODO
    return 0;
}

speed Track::maxSpeed() const
{
    // Stub definition, TODO
    return 0;
}

speed Track::averageSpeed(bool includeRests) const
{
    // Stub definition, TODO
    return 0;
}

speed Track::maxRateOfAscent() const
{
    // Stub definition, TODO
    return 0;
}

speed Track::maxRateOfDescent() const
{
    // Stub definition, TODO
    return 0;
}

Track::Track(std::string source, bool isFileName, metres granularity)
{
    using namespace std;
    using namespace XML;
    ostringstream oss,oss2;
    Element ele = SelfClosingElement("",{}), temp = ele, temp2 = ele, ele2 = ele; // Work-around because there's no public constructor in Element.
    string lat,lon,el,name,time;
    metres deltaH,deltaV;
    seconds startTime,currentTime,timeElapsed;
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
    if (! ele.containsSubElement("trk")) throw domain_error("Missing 'trk' element.");
    ele = ele.getSubElement("trk");
    if (ele.containsSubElement("name")) {
        temp = ele.getSubElement("name");
        name = temp.getLeafContent();
        i = name.find_first_not_of(' ');
        j = name.find_last_not_of(' ');
        routeName = (i == -1) ? "" : name.substr(i,j-i+1);
        oss << "Track name is: " << routeName << endl;
    }
    num = 0;
    if (! ele.containsSubElement("trkseg")) {
        if (! ele.containsSubElement("trkpt")) throw domain_error("Missing 'trkpt' element.");
        total = ele.countSubElements("trkpt");
        temp = ele.getSubElement("trkpt");
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
        arrived.push_back(0);
        departed.push_back(0);
        if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
        temp2 = temp.getSubElement("time");
        time = temp2.getLeafContent();
        startTime = currentTime = stoull(time);
        Position prevPos = positions.back(), nextPos = positions.back();
        skipped = 0;
        while (num+skipped < total) {
            temp = ele.getSubElement("trkpt",num+skipped);
            if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
            if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
            lat = temp.getAttribute("lat");
            lon = temp.getAttribute("lon");
            if (temp.containsSubElement("ele")) {
                temp2 = temp.getSubElement("ele");
                el = temp2.getLeafContent();
                nextPos = Position(lat,lon,el);
            } else nextPos = Position(lat,lon);
            if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
            temp2 = temp.getSubElement("time");
            time = temp2.getLeafContent();
            currentTime = stoull(time);
            if (areSameLocation(nextPos, prevPos)) {
                // If we're still at the same location, then we haven't departed yet.
                departed.back() = currentTime - startTime;
                oss << "Position ignored: " << nextPos.toString() << endl;
                ++skipped;
            } else {
                if (temp.containsSubElement("name")) {
                    temp2 = temp.getSubElement("name");
                    name = temp2.getLeafContent();
                    i = name.find_first_not_of(' ');
                    j = name.find_last_not_of(' ');
                    name = (i == -1) ? "" : name.substr(i,j-i+1);
                } else name = ""; // Fixed bug by adding this.
                positions.push_back(nextPos);
                positionNames.push_back(name);
                timeElapsed = currentTime - startTime;
                arrived.push_back(timeElapsed);
                departed.push_back(timeElapsed);
                oss << "Position added: " << nextPos.toString() << endl;
                oss << " at time: " << to_string(timeElapsed) << endl;
                ++num;
                prevPos = nextPos;
            }
        }
    }
    else
    {
        for (unsigned int segNum = 0; segNum < ele.countSubElements("trkseg"); ++segNum) {
            ele2 = ele.getSubElement("trkseg",segNum);
            if (! ele2.containsSubElement("trkpt")) throw domain_error("Missing 'trkpt' element.");
            total = ele2.countSubElements("trkpt");
            skipped = -num; // Setting skipped to start at -num (rather than 0) cancels any points accumulated from previous segments
                            // We have to set it here, rather than just before the loop, because num may increment in the next if-statement
            if (segNum == 0) {
                temp = ele2.getSubElement("trkpt");
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
                }
                else {
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
                arrived.push_back(0);
                departed.push_back(0);
                if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
                temp2 = temp.getSubElement("time");
                time = temp2.getLeafContent();
                startTime = currentTime = stoull(time);
           }
           Position prevPos = positions.back(), nextPos = positions.back();
           while (num+skipped < total) {
               temp = ele2.getSubElement("trkpt",num+skipped);
               if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
               if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
               lat = temp.getAttribute("lat");
               lon = temp.getAttribute("lon");
               if (temp.containsSubElement("ele")) {
                   temp2 = temp.getSubElement("ele");
                   el = temp2.getLeafContent();
                   nextPos = Position(lat,lon,el);
               } else nextPos = Position(lat,lon);
               if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
               temp2 = temp.getSubElement("time");
               time = temp2.getLeafContent();
               currentTime = stoull(time);
               if (areSameLocation(nextPos, prevPos)) {
                   // If we're still at the same location, then we haven't departed yet.
                   departed.back() = currentTime - startTime;
                   oss << "Position ignored: " << nextPos.toString() << endl;
                   ++skipped;
               } else {
                   if (temp.containsSubElement("name")) {
                       temp2 = temp.getSubElement("name");
                       name = temp2.getLeafContent();
                       i = name.find_first_not_of(' ');
                       j = name.find_last_not_of(' ');
                       name = (i == -1) ? "" : name.substr(i,j-i+1);
                   } else name = ""; // Fixed bug by adding this.
                   positions.push_back(nextPos);
                   positionNames.push_back(name);
                   timeElapsed = currentTime - startTime;
                   arrived.push_back(timeElapsed);
                   departed.push_back(timeElapsed);
                   oss << "Position added: " << nextPos.toString() << endl;
                   oss << " at time: " << to_string(timeElapsed) << endl;
                   ++num;
                   prevPos = nextPos;
               }
           }
        }
    }
    oss << num << " positions added." << endl;
    routeLength = 0;
    for (int i = 1; i < num; ++i) {
        deltaH = Position::distanceBetween(positions[i-1], positions[i]);
        deltaV = positions[i-1].elevation() - positions[i].elevation();
        routeLength += sqrt(pow(deltaH,2) + pow(deltaV,2));
    }
    // buildReport = oss.str();
}

void Track::setGranularity(metres)
{
    // TODO
}
