#include <iostream>

#include "logs.h"
#include "route.h"
#include "track.h"

using std::cout;
using std::endl;

void testRoute(std::string fileName)
{
    const std::string filePath = GPS::LogFiles::GPXRoutesDir + fileName + ".gpx";

    const bool isFileName = true;

    GPS::Route route = GPS::Route(filePath,isFileName);

    cout << "Route name: " << route.name() << endl;
    // cout << "Number of positions in route: " << route.numPositions() << endl;
    cout << "Total route length: " << route.totalLength() << endl;
    // cout << "Net route length: " << route.netLength() << endl;
    cout << endl;
}

void testTrack(std::string fileName)
{
    const std::string filePath = GPS::LogFiles::GPXTracksDir + fileName + ".gpx";

    const bool isFileName = true;

    GPS::Track track = GPS::Track(filePath,isFileName);

    cout << "Track name: " << track.name() << endl;
    // cout << "Number of positions in track: " << track.numPositions() << endl;
    cout << "Total track length: " << track.totalLength() << endl;
    // cout << "Net track length: " << track.netLength() << endl;
    // cout << "Travelling time: " << track.travellingTime() << endl;
    cout << endl;
}

int main()
{
    testRoute("NorthYorkMoors");
    testRoute("ABCD");
    testTrack("A1B3C");
}
