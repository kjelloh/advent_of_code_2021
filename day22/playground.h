#include <vector>

namespace coord_compress {
  namespace d1 {
    // One dimensional coordinate compression
    using Coord = int;
    struct Region {
      Coord begin,end;
    };
    using Boundaries = std::vector<Coord>;


    bool test();
  }
}