#include <iostream>
#include <fstream>
#include <cassert>

#include "gridworld_model.h"
#include "gridworld_route.h"
#include "gridworld_track.h"

using namespace GPS;

int main()
{
    std::string exampleRouteString = "ABC";

    assert( GridWorldRoute::isValidRouteString(exampleRouteString) );

    GridWorldRoute exampleGWroute(exampleRouteString);

    std::cout << exampleGWroute.toGPX() << std::endl;

    return 0;
}
