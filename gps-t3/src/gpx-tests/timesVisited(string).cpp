//
// Created by Angus on 23/10/2020.
//
#include <boost/test/unit_test.hpp>

#include "logs.h"
#include "route.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE( Route_timesVisitedString )

const bool isFileName = false; // all data for this test suite is passed in as strings, not files.

    // A simple route with one point and one name to check.
    BOOST_AUTO_TEST_CASE( singleton_route )
    {
        const std::string gpxData =
                R"(<gpx><rte><name>MyRoute</name><rtept lat="0" lon="0"><name>MyPosition</name></rtept></rte></gpx>)";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("MyPosition"), 1 );
    }

    // A simple route with one point and one name to check, with leading and trailing spaces to check the method matches the constructor.
    BOOST_AUTO_TEST_CASE( singleton_route_with_spaces )
    {
        const std::string gpxData =
                R"(<gpx><rte><name>MyRoute</name><rtept lat="0" lon="0"><name>    My Position    </name></rtept></rte></gpx>)";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("My Position"), 1 );
    }

    // A simple route with one point and checking the number of times for a position that wasn't visited.
    BOOST_AUTO_TEST_CASE( position_not_visited )
    {
        const std::string gpxData =
                R"(<gpx><rte><name>MyRoute</name><rtept lat="0" lon="0"></rtept></rte></gpx>)";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("Your Position"), 0 );
    }

    // A simple route with one point and checking invalid_argument is thrown when a blank string is passed in.
    BOOST_AUTO_TEST_CASE( bad_input_string )
    {
        const std::string gpxData =
                R"(<gpx><rte><name>MyRoute</name><rtept lat="0" lon="0"><name>MyPosition</name></rtept></rte></gpx>)";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_THROW( route.timesVisited(""), std::invalid_argument );
    }

    // A complex route where many positions share the same name.
    BOOST_AUTO_TEST_CASE( one_name_multiple_positions )
    {
        const std::string gpxData =
                "<gpx><rte><name>MyRoute</name>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"1\" lon=\"2\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"90\" lon=\"40\"><name>NotMyPosition</name></rtept>"
                "   <rtept lat=\"-11\" lon=\"150\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"25\" lon=\"120\"><name>MyPosition</name></rtept>"
                "</rte></gpx>";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("MyPosition"), 4 );
    }

    // A complex route where one position is visited many times.
    BOOST_AUTO_TEST_CASE( one_position_many_visits )
    {
        const std::string gpxData =
                "<gpx><rte><name>MyRoute</name>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"90\" lon=\"40\"><name>NotMyPosition</name></rtept>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"90\" lon=\"40\"><name>NotMyPosition</name></rtept>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "</rte></gpx>";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("MyPosition"), 3 );
    }

    // A complex route where there are many positions with the same name, visited multiple times.
    BOOST_AUTO_TEST_CASE( one_name_many_positions_and_many_visits )
    {
        const std::string gpxData =
                "<gpx><rte><name>MyRoute</name>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"90\" lon=\"40\"><name>NotMyPosition</name></rtept>"
                "   <rtept lat=\"-11\" lon=\"150\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"-11\" lon=\"150\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"90\" lon=\"40\"><name>NotMyPosition</name></rtept>"
                "   <rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept>"
                "   <rtept lat=\"-11\" lon=\"150\"><name>MyPosition</name></rtept>"
                "</rte></gpx>";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("MyPosition"), 6 );
    }

BOOST_AUTO_TEST_SUITE_END()
