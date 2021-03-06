#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <array>
#include <set>
#include <iterator>
#include <numeric>

extern char const* pTestp1;
extern char const* pTestp2;
extern char const* pData;

using Result = std::ptrdiff_t;

std::vector<std::pair<std::string,Result>> pTest0States{
  {R"(on x=0..1,y=0..1,z=0..1)",8}
  ,{R"(on x=1..2,y=1..2,z=1..2)",7}
};

std::vector<std::pair<std::string,Result>> pExample1States{
  {R"(on x=10..12,y=10..12,z=10..12)",27}
  ,{R"(on x=11..13,y=11..13,z=11..13)",19}
  ,{R"(off x=9..11,y=9..11,z=9..11)",-8}
  ,{R"(on x=10..10,y=10..10,z=10..10)",1}
};
std::vector<std::string> pSnippets{
// single cube
  R"(on x=-20..26,y=-36..17,z=-47..7)"
// single cube on then off
  ,R"(on x=-20..26,y=-36..17,z=-47..7
off x=-20..26,y=-36..17,z=-47..7)"
// initialization cubes
,R"(on x=-20..26,y=-36..17,z=-47..7
on x=-20..33,y=-21..23,z=-26..28
on x=-22..28,y=-29..23,z=-38..16
on x=-46..7,y=-6..46,z=-50..-1
on x=-49..1,y=-3..46,z=-24..28
on x=2..47,y=-22..22,z=-23..27
on x=-27..23,y=-28..26,z=-21..29
on x=-39..5,y=-6..47,z=-3..44
on x=-30..21,y=-8..43,z=-13..34
on x=-22..26,y=-27..20,z=-29..19
off x=-48..-32,y=26..41,z=-47..-37
on x=-12..35,y=6..50,z=-50..-2
off x=-48..-32,y=-32..-16,z=-15..-5
on x=-18..26,y=-33..15,z=-7..46
off x=-40..-22,y=-38..-28,z=23..41
on x=-16..35,y=-41..10,z=-47..6
off x=-32..-23,y=11..30,z=-14..3
on x=-49..-5,y=-3..45,z=-29..18
off x=18..30,y=-20..-8,z=-3..13
on x=-41..9,y=-7..43,z=-33..15)"
// example 2 minus initialisation cubes (should remove 474140 cubes according to example in part 2)
,R"(on x=-5..47,y=-31..22,z=-19..33
on x=-44..5,y=-27..21,z=-14..35
on x=-49..-1,y=-11..42,z=-10..38
on x=-20..34,y=-40..6,z=-44..1
off x=26..39,y=40..50,z=-2..11
on x=-41..5,y=-41..6,z=-36..8
off x=-43..-33,y=-45..-28,z=7..25
on x=-33..15,y=-32..19,z=-34..11
off x=35..47,y=-46..-34,z=-11..5
on x=-14..36,y=-6..44,z=-16..29
on x=-57795..-6158,y=29564..72030,z=20435..90618
on x=36731..105352,y=-21140..28532,z=16094..90401
on x=30999..107136,y=-53464..15513,z=8553..71215
on x=13528..83982,y=-99403..-27377,z=-24141..23996
on x=-72682..-12347,y=18159..111354,z=7391..80950
on x=-1060..80757,y=-65301..-20884,z=-103788..-16709
on x=-83015..-9461,y=-72160..-8347,z=-81239..-26856
on x=-52752..22273,y=-49450..9096,z=54442..119054
on x=-29982..40483,y=-108474..-28371,z=-24328..38471
on x=-4958..62750,y=40422..118853,z=-7672..65583
on x=55694..108686,y=-43367..46958,z=-26781..48729
on x=-98497..-18186,y=-63569..3412,z=1232..88485
on x=-726..56291,y=-62629..13224,z=18033..85226
on x=-110886..-34664,y=-81338..-8658,z=8914..63723
on x=-55829..24974,y=-16897..54165,z=-121762..-28058
on x=-65152..-11147,y=22489..91432,z=-58782..1780
on x=-120100..-32970,y=-46592..27473,z=-11695..61039
on x=-18631..37533,y=-124565..-50804,z=-35667..28308
on x=-57817..18248,y=49321..117703,z=5745..55881
on x=14781..98692,y=-1341..70827,z=15753..70151
on x=-34419..55919,y=-19626..40991,z=39015..114138
on x=-60785..11593,y=-56135..2999,z=-95368..-26915
on x=-32178..58085,y=17647..101866,z=-91405..-8878
on x=-53655..12091,y=50097..105568,z=-75335..-4862
on x=-111166..-40997,y=-71714..2688,z=5609..50954
on x=-16602..70118,y=-98693..-44401,z=5197..76897
on x=16383..101554,y=4615..83635,z=-44907..18747
off x=-95822..-15171,y=-19987..48940,z=10804..104439
on x=-89813..-14614,y=16069..88491,z=-3297..45228
on x=41075..99376,y=-20427..49978,z=-52012..13762
on x=-21330..50085,y=-17944..62733,z=-112280..-30197
on x=-16478..35915,y=36008..118594,z=-7885..47086
off x=-98156..-27851,y=-49952..43171,z=-99005..-8456
off x=2032..69770,y=-71013..4824,z=7471..94418
on x=43670..120875,y=-42068..12382,z=-24787..38892
off x=37514..111226,y=-45862..25743,z=-16714..54663
off x=25699..97951,y=-30668..59918,z=-15349..69697
off x=-44271..17935,y=-9516..60759,z=49131..112598
on x=-61695..-5813,y=40978..94975,z=8655..80240
off x=-101086..-9439,y=-7088..67543,z=33935..83858
off x=18020..114017,y=-48931..32606,z=21474..89843
off x=-77139..10506,y=-89994..-18797,z=-80..59318
off x=8476..79288,y=-75520..11602,z=-96624..-24783
on x=-47488..-1262,y=24338..100707,z=16292..72967
off x=-84341..13987,y=2429..92914,z=-90671..-1318
off x=-37810..49457,y=-71013..-7894,z=-105357..-13188
off x=-27365..46395,y=31009..98017,z=15428..76570
off x=-70369..-16548,y=22648..78696,z=-1892..86821
on x=-53470..21291,y=-120233..-33476,z=-44150..38147
off x=-93533..-4276,y=-16170..68771,z=-104985..-24507
off x=-50..50,y=-50..50,z=-50..50)" // should remove 474140 cubes according to example in part 2
};

using Answers = std::vector<std::pair<std::string,Result>>;

