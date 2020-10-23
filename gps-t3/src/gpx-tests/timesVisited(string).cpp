//
// Created by Angus on 23/10/2020.
//
#include <boost/test/unit_test.hpp>

#include "logs.h"
#include "types.h"
#include "route.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE( Route_timesVisitedString )

const bool isFileName = false;

    // A simple route with one point and one name to check.
    BOOST_AUTO_TEST_CASE( singleton_route )
    {
        const std::string gpxData =
                "<gpx><rte><name>MyRoute</name><rtept lat=\"0\" lon=\"0\"><name>MyPosition</name></rtept></rte></gpx>";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("MyPosition"), 1 );
    }

    // A simple route with one point and one name to check, with leading and trailing spaces to check the method matches the constructor.
    BOOST_AUTO_TEST_CASE( singleton_route_with_spaces )
    {
        const std::string gpxData =
                "<gpx><rte><name>MyRoute</name><rtept lat=\"0\" lon=\"0\"><name>    My Position    </name></rtept></rte></gpx>";
        Route route = Route(gpxData, isFileName);
        BOOST_CHECK_EQUAL( route.timesVisited("My Position"), 1 );
    }

BOOST_AUTO_TEST_SUITE_END()
