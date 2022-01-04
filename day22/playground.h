#include <vector>

namespace coord_compress {
  namespace d1 {
    // One dimensional coordinate compression
    using Coord = int;
    struct Region {
      Coord begin,end;
    };
    using Boundaries = std::vector<Coord>;

    template <typename C>
    using Compressed = std::vector<C>;

    template <typename C>
    Compressed<C> color(C c,Region const& r);

    bool test();
  }
}