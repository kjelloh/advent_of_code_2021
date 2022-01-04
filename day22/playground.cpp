#include "playground.h"
#include <iostream>

namespace coord_compress {
  namespace d1 {
    // One dimensional coordinate compression
    // map range -> coordinate
    using Coord = int;
    struct Range {
      Coord begin,end;
    };
    struct Region {
      Coord begin,end;
    };
    using Boundaries = std::vector<Coord>;

    struct Pos {
      Coord x;
    };

    template <typename C>
    class Map {
    public:
      C const& at(Region const& r);
      C const& set(Region const& r,C const& c);
    private:
      std::vector<Coord> m_x_boundaries{};
      std::vector<C> m_colours{};
    };

    bool test() {
      // We have the following test cases (potential corner cases)
      /*
          r1            aaaaaaa 
          r2        bbb                       // disjunkt to the left             bbb aaaaaaa
          r2         bbb                      // adjacent left                     bbbaaaaaaa
          r2          bbb                     // left overlap                       bbbaaaaaa
          r2            bbb                   // inside adjacent left                 bbbaaaa
          r2            bbbbbbb               // Equal (adjacent left and right)      bbbbbbb
          r2             bbbbb                // inside                               abbbbba
          r2                bbb               // inside adjacent right                aaaabbb
          r2           bbbbbbbbb              // extended overlap                    bbbbbbbbb
          r2                  bbb             // right overlap                        aaaaaabbb
          r2                   bbb            // adjacent right                       aaaaaaabbb
          r2                    bbb           // disjunct to the right                aaaaaaa bbb
      */
     Range r1{1,11}; // [1..10]
     std::vector<Range> r2s{{-1,-3},{-1,}};
     for (auto const r2 : r2s) {

     }

     return false; 
    }
  }
}