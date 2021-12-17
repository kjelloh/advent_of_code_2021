#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <optional>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;
using Coord = int;
using CoordRange = std::pair<Coord, Coord>;
struct Area {
  CoordRange x_range, y_range;
};
using Shot = std::pair<int, int>;
using Shots = std::vector<Shot>;
struct TargetPractice {
  Area target;
  Shots shots;
};
using Model = TargetPractice;

std::pair<std::string, std::string> split(std::string const& token, std::string const& delim) {
  auto pos = token.find(delim);
  auto left = token.substr(0, pos);
  auto right = token.substr(pos+delim.size());
  return { left,right };
}
std::pair<std::string, std::string> split(std::string const& token, char delim) {
  return split(token, std::string{ delim });
}

CoordRange coord_range(std::string const& sc1, std::string const& sc2) {
  auto c1 = std::stoi(sc1);
  auto c2 = std::stoi(sc2);
  if (c1 < c2) {
    return { c1,c2 };
  }
  else {
    return { c2,c1 };
  }
}
bool in_range(int coord, CoordRange const& range) {
  return (coord >= range.first and coord <= range.second);
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
    auto x_range = coord_range(sx1,sx2);
    auto y_range = coord_range(sy1, sy2);
    TargetPractice target_practice{ {x_range,y_range},Shots{} };
    while (in >> line) {
      if (line.size() > 0) {
        auto [dx_s, dy_s] = split(line, ',');
        std::cout << "\nshot " << dx_s << " " << dy_s;
        target_practice.shots.push_back({ std::stoi(dx_s),std::stoi(dy_s) });
      }
    }
    return target_practice;
}

using MaxY = int;
using OptionalMaxY = std::optional<MaxY>;
bool may_hit_target(Area const& target,int x, int y, int dx, int dy) {
  //std::cout << "\nmay_hit_target {x:" << x << ",y:" << y << "} {dx:" << dx << ",dy:" << dy << "}";
  if (dx == 0 and not in_range(x,target.x_range)) return false;
  if (dx > 0 and x > target.x_range.second) return false;
  if (dx < 0 and x < target.x_range.first) return false;
  if (dy < 0 and y < target.y_range.first) return false;
  // Note: if dy>0 it decrements towards dy<0 so we cant know for sure it may not continue up or later come down to y_range
  return true;
}
int global_shot_count{ 0 };
OptionalMaxY max_y_if_target_hit(Area const& target,int dx,int dy) {
  std::cout << "\nmax_y_if_target_hit dx:" << dx << " dy:" << dy;
  global_shot_count++;
  MaxY result{0};
  bool found_one = false;
  int x = dx;
  int y = dy;
  while (may_hit_target(target,x, y, dx, dy)) {
    //std::cout << "\ntest {x:" << x << ",y:" << y << "}";
    result = std::max(result,y);
    if (in_range(x,target.x_range) and in_range(y,target.y_range)) {
      std::cout << " ==> in target! max_y=" << result;
      found_one = true;
      break;
    }
    if (dx > 0) dx--; // x velocity decrements to zero and stays there
    --dy; // y falls "forever"
    x += dx;
    y += dy;
  }
  return (found_one) ? OptionalMaxY{ result } : std::nullopt;
}

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto target_practice = parse(in);
      auto target_area = target_practice.target;
      std::cout << "\ntarget area:"
        << " x_range [" << target_area.x_range.first << ".." << target_area.x_range.second << "]"
        << " y_range [" << target_area.y_range.first << ".." << target_area.y_range.second << "]";
      int min_dx = 1; // brute force!
      int max_dx = 100; // brute force!
      int min_dy = 1; // brute force!
      int max_dy = 100; // brute force!
      // search the space of dx,dy
      int max_y{ 0 };
      for (int dx = min_dx; dx < max_dx; dx++) {
        for (int dy = min_dy; dy < max_dy; dy++) {
          //std::cout << "\ntest init{dx:" << dx << ",dy:" << dy << "}";
          if (auto optional_max_y = max_y_if_target_hit(target_area, dx, dy)) {
            max_y = std::max(max_y, *optional_max_y);
            std::cout << "\n\tmax_y=" << max_y;
          }
        }
      }
      result = max_y;
      return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto target_practice = parse(in);
      std::cout << "\nshot count:" << target_practice.shots.size();
      int hit_count{ 0 };
      global_shot_count = 0;
      for (auto const& shot : target_practice.shots) {
        if (auto optional_max_y = max_y_if_target_hit(target_practice.target, shot.first, shot.second)) {
          ++hit_count;
          std::cout << "\n\thit count:" << hit_count << " global shot count:" << global_shot_count;
        }
      }
      result = hit_count;
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  //answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\nPress <enter>...";
  std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(target area: x=20..30, y=-10..-5)";
char const* pData = R"(target area: x=137..171, y=-98..-73
23,-10  25,-9   27,-5   29,-6   22,-6   21,-7   9,0     27,-7   24,-5
25,-7   26,-6   25,-5   6,8     11,-2   20,-5   29,-10  6,3     28,-7
8,0     30,-6   29,-8   20,-10  6,7     6,4     6,1     14,-4   21,-6
26,-10  7,-1    7,7     8,-1    21,-9   6,2     20,-7   30,-10  14,-3
20,-8   13,-2   7,3     28,-8   29,-9   15,-3   22,-5   26,-8   25,-8
25,-6   15,-4   9,-2    15,-2   12,-2   28,-9   12,-3   24,-6   23,-7
25,-10  7,8     11,-3   26,-7   7,1     23,-9   6,0     22,-10  27,-6
8,1     22,-8   13,-4   7,6     28,-6   11,-4   12,-4   26,-9   7,4
24,-10  23,-8   30,-8   7,0     9,-1    10,-1   26,-5   22,-9   6,5
7,5     23,-6   28,-10  10,-2   11,-1   20,-9   14,-2   29,-7   13,-3
23,-5   24,-8   27,-9   30,-7   28,-5   21,-10  7,9     6,6     21,-5
27,-10  7,2     30,-9   21,-8   22,-7   24,-9   20,-6   6,9     29,-5
8,-2    27,-8   30,-5   24,-7)";
