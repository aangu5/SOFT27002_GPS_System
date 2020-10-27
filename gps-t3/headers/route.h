#ifndef ROUTE_H_211217
#define ROUTE_H_211217

#include <string>
#include <vector>

#include "types.h"
#include "position.h"
#include "xml/parser.h"

namespace GPS
{
  class Route
  {
    public:
      /*  Routes are constructed from GPX data.  The data can be provided as a string, or from a file.
       *  Whitespace is trimmed from the start and end of route names and route-point names.
       *  Any route points closer to their predecessor than the 'granularity' parameter are discarded.
       */
      Route(std::string source,
            bool isFileName, // Is the first parameter a file name or a string containing GPX data?
            metres granularity = 20); // The minimum distance between successive route points.

      /* Update the granularity of the stored Route.  Any position in the Route that differs in distance
       * from its predecessor by less than the updated granularity is discarded.
       */
      virtual void setGranularity(metres);

      // Returns the name of the Route, or "Unnamed Route" if the route is nameless.
      std::string name() const;

      // Returns the number of stored route points.
      unsigned int numPositions() const;

      // The total length of the Route; this is the sum of the distances between successive route points.
      metres totalLength() const;

      // The distance between the first and last points on the Route.
      metres netLength() const;

      /* The sum of all the positive (>0) height differences between successive route points.
       * That is, downhill changes are ignored.
       */
      metres totalHeightGain() const;

      /* The increase in height from the start point to the finishing point.
       * Returns zero if the height difference is negative.
       */
      metres netHeightGain() const;

      /* The steepest uphill gradient (in degrees) between successive points on the route.
       * If the entire route is downhill, then this is the least steep downhill gradient (i.e. negative).
       * Throws a std::domain_error if the route only contains a single Position.
       */
      degrees maxGradient() const;

      /* The steepest downhill gradient (in degrees) between successive points on the Route.
       * If the entire route is uphill, then this is the least steep uphill gradient (i.e. positive).
       * Throws a std::domain_error if the route only contains a single Position.
       */
      degrees minGradient() const;

      /* The steepest gradient between successive points on the Route, whether uphill or downhill.
       * Throws a std::domain_error if the route only contains a single Position.
       */
      degrees steepestGradient() const;

      // The latitude of the most Southerly point on the Route.
      degrees minLatitude() const;

      // The latitude of the most Northerly point on the Route.
      degrees maxLatitude() const;

      // The longitude of the most Westerly point on the Route.
      degrees minLongitude() const;

      // The longitude of the most Easterly point on the Route.
      degrees maxLongitude() const;

      // The elevation of the lowest point on the Route.
      metres minElevation() const;

      // The elevation of the highest point on the Route.
      metres maxElevation() const;

      /* Return the route point at the specified index.
       * Throws a std::out_of_range exception if the index is out-of-range.
       */
      Position operator[](unsigned int) const;

      /* Find the route point bearing the specified name.
       * Throws a std::invalid_argument exception if the argument is an empty string.
       * Throws a std::domain_error exception if the name is not found.
       */
      Position findPosition(std::string soughtName) const;

      /* Returns the name of the Position, or "Unnamed Position" if the point is nameless.
       * Throws a std::domain_error exception if that Position is not within "granularity"
       * of any stored Positions.
       */
      std::string findNameOf(Position) const;

      /* Compute how many times the named route point is visited during the route.
       * Throws a std::invalid_argument exception if the argument is an empty string.
       */
      unsigned int timesVisited(std::string soughtName) const;

      // Compute how many times the specified Position is visited during the route.
      unsigned int timesVisited(Position) const;

      // Determine if any Position is visited more than once during the route.
      bool containsCycles() const;

    protected:
      Route() {} // Only called by Track constructor.

      std::vector<Position> positions;
      std::vector<std::string> positionNames;

      std::string routeName;
      metres routeLength;

      metres granularity;

      /* Two Positions are considered to be the same location if they are less than
       * "granularity" metres apart (horizontally).
       */
      bool areSameLocation(Position, Position) const;
  };
}

#endif
