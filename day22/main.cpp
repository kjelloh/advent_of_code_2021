#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <array>
#include <set>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
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
class Cuboid {
public:
  Cuboid(bool on,CoordRange<std::string> xr, CoordRange<std::string> yr, CoordRange<std::string> zr)
    :  m_on{on}
      ,m_xr{ std::stoi(xr.first),std::stoi(xr.second)}
      ,m_yr{std::stoi(yr.first),std::stoi(yr.second)}
      ,m_zr{std::stoi(zr.first),std::stoi(zr.second)} {}
  std::string to_string() const {
    std::ostringstream os{};
    os << "<cuboid>";
    os << " {" << std::to_string(m_xr.first) << "," << std::to_string(m_yr.first) << "," << std::to_string(m_zr.first) << "}";
    os << " .. ";
    os << " {" << std::to_string(m_xr.second) << "," << std::to_string(m_yr.second) << "," << std::to_string(m_zr.second) << "}";
    if (m_on) os << " ON";
    else os << "off";
    return os.str();
  }
  void for_each(auto f) const {
    for (Coord x = m_xr.first; x <= m_xr.second; x++) {
      for (Coord y = m_yr.first; y <= m_yr.second; y++) {
        for (Coord z = m_zr.first; z <= m_xr.second; z++) {
          f(Vector{ x,y,z },m_on);
        }
      }
    }
  }
private:
  bool m_on;
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
    while (in >> line) {
    }
    return result;
}

class Reactor {
public:
  void set_cube(Vector const& v,bool on) {
    if (on) {
      m_cubes.insert(v);
    }
    else {
      m_cubes.erase(v);
    }
  }
  void set_cubes(Cuboid const& cuboid) {
    cuboid.for_each([this](Vector const& v,bool on) {
      this->set_cube(v, on);
      });
  }
  Result cubes_on_count() const { return m_cubes.size(); }
private:
  std::set<Vector> m_cubes;
};

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      {
        Model puzzle_model{};
        const char* pSnippet = R"(on x=10..12,y=10..12,z=10..12
on x=11..13,y=11..13,z=11..13
off x=9..11,y=9..11,z=9..11
on x=10..10,y=10..10,z=10..10)";
        const char* pSnippet2 = R"(on x=-20..26,y=-36..17,z=-47..7
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
on x=-41..9,y=-7..43,z=-33..15)";
//on x=-54112..-39298,y=-85059..-49293,z=-27449..7877
//on x=967..23432,y=45373..81175,z=27513..53682)";
        std::stringstream in{ pSnippet2 };
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
          puzzle_model.push_back(cuboid);
        }
        std::cout << "\ncuboid count " << puzzle_model.size();
        for (auto const& cuboid : puzzle_model) {
          std::cout << "\n" << cuboid.to_string();
        }
        Reactor reactor{};
        for (auto const& cuboid : puzzle_model) {
          reactor.set_cubes(cuboid);
        }
        result = reactor.cubes_on_count();

      }
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\nPress <enter>...";
  std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"()";
char const* pData = R"()";
