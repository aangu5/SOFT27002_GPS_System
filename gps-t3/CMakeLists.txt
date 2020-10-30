cmake_minimum_required(VERSION 3.16)
project(deliverable3)

set(CMAKE_CXX_STANDARD 14)

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})
include_directories(./headers/)
include_directories(./headers/gridworld)
include_directories(./headers/xml)

add_executable (Boost_Tests_run ./src/gpx-tests.cpp ./src/gpx-tests/timesVisited\(string\).cpp ./src/route.cpp ./src/geometry.cpp ./src/xml/element.cpp ./src/xml/parser.cpp ./src/position.cpp ./src/earth.cpp ./src/logs.cpp)
target_link_libraries (Boost_Tests_run ${Boost_LIBRARIES})
