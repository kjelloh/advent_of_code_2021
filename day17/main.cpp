#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;
using Coord = int;
struct Position {
  std::pair<Coord, Coord> coord;
  Coord row() { return coord.second; }
  Coord col() { return coord.first; }
};
struct Area {
  Position ul_corner, dr_corner;
};
using Model = Area;

std::pair<std::string, std::string> split(std::string const& token, std::string const& delim) {
  auto pos = token.find(delim);
  auto left = token.substr(0, pos);
  auto right = token.substr(pos+delim.size());
  return { left,right };
}
std::pair<std::string, std::string> split(std::string const& token, char delim) {
  return split(token, std::string{ delim });
}

std::pair<Coord, Coord> coord_range(auto sc1, auto sc2) {
  auto c1 = std::stoi(sc1);
  auto c2 = std::stoi(sc2);
  if (std::abs(c1) < std::abs(c2)) {
    return { c1,c2 };
  }
  else {
    return { c2,c1 };
  }
}

Model parse(auto& in) {
    Model result{};
    std::string line{};
    std::getline(in,line);
    // target area: x=20..30, y=-10..-5
    auto [left,right] = split(line,',');
    std::cout << "\nleft=\"" << left << "\" right=\"" << right << "\"";
    auto [head,head_range] = split(left,'=');
    auto [tail,tail_range] = split(right,'=');
    std::cout << "\nhead:" << head_range << " tail:" << tail_range;
    auto [sx1,sx2] = split(head_range,"..");
    auto [sy1,sy2] = split(tail_range, "..");
    auto [x1,x2] = coord_range(sx1,sx2);
    auto [y1, y2] = coord_range(sy1, sy2);
    Position ul{ {x1,y1} };
    Position dr{ {x2,y2} };
    return {ul,dr};
}

int x(auto v0, auto n) {
  if (n > v0) return x(v0,v0);
  else return (n * v0 + n * (1 - n) / 2);
}

int y(auto v0,auto n) {
  return (n*v0+n*(1-n)/2);
}

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto target_area = parse(in);
      std::cout << "\ntarget area:"
        << " {x:" << target_area.ul_corner.col() << ",y:" << target_area.ul_corner.row() << "}"
        << " {x:" << target_area.dr_corner.col() << ",y:" << target_area.dr_corner.row() << "}";
      // x,y movement are independent
      // We are looking for n: x(n) = Sum(vx0,vx0-1,vx0-2,...,vx0-n) is in the target area begin..end
      // and the same for y.
      // Now the y coordinat is a geometrical series y0 - (0+1+2+3+4...+n) = y0 - n*(n+1)/2
      // where y0 = the original y velocity.
      int x0 = 6;
      int y0 = 3;
      for (int n = 0; n < 10; n++) {
        std::cout << "\n\tn" << n << ": {x:" << x(x0, n) << ",y:" << y(y0, n) << "}";
      }
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
  answers.push_back({"Part 1     ",part1::solve_for(pData)});
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

char const* pTest = R"(target area: x=20..30, y=-10..-5)";
char const* pData = R"(target area: x=137..171, y=-98..-73)";