using Coord = int;
template <typename T>
using CoordRange = std::pair<T, T>;
class Vector {
public:
  Vector(Coord x, Coord y, Coord z) : m_v{x,y,z} {}
  bool operator<(Vector const& other) const { return m_v < other.m_v; }
  Coord x() const { return m_v[0]; }
  Coord y() const { return m_v[1]; }
  Coord z() const { return m_v[2]; }
private:
  std::array<Coord, 3> m_v;
};
// A cubeoid is a volume of cubes all in the same state on/off
// A cubeoid may grow with the cubes of another cubeoid (see on())
class Cuboid {
public:
  Cuboid(bool on,CoordRange<std::string> xr, CoordRange<std::string> yr, CoordRange<std::string> zr)
    :  m_on{on}
      ,m_xr{ coord_range(xr)}
      ,m_yr{ coord_range(yr)}
      ,m_zr{ coord_range(zr)} {}
  std::string to_string() const {
    std::ostringstream os{};
    os << "<cuboid>";
    os << " {" << std::to_string(m_xr.first) << "," << std::to_string(m_yr.first) << "," << std::to_string(m_zr.first) << "}";
    os << " .. ";
    os << " {" << std::to_string(m_xr.second) << "," << std::to_string(m_yr.second) << "," << std::to_string(m_zr.second) << "}";
    if (m_on) os << " ON!";
    else os << " off";
    return os.str();
  }
  bool on() const {return m_on;}
  CoordRange<Coord> x_range() const {return m_xr;}
  CoordRange<Coord> y_range() const {return m_yr;}
  CoordRange<Coord> z_range() const {return m_zr;}
  void for_each_50(auto f) const {
    // apply provided mutating function f to each cubeoid in 50x50x50 cube coordinate volume
    for (Coord x = std::max(-50,m_xr.first); x <= std::min(50,m_xr.second); x++) {
      for (Coord y = std::max(-50,m_yr.first); y <= std::min(50,m_yr.second); y++) {
        for (Coord z = std::max(-50,m_zr.first); z <= std::min(50,m_zr.second); z++) {
          f(Vector{ x,y,z },m_on);
        }
      }
    }
  }
private:
  bool m_on;
  CoordRange<Coord> coord_range(CoordRange<std::string> const& r) {
    auto first = std::stoi(r.first);
    auto second = std::stoi(r.second);
    return {first,second};
  }
  CoordRange<Coord> m_xr;
  CoordRange<Coord> m_yr;
  CoordRange<Coord> m_zr;
};

using Model = std::vector<Cuboid>;

std::pair<std::string, std::string> split(std::string const& token,std::string delim) {
    auto pos = token.find(delim);
    auto left = token.substr(0,pos);
    auto right = token.substr(pos+delim.size());
    return {left,right};
}
Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in, line)) {
      auto [left, right] = split(line, " ");
      // std::cout << "left \"" << left << "\" right \"" << right << "\"";
      bool on = left == "on";
      auto [xs, right_x] = split(right, ",");
      auto [ys, right_y] = split(right_x, ",");
      auto [zs, right_z] = split(right_y, ",");
      // std::cout << "\nx:" << xs << " y:" << ys << " z:" << zs;
      auto [xs_left, xs_right] = split(xs, "=");
      auto [ys_left, ys_right] = split(ys, "=");
      auto [zs_left, zs_right] = split(zs, "=");
      // std::cout << "x:" << xs_right << " y:" << ys_right << " z:" << zs_right;
      auto [xs1, xs2] = split(xs_right, "..");
      auto [ys1, ys2] = split(ys_right, "..");
      auto [zs1, zs2] = split(zs_right, "..");
      // std::cout << "\n" << "x:" << xs1 << ".." << xs2 << " y:" << ys1 << ".." << ys2 << " z:" << zs1 << ".." << zs2;
      Cuboid cuboid{ on,{xs1,xs2},{ys1,ys2},{zs1,zs2} };
      result.push_back(cuboid);
    }
    std::cout << "\ncuboid count " << result.size();
    for (auto const& cuboid : result) {
      std::cout << "\n" << cuboid.to_string();
    }
    return result;
}

namespace part1 {
  // A reactor wraps a, potentally sparse, set of cubes (each being on or off) 
  class Reactor {
  public:
    using CountChange = Result;
    CountChange apply(Cuboid const& command) {
      auto count_before{this->cubes_on_count()};
      this->set_cubes(command);
      auto count_after{this->cubes_on_count()};
      return count_after-count_before;
    }
    void set_cube(Vector const& v,bool on) {
      if (on) {
        m_cubes.insert(v);
      }
      else {
        m_cubes.erase(v);
      }
    }
    void set_cubes(Cuboid const& cuboid) {
      // call for_each... of cuboid with lambda to set each required cube to on
      cuboid.for_each_50([this](Vector const& v,bool on) {
        this->set_cube(v, on);
        });
    }
    Result cubes_on_count() const { return m_cubes.size(); }
  private:
    std::set<Vector> m_cubes{}; // reactor cubes in a volume spanned by Vectors
  };

  Result solve_for(char const* pData) {
      Result result{};
      Reactor reactor{};
      if (pData==nullptr) {
        // Run the test sequence
        for (auto const& entry : pExample1States) {
          auto [pData,count] = entry;
          std::stringstream in{ pData };
          auto cuboids = parse(in);
          std::cout << "\napplied:" << pData;
          if (auto count_change = reactor.apply(cuboids.front());count_change == count) std::cout << " : " << count_change << " == " << count <<   " passed";
          else std::cout << " : " << count_change << " != " << count <<   " FAILED";
        }
      }
      else {
        // Solve puzzle for indata
        std::stringstream in{ pData };
        auto puzzle_model = parse(in);
        for (auto const& cuboid : puzzle_model) {
          reactor.set_cubes(cuboid);
        }
        result = reactor.cubes_on_count();
      }

      return result;
  }
}

