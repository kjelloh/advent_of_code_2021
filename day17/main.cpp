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
    auto [x1,x2] = split(head_range,"..");
    auto [y1, y2] = split(tail_range, "..");
    Position ul{ {std::stoi(x1),std::stoi(y1)} };
    Position dr{ {std::stoi(x2),std::stoi(y2)} };
    return {ul,dr};
}

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto target_area = parse(in);
      std::cout << "\ntarget area:"
        << " {x:" << target_area.ul_corner.col() << ",y:" << target_area.ul_corner.row() << "}"
        << " {x:" << target_area.dr_corner.col() << ",y:" << target_area.dr_corner.row() << "}";
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

char const* pTest = R"(target area: x=20..30, y=-10..-5)";
char const* pData = R"(target area: x=137..171, y=-98..-73)";
