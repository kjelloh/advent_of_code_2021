#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <map>
#include <optional>
#include <vector>
#include <chrono>
#include <ostream>

char const* pTest = R"(#############
#...........#
###B#C#B#D###
  #A#D#C#A#
  #########)";
char const* pData = R"(#############
#...........#
###C#D#A#B###
  #B#A#D#C#
  #########)";

namespace {
  std::ostream& operator<<(std::ostream& os,std::vector<std::string> const& t) {
    for (auto const& entry : t) {
      os << "\n" << entry;
    }
    return os;
  }
}

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using Tokens = std::vector<std::string>;

Tokens parse(auto& in) {
    Tokens result{};
    std::string line{};
    while (std::getline(in,line)) {
        result.push_back(line);
    }
    return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

namespace part2 {
  using Cost = size_t;
  class Pos {
    int row,col;
  };
  using Type = std::optional<char>;
  using Pods = std::map<Pos,Type>;
  struct Room {
    int col;
    int entry_row,bottom_row;
  };
  class Burrow {
    public:
    private:
    std::array<Room,4> m_rooms;
  };
  using State = Pods;
  // Recursive "find the lowest cost to re-arrange pods to reach end state"
  Cost best(State const& visitee,State const& end) {
    Cost result;
    return result;
  }
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto tokens = parse(in);
    // Part 2, insert two more rows
    for (auto const& entry : {"  #D#C#B#A#","  #D#B#A#C#"}) {
      tokens.insert(tokens.end()-2,entry);
    }
    // Log
    if (true) {
      std::cout << tokens;
    }
    auto start_time = std::chrono::steady_clock::now();
    // Solve code goes here
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time-start_time;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";      
    return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}
