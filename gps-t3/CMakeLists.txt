cmake_minimum_required(VERSION 3.16)
project(deliverable3)

set(CMAKE_CXX_STANDARD 14)

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})
include_directories(./headers/)
include_directories(../gps-t2/headers/)
include_directories(../gps-analysing_routes/headers/)
include_directories(../gps-analysing_routes/headers/xml/)

add_executable (Boost_Tests_run ./src/gpx-tests.cpp ./src/gpx-tests/timesVisited\(string\).cpp ../gps-analysing_routes/src/route.cpp ../gps-t2/src/geometry.cpp ../gps-analysing_routes/src/xml/element.cpp ../gps-analysing_routes/src/xml/parser.cpp ../gps-t2/src/position.cpp ../gps-t2/src/earth.cpp ../gps-t2/src/logs.cpp)
target_link_libraries (Boost_Tests_run ${Boost_LIBRARIES})