namespace part2 {
  struct Reactor {
    public:
    using CountChange = Result;
    CountChange apply(std::vector<Cuboid> const& cuboids) {
      static int progress_counter{0};
      CountChange result{};

      // Kudos to Jonathan Paulsson for teaching me about this approach (https://youtu.be/7gW_h0RTDd8?t=2653)
      // Kudos to Neal Wu for his beatiful C++ teaching of the same approach (https://youtu.be/YKpViLcTp64?t=882)

      // Observation 1: All we need to process are the cuboid boundaries and the state of cubes within each cuboid.
      // "Proof": All cube state switching happens at cuboid faces. Between such faces all cubes have the same state.

      // Approach - compress down the huge 3D cube space to a matrix of all cuboid boundaries.
      // let this grid keep track of the boolean state "on" of each volume defined by all the boundaries 
      // of the applied cuboids.
      std::vector<std::vector<std::vector<bool>>> on_off_grid{};
      // Observation 2: if we apply the "on" and "off" for each space defined by each cuboid in this on/off grid
      // we should get the final cube states represented by the grid.

      // Problem 1: We need to map the huge 3D cube space to indexies in the on_off_grid matrix.
      // Solution part 1: Create one vector for each dimension and put the cube boundaries in that dimension into them.
      std::vector<Coord> x_boundaries{},y_boundaries{},z_boundaries{};
      for (auto const& cuboid : cuboids) {
        // Update the boundaries in each dimesnion where the state of a cube changes.
        // This means the first in each range and the +1 after the last one in the provided range!
        x_boundaries.push_back(cuboid.x_range().first);
        x_boundaries.push_back(cuboid.x_range().second+1); // +1 to pin point the cube outside the range (potentially another state than those in range)
        y_boundaries.push_back(cuboid.y_range().first);
        y_boundaries.push_back(cuboid.y_range().second+1); // +1 to pin point the cube outside the range (potentially another state than those in range)
        z_boundaries.push_back(cuboid.z_range().first);
        z_boundaries.push_back(cuboid.z_range().second+1); // +1 to pin point the cube outside the range (potentially another state than those in range)
      }

      // Sorting ensure we have a continous "space" in each dimension
      std::sort(x_boundaries.begin(),x_boundaries.end());
      std::sort(y_boundaries.begin(),y_boundaries.end());
      std::sort(z_boundaries.begin(),z_boundaries.end());

      {
        // Debug
        // std::cout << "\nx coordinates of state changes:";
        // for (auto c : x_boundaries) std::cout << " " << c;
        // std::cout << "\ny coordinates of state changes:";
        // for (auto c : y_boundaries) std::cout << " " << c;
        // std::cout << "\nz coordinates of state changes:";
        // for (auto c : z_boundaries) std::cout << " " << c;
        /*
        // Example
        x coordinates of state changes: 9 10 10 11 11 12 13 14
        y coordinates of state changes: 9 10 10 11 11 12 13 14
        z coordinates of state changes: 9 10 10 11 11 12 13 14

        Define ranges [9,10[, [10,10[, [10,11[, [11,11[, [11,12[, [12,13[ [13,14[

        */
      }

      // Observation: the size of the boundary vectors now defines the dimensions of the on_off_grid
      auto const boundaries_count = x_boundaries.size();
      // We can now init the on_off_grid with correct dimensions and init state "off" for all "space"
      on_off_grid = std::vector(boundaries_count,std::vector(boundaries_count,std::vector(boundaries_count,false)));
      // Solution part 2: Imagine a function on_off_grid_index and use it to fill the on_off_grid
      // as definde by each cuboid to apply
      auto on_off_grid_index = [](auto const& boundaries,Coord c)  {
        // Return the index in the boundaries vector of the provided 3d boundary c.
        // This is the distance from the beginning of the sorted boundaries vector
        // to the position of c in the vector
        auto iter = std::lower_bound(boundaries.begin(),boundaries.end(),c);
        // Note: We need to use lower_bound (and not find) as we added end+1 for the end-boundary so when
        // called with a cuboid end boundary it will not be exactly found. the lower_bound will :)
        return std::distance(boundaries.begin(),iter);
      };
      for (auto const& cuboid : cuboids) {
        /*
        The on_off grid has a lcoation for each r a n g e of coordinates with the same state.
        Thus, a boundary vector {c0,c1,c2...cn} defines ranges [c0,c1[, [c1,c2[, [c2,*[ .. [*,cn[
        with index 0..n                     on_off grid index    0        1        2        n-1     
        */
        auto on_off_x_first = on_off_grid_index(x_boundaries,cuboid.x_range().first);
        auto on_off_x_second = on_off_grid_index(x_boundaries,cuboid.x_range().second+1);
        auto on_off_y_first = on_off_grid_index(y_boundaries,cuboid.y_range().first);
        auto on_off_y_second = on_off_grid_index(y_boundaries,cuboid.y_range().second+1);
        auto on_off_z_first = on_off_grid_index(z_boundaries,cuboid.z_range().first);
        auto on_off_z_second = on_off_grid_index(z_boundaries,cuboid.z_range().second+1);
        bool state = cuboid.on();
        // We can now update the state of each range using the index into the on_off grid
        // Note: loop over range [first..last[ (last is NOT part of the range of coord with the same state)
        for (int on_off_x=on_off_x_first;on_off_x<on_off_x_second;on_off_x++) {
          for (int on_off_y=on_off_y_first;on_off_y<on_off_y_second;on_off_y++) {
            for (int on_off_z=on_off_z_first;on_off_z<on_off_z_second;on_off_z++) {
              // {
              //   // Debug
              //   std::cout << "\non_off_x " << on_off_x << " on_off_y " << on_off_y << " on_off_z " << on_off_z;
              // }
              /*
              Example
              ...
              on_off_x 5 on_off_y 5 on_off_z 6
              on_off_x 5 on_off_y 6 on_off_z 3
              on_off_x 5 on_off_y 6 on_off_z 4
              on_off_x 5 on_off_y 6 on_off_z 5
              on_off_x 5 on_off_y 6 on_off_z 6
              on_off_x 6 on_off_y 3 on_off_z 3
              ...
              
              */
              on_off_grid[on_off_x][on_off_y][on_off_z] = state; // For this cuboid, set this state
              // Note that overlapping cuboids will overwrite prevous state
              // in the overlapping region (just as they should)
              if (progress_counter++%10000000==0) std::cout << '.' << std::flush;
            }            
          }
        }
      }
      // Now Magic has happened! The on_off_grid now represents the on/off state of the 3d space.
      // We "just" needs to map the on-regions back to actual 3d space dimensions to get the
      // "volume" of all  3d space that is "on".
      Result on_volume_count{0};
      for (int on_off_x=0;on_off_x < boundaries_count-1;on_off_x++) {
        for (int on_off_y=0;on_off_y < boundaries_count-1;on_off_y++) {
          for (int on_off_z=0;on_off_z < boundaries_count-1;on_off_z++) {
            // {
            //   // Debug
            //   std::cout << "\ncuboid"
            //   << " [{x:" << x_boundaries[on_off_x]
            //   << ",y:"  << y_boundaries[on_off_y]
            //   << ",z:"  << z_boundaries[on_off_z]
            //   << "}"
            //   << " -> [{x:" << x_boundaries[on_off_x+1]
            //   << ",y:"  << y_boundaries[on_off_y+1]
            //   << ",z:"  << z_boundaries[on_off_z+1]
            //   << "}[";
            //   /*
            //   Example (note that we will get zero voume cubes for overlapping cuts)
            //   I suppose this is a consequence of the fact we get equal counts of x,y and z ranges?
            //   And if any of these are zero (cut on same coorinate) the volume will be zero (although the range in other coordinates are >0)?
            //   TODO: Can we eliminate adding zero volume cut cubes?
            //
            //  Example:
            //   ...
            //   cuboid [{x:11,y:11,z:11} -> [{x:12,y:12,z:12}[ off
            //   cuboid [{x:11,y:11,z:12} -> [{x:12,y:12,z:13}[ ON volume:1
            //   cuboid [{x:11,y:11,z:13} -> [{x:12,y:12,z:14}[ ON volume:1
            //   cuboid [{x:11,y:12,z:9} -> [{x:12,y:13,z:10}[ off
            //   cuboid [{x:11,y:12,z:10} -> [{x:12,y:13,z:10}[ ON volume:0
            //   cuboid [{x:11,y:12,z:10} -> [{x:12,y:13,z:11}[ ON volume:1
            //   cuboid [{x:11,y:12,z:11} -> [{x:12,y:13,z:11}[ ON volume:0
            //   cuboid [{x:11,y:12,z:11} -> [{x:12,y:13,z:12}[ ON volume:1
            //   cuboid [{x:11,y:12,z:12} -> [{x:12,y:13,z:13}[ ON volume:1
            //   cuboid [{x:11,y:12,z:13} -> [{x:12,y:13,z:14}[ ON volume:1
            //   cuboid [{x:11,y:13,z:9} -> [{x:12,y:14,z:10}[ off
            //   ...
            //   */
            // }

            if (on_off_grid[on_off_x][on_off_y][on_off_z]) {
              // Note that the range is [..[ (the last coordinate is NOT part of the range)
              // Thus end-start IS the count of cubes in the range [..[
              Result dx = (x_boundaries[on_off_x+1] - x_boundaries[on_off_x]);
              Result dy = (y_boundaries[on_off_y+1] - y_boundaries[on_off_y]);
              Result dz = (z_boundaries[on_off_z+1] - z_boundaries[on_off_z]);
              auto volume = dx*dy*dz;
              on_volume_count += volume;
              if (progress_counter++%1000000==0) std::cout << "\nCount:" << on_volume_count << std::flush;
            }
          }
        }
      }
      result = on_volume_count;
      return result;
    }
  private:
  };

