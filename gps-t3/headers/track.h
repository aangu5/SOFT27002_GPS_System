#ifndef TRACK_H_211217
#define TRACK_H_211217

#include <string>
#include <vector>

#include "types.h"
#include "position.h"
#include "route.h"
#include "xml/parser.h"

namespace GPS
{
  class Track : public Route
  {
    public:
      /*  Tracks are constructed from GPX data.  The data can be provided as a string, or from a file.
       *  Whitespace is trimmed from the start and end of track names and track-point names.
       *  Any track points closer to their predecessor than the 'granularity' parameter are discarded.
       */
      Track(std::string source,
            bool isFileName, // Is the first parameter a file name or a string containing GPX data?
            metres granularity = 10); // The minimum distance between successive track points.

      /* Update the granularity of the stored track.  Any position in the track that differs in distance
       * from its predecessor by less than the updated granularity is discarded.
       */
      void setGranularity(metres) override;

      // Total elapsed time between start and finish of track.
      seconds totalTime() const;

      // Total elapsed time between start and finish of the track that is spent moving.
      seconds travellingTime() const;

      // Total elapsed time between start and finish of the track that is spent not moving.
      seconds restingTime() const;

      // The duration of the longest period spent not moving during the track.
      seconds longestRest() const;

      // The fastest speed between successive track points.
      // Throws a std::domain_error if the travelling time of the track is zero.
      speed maxSpeed() const;

      // The average speed between successive track points over the course of the whole track.
      // The parameter controls whether resting time should be included in the average or not.
      // Throws a std::domain_error if the (included) time of the track is zero.
      speed averageSpeed(bool includeRests) const;

      // The fastest rate of positive elevation change between successive track points.
      // Returns zero if the entire track is downhill.
      // Throws a std::domain_error if the travelling time of the track is zero.
      speed maxRateOfAscent() const;

      // The fastest rate of negative elevation change between successive track points.
      // Returns zero if the entire track is uphill.
      // Throws a std::domain_error if the travelling time of the track is zero.
      speed maxRateOfDescent() const;

    protected:
      /* These vectors store the arrival time and departure time at each
       * Position in the Track.  These times are relative to the start of
       * the Track; thus arrived[0] is always 0.
       */
      std::vector<seconds> arrived;
      std::vector<seconds> departed;
  };
}

#endif
