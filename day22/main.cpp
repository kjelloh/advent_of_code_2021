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

using Model = std::vector<std::string>;

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
        result.push_back(line);
    }
    return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      {
          const char* pSnippet = R"(on x=10..12,y=10..12,z=10..12
on x=11..13,y=11..13,z=11..13
off x=9..11,y=9..11,z=9..11
on x=10..10,y=10..10,z=10..10)";
          std::stringstream in{pSnippet};
          std::string line{};
          while (std::getline(in,line)) {
              auto [left,right] = split(line," ");
              // std::cout << "left \"" << left << "\" right \"" << right << "\"";
              bool on = left=="on";
              auto [xs,right_x] = split(right,",");
              auto [ys,right_y] = split(right_x,",");
              auto [zs,right_z] = split(right_y,",");
              // std::cout << "\nx:" << xs << " y:" << ys << " z:" << zs;
              auto [xs_left,xs_right] = split(xs,"=");
              auto [ys_left,ys_right] = split(ys,"=");
              auto [zs_left,zs_right] = split(zs,"=");
              std::cout << "x:" << xs_right << " y:" << ys_right << " z:" << zs_right;
          }
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
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"()";
char const* pData = R"()";