  Result solve_for(char const* pData) {
      Result result{};
      Reactor reactor{};
      if (pData==nullptr) {
        // Run the test sequence
        // auto pExampleStates = pTest0States;
        auto pExampleStates = pExample1States;
        std::vector<Result> count_hist{{0}};
        for (int i=0; i<pExampleStates.size();i++) {
          std::pair<std::string,Result> unit{};
          auto entry = std::accumulate(pExampleStates.begin(),pExampleStates.begin()+i+1,unit,[](auto acc,auto const& entry){
            if (acc.first.size()>0) acc.first += "\n";
            acc.first += entry.first;
            acc.second = entry.second;
            // std::cout << "\nacc.first=" << acc.first << ",acc.second=" << acc.second << std::flush;
            return acc;
          });
          // std::cout << "\nentry.first=" << entry.first << ",entry.second=" << entry.second << std::flush;
          auto [pData,expected_delta] = entry;
          // std::cout << "\npData=" << pData;
          // std::cout << "\ncount=" << count;
          std::stringstream in{ pData };
          auto cuboids = parse(in);
          std::cout << "\napplied:\n" << pData;
          auto prev_count = count_hist.back();
          auto count = reactor.apply(cuboids);
          count_hist.push_back(count);
          auto delta = count-prev_count;
          if (delta == expected_delta) std::cout << "\noutcome " << delta << " == expected " << expected_delta <<   " ==> passed";
          else std::cout << "\noutcome " << delta << " != expected " << expected_delta <<   " ==> FAILED";
        }
      }
      else {
        // Solve Puzzle for indata
        std::stringstream in{ pData };
        auto cuboids = parse(in);
        result = reactor.apply(cuboids);
      }
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(nullptr)});
  answers.push_back({"Part 1 Test",part1::solve_for(pTestp1)});
  answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(nullptr)});  
  // answers.push_back({"Part 2 Test 1",part2::solve_for(pSnippets[2].c_str())});
  // answers.push_back({"Part 2 Test 1",part2::solve_for(pTestp1)}); 
  answers.push_back({"Part 2 Test",part2::solve_for(pTestp2)});
  // answers.push_back({"Part 2 Test 1",part2::solve_for(pSnippets[3].c_str())});
  // std::cout << "\ndiff " << answers[answers.size()-2].second - answers[answers.size()-1].second;
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // Visual Studio fix if unchecking "Tools->Options->Debugging->Automatically close the console when debugging stops" does not work
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTestp1 = R"(on x=-20..26,y=-36..17,z=-47..7
on x=-20..33,y=-21..23,z=-26..28
on x=-22..28,y=-29..23,z=-38..16
on x=-46..7,y=-6..46,z=-50..-1
on x=-49..1,y=-3..46,z=-24..28
on x=2..47,y=-22..22,z=-23..27
on x=-27..23,y=-28..26,z=-21..29
on x=-39..5,y=-6..47,z=-3..44
on x=-30..21,y=-8..43,z=-13..34
on x=-22..26,y=-27..20,z=-29..19
off x=-48..-32,y=26..41,z=-47..-37
on x=-12..35,y=6..50,z=-50..-2
off x=-48..-32,y=-32..-16,z=-15..-5
on x=-18..26,y=-33..15,z=-7..46
off x=-40..-22,y=-38..-28,z=23..41
on x=-16..35,y=-41..10,z=-47..6
off x=-32..-23,y=11..30,z=-14..3
on x=-49..-5,y=-3..45,z=-29..18
off x=18..30,y=-20..-8,z=-3..13
on x=-41..9,y=-7..43,z=-33..15
on x=-54112..-39298,y=-85059..-49293,z=-27449..7877
on x=967..23432,y=45373..81175,z=27513..53682)";
char const* pTestp2 = R"(on x=-5..47,y=-31..22,z=-19..33
on x=-44..5,y=-27..21,z=-14..35
on x=-49..-1,y=-11..42,z=-10..38
on x=-20..34,y=-40..6,z=-44..1
off x=26..39,y=40..50,z=-2..11
on x=-41..5,y=-41..6,z=-36..8
off x=-43..-33,y=-45..-28,z=7..25
on x=-33..15,y=-32..19,z=-34..11
off x=35..47,y=-46..-34,z=-11..5
on x=-14..36,y=-6..44,z=-16..29
on x=-57795..-6158,y=29564..72030,z=20435..90618
on x=36731..105352,y=-21140..28532,z=16094..90401
on x=30999..107136,y=-53464..15513,z=8553..71215
on x=13528..83982,y=-99403..-27377,z=-24141..23996
on x=-72682..-12347,y=18159..111354,z=7391..80950
on x=-1060..80757,y=-65301..-20884,z=-103788..-16709
on x=-83015..-9461,y=-72160..-8347,z=-81239..-26856
on x=-52752..22273,y=-49450..9096,z=54442..119054
on x=-29982..40483,y=-108474..-28371,z=-24328..38471
on x=-4958..62750,y=40422..118853,z=-7672..65583
on x=55694..108686,y=-43367..46958,z=-26781..48729
on x=-98497..-18186,y=-63569..3412,z=1232..88485
on x=-726..56291,y=-62629..13224,z=18033..85226
on x=-110886..-34664,y=-81338..-8658,z=8914..63723
on x=-55829..24974,y=-16897..54165,z=-121762..-28058
on x=-65152..-11147,y=22489..91432,z=-58782..1780
on x=-120100..-32970,y=-46592..27473,z=-11695..61039
on x=-18631..37533,y=-124565..-50804,z=-35667..28308
on x=-57817..18248,y=49321..117703,z=5745..55881
on x=14781..98692,y=-1341..70827,z=15753..70151
on x=-34419..55919,y=-19626..40991,z=39015..114138
on x=-60785..11593,y=-56135..2999,z=-95368..-26915
on x=-32178..58085,y=17647..101866,z=-91405..-8878
on x=-53655..12091,y=50097..105568,z=-75335..-4862
on x=-111166..-40997,y=-71714..2688,z=5609..50954
on x=-16602..70118,y=-98693..-44401,z=5197..76897
on x=16383..101554,y=4615..83635,z=-44907..18747
off x=-95822..-15171,y=-19987..48940,z=10804..104439
on x=-89813..-14614,y=16069..88491,z=-3297..45228
on x=41075..99376,y=-20427..49978,z=-52012..13762
on x=-21330..50085,y=-17944..62733,z=-112280..-30197
on x=-16478..35915,y=36008..118594,z=-7885..47086
off x=-98156..-27851,y=-49952..43171,z=-99005..-8456
off x=2032..69770,y=-71013..4824,z=7471..94418
on x=43670..120875,y=-42068..12382,z=-24787..38892
off x=37514..111226,y=-45862..25743,z=-16714..54663
off x=25699..97951,y=-30668..59918,z=-15349..69697
off x=-44271..17935,y=-9516..60759,z=49131..112598
on x=-61695..-5813,y=40978..94975,z=8655..80240
off x=-101086..-9439,y=-7088..67543,z=33935..83858
off x=18020..114017,y=-48931..32606,z=21474..89843
off x=-77139..10506,y=-89994..-18797,z=-80..59318
off x=8476..79288,y=-75520..11602,z=-96624..-24783
on x=-47488..-1262,y=24338..100707,z=16292..72967
off x=-84341..13987,y=2429..92914,z=-90671..-1318
off x=-37810..49457,y=-71013..-7894,z=-105357..-13188
off x=-27365..46395,y=31009..98017,z=15428..76570
off x=-70369..-16548,y=22648..78696,z=-1892..86821
on x=-53470..21291,y=-120233..-33476,z=-44150..38147
off x=-93533..-4276,y=-16170..68771,z=-104985..-24507)";
char const* pData = R"(on x=-33..18,y=-35..11,z=-49..2
on x=-14..32,y=5..49,z=-42..5
on x=-28..18,y=-38..10,z=-14..33
on x=-40..6,y=-22..32,z=-32..13
on x=-14..37,y=-37..12,z=-31..19
on x=-24..30,y=-40..6,z=-19..27
on x=-29..15,y=-44..7,z=-22..22
on x=-49..2,y=-29..15,z=-1..48
on x=-1..45,y=-6..39,z=-16..37
on x=-15..30,y=-35..14,z=4..49
off x=20..39,y=29..46,z=-30..-15
on x=-36..9,y=-8..38,z=-38..10
off x=-22..-4,y=10..19,z=-4..10
on x=-43..9,y=-37..16,z=-24..23
off x=10..29,y=1..10,z=-2..16
on x=-18..34,y=-39..8,z=-31..23
off x=-2..7,y=-41..-23,z=4..23
on x=-18..31,y=-27..27,z=-23..25
off x=17..33,y=5..14,z=-26..-7
on x=-29..25,y=-1..44,z=-5..44
on x=-2089..22263,y=63383..83317,z=1521..34560
on x=16324..26707,y=-76181..-51644,z=27623..60727
on x=-95252..-74929,y=-36262..-8635,z=-14014..-4726
on x=26087..56689,y=13866..33100,z=-77678..-54222
on x=-58261..-34558,y=-52429..-39076,z=-46433..-17880
on x=38567..59604,y=66243..73633,z=-4724..7559
on x=-76223..-52004,y=-4022..17420,z=-56813..-31010
on x=-28531..-6777,y=52497..78911,z=39320..53754
on x=-1705..20943,y=71494..92397,z=2586..17205
on x=43970..50168,y=-44993..-18711,z=-68417..-46597
on x=-2878..23564,y=41697..70518,z=42674..68678
on x=22987..51001,y=-74194..-62557,z=-40942..-24407
on x=10735..33969,y=26636..35170,z=62555..84961
on x=-87260..-58460,y=-25011..9170,z=-40488..-16433
on x=-41317..-19098,y=55028..74495,z=9284..32979
on x=57730..86394,y=-5618..4073,z=8825..29287
on x=70498..95095,y=-26060..-15204,z=-11673..3577
on x=-12181..6283,y=-76734..-56136,z=-41358..-26351
on x=55479..68566,y=-65998..-51191,z=-388..13824
on x=-7142..9719,y=17804..41523,z=-73454..-51015
on x=-6780..13903,y=64955..76512,z=-35477..-12220
on x=8140..17232,y=-84619..-62523,z=-28417..-860
on x=-59883..-49433,y=-65970..-42610,z=8348..17483
on x=-46361..-34652,y=-77141..-58593,z=-32375..2092
on x=18682..23347,y=-70715..-62353,z=15488..48836
on x=13625..33572,y=-53090..-39232,z=38313..62732
on x=-7845..8128,y=72275..83684,z=9204..44598
on x=30099..52396,y=-37319..-13384,z=-65551..-51334
on x=66812..86665,y=-12147..-8315,z=-14456..18299
on x=-80768..-62714,y=12166..32801,z=9298..28465
on x=32917..51529,y=-39942..-11579,z=-77830..-58654
on x=6795..30180,y=-52748..-23421,z=-79235..-66193
on x=41104..59365,y=-65904..-44389,z=-58998..-32127
on x=46835..68448,y=-13278..-4034,z=47409..74764
on x=61812..89996,y=-11979..17125,z=-42861..-30521
on x=36211..58568,y=65204..73638,z=-2374..22331
on x=24752..29245,y=68946..77065,z=21993..33524
on x=64234..90251,y=15294..35642,z=26838..33725
on x=-19948..6112,y=-38109..-27877,z=62697..87704
on x=-45769..-27330,y=-76264..-42424,z=34120..46556
on x=-64655..-37560,y=32027..51779,z=36246..55408
on x=46729..77810,y=30499..52592,z=22011..48493
on x=-59194..-47087,y=61036..63539,z=-9127..12439
on x=31894..45260,y=56306..80190,z=-39056..-4346
on x=-28505..-11114,y=76279..91855,z=-17517..2342
on x=-29827..357,y=-55982..-33070,z=53295..78681
on x=61904..90365,y=-45148..-26196,z=-31689..-11315
on x=-63745..-30029,y=-60515..-23200,z=-53318..-41274
on x=35857..48971,y=-18159..16867,z=-83848..-48218
on x=-58426..-37407,y=47038..72640,z=2498..22588
on x=40878..41799,y=-86884..-48581,z=2168..20716
on x=-26137..-535,y=60867..81985,z=-60883..-30553
on x=-49390..-18873,y=19679..41934,z=64209..74274
on x=-13205..4850,y=-54788..-33129,z=-74136..-52452
on x=34452..69258,y=-18117..-14331,z=53496..73956
on x=-26084..-6353,y=-28975..-2494,z=57753..84222
on x=-55087..-32324,y=-62117..-47889,z=-36986..-26405
on x=-49256..-42251,y=-58341..-32455,z=35370..57488
on x=-29709..1208,y=44395..60836,z=44943..75568
on x=9674..39186,y=-85531..-68265,z=11474..22020
on x=-54733..-30617,y=-57541..-41688,z=44653..71542
on x=58013..78420,y=-31202..-19242,z=18818..28322
on x=-68634..-53973,y=-58009..-31614,z=-5131..5865
on x=-16576..3952,y=63366..90574,z=8992..37515
on x=-7386..14817,y=-14155..5703,z=66136..88553
on x=19106..54924,y=-12140..15434,z=68415..80932
on x=7336..31425,y=65432..82588,z=8047..29593
on x=22525..33599,y=-68372..-39358,z=-68143..-43500
on x=5213..21274,y=-39249..-27045,z=-91831..-72038
on x=-31642..-16632,y=-2869..8672,z=66852..81506
on x=70305..84886,y=-41625..-15439,z=-29089..-2340
on x=-60096..-54846,y=-46888..-14091,z=-57849..-36369
on x=-44911..-23725,y=-22548..198,z=55411..76767
on x=-57512..-33022,y=-64054..-54691,z=22591..39559
on x=535..39401,y=56745..78405,z=23560..28326
on x=-56374..-39060,y=-77520..-41804,z=-7273..13316
on x=7013..28554,y=32070..54196,z=47529..70955
on x=-76288..-56876,y=-56665..-25593,z=-16778..-6302
on x=-37510..-11747,y=61094..74622,z=-2370..18659
on x=-58709..-30616,y=-33464..-11638,z=50961..72748
on x=-55454..-27679,y=-82741..-59621,z=-9194..15631
on x=13172..40388,y=11138..40063,z=-82760..-65315
on x=14388..27772,y=-67554..-46788,z=-70583..-43179
on x=15748..48411,y=-58853..-46690,z=45719..67485
on x=-64826..-42121,y=-69677..-41462,z=-19124..10506
on x=-38968..-4505,y=21318..56103,z=53808..76243
on x=52992..73842,y=-45007..-30798,z=-40845..-17070
on x=-72448..-52368,y=-13003..12727,z=-51653..-39744
on x=-89717..-56812,y=-11429..11004,z=-33713..-29320
on x=-82797..-68105,y=-907..18442,z=-48479..-36446
on x=-52400..-36160,y=-2729..11358,z=-73134..-48314
on x=-25713..-1239,y=45923..57024,z=-64104..-48955
on x=-24274..3585,y=-91301..-60004,z=-36996..-8396
on x=-72952..-55266,y=-33277..-2819,z=-34959..-11204
on x=-66793..-46197,y=50987..54931,z=-27353..-16284
on x=43392..52101,y=-14627..12623,z=-71060..-48522
on x=-77723..-72809,y=6362..11492,z=9252..30717
on x=32463..54093,y=18767..32223,z=-79630..-54627
on x=26726..36372,y=-18434..723,z=54237..77721
on x=62209..80598,y=-43071..-26185,z=11647..20283
on x=-12408..8805,y=-82322..-75005,z=-7927..12271
on x=66471..72381,y=-1981..28059,z=27987..48109
on x=-80370..-49201,y=-61716..-49683,z=-30867..-7453
on x=37596..66737,y=-68892..-60777,z=-33858..-8114
on x=17321..30730,y=-52278..-42302,z=54029..68729
on x=-77142..-71446,y=7200..27046,z=-14711..12602
on x=-22544..-1958,y=-33075..-2255,z=-86145..-70682
on x=55145..79591,y=-11592..16417,z=-30690..-11963
on x=-13258..8815,y=64606..98004,z=-8762..6548
on x=11099..20143,y=57055..77284,z=-23767..-18751
on x=52489..83696,y=15746..31820,z=8280..43534
on x=-66484..-41050,y=-57811..-31218,z=24204..41390
on x=60564..85343,y=-27885..-15572,z=-28159..7052
on x=-317..14600,y=68181..76576,z=-39440..-13155
on x=-75121..-51737,y=-40321..-29860,z=15647..37621
on x=-54204..-41260,y=5051..35207,z=53197..81544
on x=36998..51680,y=51159..76975,z=5399..12759
on x=-16863..16337,y=-80953..-58351,z=-59967..-35458
on x=31744..60271,y=-14750..10965,z=49476..82924
on x=-2256..18179,y=-73825..-67358,z=-34072..-25314
on x=17732..43807,y=62479..77780,z=13938..28034
on x=1622..28357,y=-82169..-57048,z=-25837..88
on x=-40461..-9223,y=62104..86603,z=14710..22383
on x=-81227..-61564,y=-42653..-23629,z=29983..44317
on x=20880..48206,y=41863..67134,z=32598..46063
on x=-96416..-62247,y=-18355..6029,z=-5981..7217
on x=-57869..-30585,y=56288..73456,z=16646..22837
on x=75450..92466,y=-20645..12847,z=-25402..-14103
on x=-5465..14054,y=-35719..-4050,z=71849..89372
on x=-61081..-25279,y=-66905..-48434,z=23859..40935
on x=-7800..10807,y=71020..85725,z=-15915..7901
on x=53985..76201,y=20582..53668,z=-1323..23345
on x=-85787..-63425,y=27676..42137,z=-9285..-3133
on x=48336..68487,y=-38490..98,z=41624..59831
on x=-78012..-53039,y=30299..67166,z=19090..40584
on x=5113..13878,y=-78897..-73837,z=11134..18475
on x=41756..68743,y=-155..35498,z=50442..64705
on x=-81270..-68840,y=-17576..-3690,z=-32135..-14787
on x=8219..40862,y=8918..25218,z=-77252..-54208
on x=39105..44578,y=66714..83921,z=15354..22292
on x=46189..61321,y=43841..68088,z=19301..53413
on x=-62770..-57542,y=-20733..14047,z=-67275..-32150
on x=65480..84873,y=3073..19647,z=6345..23441
on x=46867..47491,y=46506..52780,z=26672..40094
on x=17013..41829,y=-11136..13219,z=-82179..-70797
on x=-34036..-23731,y=33357..53053,z=51260..72738
on x=31113..47177,y=-66933..-39151,z=-61354..-38916
on x=34145..51166,y=-3683..10926,z=52987..77874
on x=41954..65645,y=-56607..-45672,z=-4255..10356
on x=-68089..-55495,y=46167..65284,z=-2384..10704
on x=7890..33438,y=37201..73446,z=-61118..-42873
on x=-7504..12073,y=-76259..-56952,z=29469..62400
on x=13684..21664,y=-31197..-1820,z=68848..89615
on x=-85435..-60015,y=-25411..-2834,z=-13167..8182
on x=-76535..-58434,y=-29534..475,z=-33953..-16134
on x=-59255..-45581,y=42904..53487,z=-44277..-36999
on x=24422..52934,y=-69287..-47156,z=27348..40808
on x=-77855..-49832,y=-40133..-15312,z=-44484..-35346
on x=-41154..-18246,y=4608..24334,z=73408..80625
on x=-49987..-14202,y=25483..46341,z=-72668..-53755
on x=13538..18051,y=23397..42374,z=64790..80510
on x=54955..79143,y=-54794..-41945,z=10831..26883
on x=-18711..-1344,y=-66328..-31191,z=59213..66700
on x=-42306..-25865,y=-84072..-59112,z=-10635..13499
on x=-78607..-56882,y=28422..42754,z=-42069..-4658
on x=70330..93330,y=3485..16587,z=-2226..11978
on x=42999..75014,y=-73237..-35809,z=-16684..5008
on x=14380..47360,y=-7768..2171,z=-75135..-69101
on x=56578..76847,y=-38598..-29395,z=-32815..-15926
on x=-12748..8540,y=15488..35223,z=54552..86464
on x=3158..16552,y=-24454..1647,z=-80496..-74467
on x=56473..73209,y=-3433..12083,z=-54506..-38799
on x=51012..77530,y=30088..48866,z=8772..24396
on x=17665..25364,y=39750..62592,z=46640..74482
on x=-28714..-6773,y=9347..31266,z=73840..78208
on x=8925..38027,y=59998..85715,z=-24137..-10466
on x=53001..66549,y=-4287..21472,z=-73164..-44211
on x=-54149..-45881,y=-28052..-10796,z=49458..62599
on x=-21963..6658,y=66148..91598,z=-21298..6962
on x=-11782..161,y=-91012..-60861,z=21515..30530
on x=-76804..-69730,y=13746..32487,z=-23642..-13658
on x=963..15271,y=-71045..-39328,z=-71381..-47851
on x=67896..74688,y=-32914..-22017,z=12647..33400
on x=12634..39919,y=-66590..-50495,z=50004..68679
on x=8442..27410,y=9042..25167,z=68135..95195
on x=-25776..-14278,y=-29280..-10089,z=69100..90070
on x=64423..65336,y=3388..18317,z=-63336..-39916
on x=35919..66955,y=14633..25424,z=55317..61335
on x=-73286..-65915,y=-44128..-28250,z=22971..40421
on x=15859..26558,y=-56047..-29767,z=42976..66388
on x=-6917..5955,y=65260..87252,z=-24846..4318
on x=-65972..-29473,y=32115..66783,z=35787..53459
on x=-67248..-44667,y=33707..64039,z=-31788..-18880
on x=45484..66247,y=38595..43467,z=4589..29393
on x=13889..18349,y=66828..91044,z=14198..32527
on x=54724..85550,y=-30589..-5424,z=35166..56213
on x=-42965..-21638,y=-60889..-33965,z=43998..65167
on x=-35930..-32769,y=-78331..-56047,z=-14735..9050
on x=-24860..-8877,y=-80975..-43953,z=44660..63576
on x=-14165..8370,y=-22173..-12379,z=57378..77093
on x=71416..77554,y=-27428..6152,z=-49013..-27924
on x=-93105..-68413,y=-35230..-13620,z=-20316..293
on x=18417..29118,y=-86664..-54984,z=-36954..-22621
on x=55918..67650,y=-41116..-22359,z=-41830..-26146
off x=46522..78165,y=-65457..-45507,z=6815..36492
off x=47885..74001,y=-31398..-10975,z=28012..47043
on x=-91510..-71175,y=-16528..11618,z=-12499..8654
off x=-84874..-61512,y=8619..38028,z=-54298..-34830
on x=-83118..-51007,y=-45381..-31920,z=-23725..3021
on x=10327..15960,y=-81856..-71010,z=-33630..-17410
on x=23031..50754,y=-44713..-23633,z=-63299..-46320
on x=-46257..-28480,y=-49238..-30841,z=47755..73142
off x=20153..42123,y=-1222..5657,z=-77756..-63549
off x=-61474..-57812,y=21117..35860,z=35705..60812
off x=-79310..-66078,y=30244..44852,z=-9818..11212
on x=71346..88869,y=-31928..-3884,z=-24256..-2336
on x=-14150..11801,y=-57201..-51842,z=40079..61755
on x=-40978..-28089,y=-42914..-23485,z=55939..67185
off x=-23863..-13907,y=-536..10465,z=-90190..-61663
on x=-5454..24659,y=-79999..-64557,z=27835..52330
on x=72970..84571,y=10891..31328,z=-6227..5365
off x=-29582..-11617,y=15192..35976,z=-79544..-70922
off x=31030..48464,y=53376..61684,z=23041..44016
off x=-10069..14806,y=35482..66322,z=-63854..-56881
on x=53051..84557,y=-15724..16551,z=42287..54722
on x=14308..26815,y=-13886..-3319,z=-79479..-57838
off x=-59590..-50887,y=24924..38221,z=-57585..-33212
on x=-63473..-31750,y=-64527..-52962,z=3041..21881
off x=-31812..-7489,y=-22611..5463,z=-94027..-61918
off x=-82927..-54190,y=-9326..-491,z=20413..40734
on x=17422..42553,y=8312..32307,z=-71649..-69485
off x=-78294..-58360,y=35412..45701,z=11726..40925
on x=-21097..-3642,y=-8835..14381,z=-99475..-68222
off x=-84402..-69432,y=-14573..10548,z=14300..22158
on x=48734..68681,y=-34310..-26273,z=-52329..-23613
off x=-35075..-15047,y=-46105..-25811,z=63394..85781
on x=-86314..-54848,y=-46805..-24845,z=3445..37489
on x=-63351..-50251,y=1886..23129,z=49969..56775
off x=9515..27274,y=50806..72697,z=-49477..-25800
on x=-35588..-13052,y=15904..42770,z=-81932..-66144
off x=-46532..-36208,y=52000..73477,z=1265..25986
off x=-87317..-63208,y=-17777..9520,z=-23859..-3922
on x=-6728..4683,y=-95124..-75630,z=-29826..-12203
on x=18636..43049,y=-80651..-64046,z=-24566..10269
on x=34590..45757,y=-60461..-41469,z=-61769..-39386
off x=69352..98422,y=4474..7247,z=-13318..-2831
off x=-8443..8104,y=-47359..-17553,z=61091..78345
on x=36926..56794,y=-55435..-27634,z=31804..55162
on x=-43184..-19362,y=-73702..-61346,z=-50469..-12904
off x=-12667..10577,y=9915..24867,z=-88319..-68574
off x=-41812..-19280,y=-50846..-20974,z=-73207..-56898
on x=25317..48114,y=45115..64793,z=-34388..-8697
on x=-7572..26486,y=45477..70127,z=50627..71118
on x=-65568..-53227,y=46928..71092,z=-13475..-291
off x=-79123..-55369,y=-24021..-16788,z=17459..45068
on x=26906..52136,y=17854..35117,z=-79712..-60188
off x=73240..87023,y=-12287..13613,z=10048..23682
off x=-2039..1086,y=20152..33227,z=72963..88128
off x=-16444..-292,y=35253..51124,z=49972..79032
off x=-31142..73,y=52811..81568,z=35320..52590
off x=-45280..-38826,y=32419..56574,z=34445..72131
off x=-76025..-59111,y=14..8694,z=-60123..-35050
off x=-15911..3734,y=-23253..-9645,z=-88648..-77200
off x=47880..75437,y=28421..53226,z=33581..53756
off x=16705..55995,y=-22513..-5765,z=-79544..-52799
off x=43351..73267,y=-30268..-23880,z=-69607..-39884
on x=-49229..-24544,y=40180..60627,z=49877..77312
on x=-67421..-46871,y=30102..60244,z=16607..38923
on x=52711..81149,y=-10627..-8351,z=-48619..-22583
on x=29102..45238,y=-70802..-50027,z=16042..39140
off x=-87363..-61250,y=-1811..4566,z=-48295..-31466
off x=52765..72819,y=-38051..-27751,z=22516..53729
off x=-95898..-76590,y=-4285..-1522,z=-7096..27186
off x=39546..65197,y=59713..74511,z=-14000..15914
on x=27920..58905,y=-11677..424,z=-79346..-62053
on x=-28804..-11775,y=72541..86610,z=-12598..7458
on x=11013..13476,y=62070..76490,z=-39383..-17081
off x=39439..48894,y=-72648..-38252,z=-48344..-12634
on x=-70559..-52651,y=27489..46016,z=9134..42011
on x=-20036..2910,y=-22525..-6028,z=-87710..-68411
on x=-57816..-32833,y=51432..68426,z=-20978..2045
off x=-79019..-59736,y=15304..30243,z=-45210..-32477
off x=-90119..-75160,y=-24706..8541,z=-26255..-2577
off x=-6777..14477,y=-71771..-33912,z=46706..60930
off x=61137..75324,y=31584..49892,z=-26327..-14848
on x=1100..22606,y=28697..54124,z=-81504..-52509
off x=-71074..-58799,y=27675..44853,z=-58966..-20807
off x=-71600..-42287,y=8542..25589,z=56065..61233
on x=-45251..-35045,y=38191..54768,z=44757..67534
off x=54620..79108,y=-56141..-42099,z=12070..29484
on x=-13747..5593,y=-70633..-59152,z=35086..48154
off x=-6784..14050,y=-65722..-55419,z=44065..49503
off x=-2302..33895,y=7082..27279,z=-77945..-63272
off x=-19701..-7304,y=-76198..-72082,z=28958..48047
on x=12815..40193,y=37433..74447,z=-56868..-48723
on x=16409..41021,y=-5541..8042,z=-75386..-63309
off x=17361..35839,y=6486..33733,z=71504..85138
on x=9825..32255,y=-68518..-48749,z=-80039..-57545
off x=-62298..-50656,y=-1852..19142,z=44967..61407
off x=-65488..-40004,y=-59621..-37464,z=-48950..-23715
off x=-21676..-990,y=-79126..-51600,z=-55573..-22384
on x=16547..46077,y=59890..75234,z=-23757..-11399
on x=-87743..-69823,y=-36681..-16284,z=-11987..20192
on x=-41729..-18632,y=-35288..-5409,z=-69092..-63322
on x=40240..53567,y=51071..76567,z=-32328..-14382
off x=-62904..-45149,y=32738..63897,z=-19593..-15038
on x=21216..24135,y=-75210..-50944,z=-44984..-16166
on x=-69562..-35055,y=58744..70823,z=-15501..-9281
off x=-68150..-52172,y=26975..39630,z=-53507..-17045
on x=51169..78842,y=11583..29490,z=29644..52457
on x=-15373..4253,y=-97690..-61806,z=1940..31386
on x=73779..78637,y=5371..23397,z=-27149..-10358
on x=19250..44620,y=-7232..11656,z=-82542..-70642
on x=39149..61967,y=-16335..12372,z=52676..65415
off x=-23056..-4050,y=-38844..-16159,z=64555..74496
off x=45322..60654,y=-64033..-45875,z=-6345..6738
off x=54100..90830,y=-34444..-21517,z=5118..19759
off x=-231..8241,y=-54245..-31025,z=62160..90247
on x=-10127..-4636,y=-83774..-49558,z=30741..56326
on x=-61249..-36895,y=58338..75723,z=-15361..7006
off x=18751..41594,y=49217..65051,z=35222..37374
off x=-19037..-9828,y=-67561..-45441,z=24792..52641
on x=40827..72676,y=-15481..-1454,z=37617..66011
off x=-64739..-34793,y=-25699..-17432,z=-67453..-38858
on x=-18058..5742,y=-23939..-5488,z=-94889..-71764
on x=35519..41049,y=-27602..-9277,z=49488..82039
on x=9331..32089,y=-17498..8315,z=-80543..-76033
off x=-34327..-11605,y=11327..29037,z=-77519..-64466
off x=-71841..-43926,y=16818..21609,z=-69739..-45874
on x=-56990..-48349,y=-68324..-43481,z=3016..7811
off x=9304..25782,y=30245..56041,z=51315..85795
off x=32607..48888,y=3690..28091,z=52295..84881
off x=-2404..31293,y=58151..81212,z=-31998..-25149
on x=-63323..-36977,y=54027..77083,z=-26641..8719
off x=60302..80800,y=35514..54303,z=12123..40785
off x=47486..64483,y=7507..25137,z=-73417..-59938
off x=2177..22958,y=73828..91348,z=20624..37806
off x=-68778..-38496,y=-15625..6286,z=50357..61804
on x=42481..65067,y=414..25425,z=52357..76953
on x=21032..42183,y=-46797..-14635,z=-74998..-57422
on x=-57819..-39759,y=37245..65409,z=-31057..-9476
on x=29358..58776,y=8103..20134,z=-73101..-67091
off x=-26017..-15663,y=-53499..-40680,z=-81777..-63642
off x=-75822..-54570,y=-54832..-37673,z=-36225..-20978
on x=-30274..-12377,y=62717..73698,z=-51083..-18953
on x=25463..45515,y=-79814..-55158,z=15540..44754
off x=-58176..-42144,y=-44091..-18834,z=49801..63998
off x=40488..54727,y=-80750..-51881,z=-8037..3647
on x=66133..82929,y=-13321..9855,z=-45192..-21257
off x=3029..23415,y=-78534..-62743,z=-50610..-16058
on x=63368..74974,y=104..20701,z=37152..63545
on x=40949..74694,y=-62178..-38585,z=2610..19835
on x=-472..15977,y=64084..84976,z=-24559..2476
off x=49194..76848,y=-47174..-24042,z=-33508..-8663
off x=67399..78692,y=-28620..-16523,z=-19377..4508
off x=56630..74700,y=-14438..20792,z=36581..74432
on x=21015..41464,y=-9104..15645,z=-88506..-73629
off x=-43920..-26879,y=14492..28620,z=59594..70987
on x=-35951..-21806,y=7254..24763,z=54493..72173
on x=3336..22348,y=-15737..-3592,z=60764..78216
off x=56117..66807,y=20737..31533,z=-44715..-23712
off x=13306..42710,y=68250..78199,z=-32910..-17468
off x=-87878..-53833,y=18415..53070,z=-16577..2999
on x=33630..50999,y=-67779..-48804,z=30503..55076
off x=57002..86011,y=-10279..10607,z=19904..42650
off x=18927..33193,y=-16636..-1801,z=-88904..-63937
off x=21167..45110,y=-67846..-46840,z=-67041..-43159
off x=15448..29805,y=-69142..-47763,z=-56592..-41006
on x=-31981..-15095,y=-20248..3871,z=63022..87403
off x=-50574..-43927,y=-45935..-22203,z=-54205..-39534
off x=23509..30725,y=-59187..-39824,z=-55008..-29804
on x=-6893..10692,y=-4433..17662,z=-79958..-63043
off x=52839..82867,y=-54135..-43532,z=5074..22443
off x=-19801..2279,y=-45316..-18060,z=-73077..-53175
off x=-34124..-23489,y=-77318..-57778,z=-10644..21309
on x=-62168..-39417,y=46644..71553,z=-12633..-2688
on x=-50071..-22810,y=-45081..-19082,z=-71537..-54243
on x=-3347..16680,y=71919..95169,z=-26699..-14243
off x=-57262..-44366,y=32309..66254,z=-33394..-21315
off x=71646..96656,y=-8202..21856,z=-6535..-796
on x=-27944..-5589,y=47496..68996,z=51465..59779
on x=8322..20036,y=-71459..-51821,z=28654..53172
off x=-1130..11235,y=-72111..-54015,z=38692..55881
on x=-69102..-39230,y=21462..55716,z=-47827..-41441
on x=51340..80906,y=-52377..-19741,z=-34853..-23124
off x=-37865..-22986,y=-62050..-50913,z=-69238..-43924
on x=28721..62342,y=-8593..21244,z=52533..83625
on x=52746..80495,y=22003..40732,z=-40093..-20844
off x=35892..47834,y=-67866..-46589,z=34299..53539
off x=18277..20309,y=-15322..3925,z=-94574..-68257
off x=23509..40977,y=-81126..-48154,z=-21..33400
on x=13627..46617,y=-2537..25688,z=-74686..-56092
off x=4445..25422,y=1276..13484,z=-78843..-67185
off x=-12411..9686,y=-80652..-50914,z=-61600..-42810
on x=-18060..-1885,y=16610..43651,z=67414..76707
on x=30774..44990,y=-6499..11812,z=-87416..-59474
off x=-59552..-40954,y=48890..63902,z=-39700..-21599
off x=12390..31538,y=65013..80718,z=-15195..9149
off x=-82717..-58814,y=9792..34867,z=31729..41188
off x=-62226..-36572,y=-17931..-2307,z=64240..74646)";

