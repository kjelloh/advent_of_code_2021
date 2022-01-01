#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <optional>

char const* pTest = R"(#############
#...........#
###B#C#B#D###
  #D#C#B#A#
  #D#B#A#C#
  #A#D#C#A#
  #########";
char const* pData = R"(#############
#...........#
###C#D#A#B###
  #D#C#B#A#
  #D#B#A#C#
  #B#A#D#C#
  #########)";

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using Model = std::vector<std::string>;

Model parse(auto& in) {
    Model result{};
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
char const* pTest = R"(#############
#...........#
###B#C#B#D###
  #D#C#B#A#
  #D#B#A#C#
  #A#D#C#A#
  #########";
char const* pData = R"(#############
#...........#
###C#D#A#B###
  #D#C#B#A#
  #D#B#A#C#
  #B#A#D#C#
  #########)";
char const* pEnd = R"(#############
#...........#
###A#B#C#D###
  #A#B#C#D#
  #A#B#C#D#
  #A#B#C#D#
  #########)";

  using Cost = size_t;
  using Map = std::vector<std::string>;
  struct Pos {
    int row,col;
  };
  struct Move {
    Pos from,to;
  };
  using Step = std::vector<Move>;
  using State = Map;
  State operator+(State const& state,Step const& step) {
    return State{};
  }
  /*
  #...........#
  ###x#x#x#x###
  0123456789012
  */
  auto const ROOM_COLUMNS = {3,5,7,9}; 
  /*
  0 #############
  1 #...........#
  2 ###A#B#C#D###
  3   #A#B#C#D#
  4   #A#B#C#D#
  5   #A#B#C#D#
  6   #########
  */
 auto const ROOM_ROWS = {2,3,4,5};
  /*
  0123456789012
  #xx.x.x.x.xx#
  ###A#B#C#D###
  */
  auto const HALLWAY_COLUMNS = {1,2,4,6,8,10,11};
  auto const LEFT_ALCOVE = {1,2};
  auto const RIGHT_ALCOVE = {10,11};
  auto const BETWEEN_ROOMS = {4,6,8};
  bool is_end_state(State const& state) {
    bool result{true};
    for (int col : ROOM_COLUMNS) {
      for (int row : ROOM_ROWS) {
        char ch = state[row][col];
        if (result = result and (ch=='A'+(col-3)/2);!result) break;        
      }
      if (!result) break;
    }
    return result;
  }
  std::optional<Pos> home_pos(char type,State const& state) {
    std::optional<Pos> result{};
    bool home_pos_exist{true};
    int col = 3+(type-'A')*2;
    for (auto row : ROOM_ROWS) {
      char ch=state[row][col];
      if (ch==' ') {
        result = Pos{row,col};
        continue;
      }
      if (home_pos_exist = home_pos_exist and (ch==type);!home_pos_exist) break;
    }
    return result;
  }  
  std::vector<Pos> expand_from(State const& state, Pos const& pos) {
    std::vector<Pos> result;
    if (pos.row>1) {
      // room to room
      auto home = home_pos(state[pos.row][pos.col],state);
      if (home) result.push_back(home.value());
      // room to hallway
    }
    else {
      // hallway to room
      for (auto col : LEFT_ALCOVE) {

      }
      for (auto col : RIGHT_ALCOVE) {

      }
      for (auto col : BETWEEN_ROOMS) {

      }
    }
    return result;
  }
  std::vector<Move> expand(State const& state) {
    std::vector<Move> result;
    std::vector<Pos> froms{};
    for (auto col : ROOM_COLUMNS) {
      for (auto row : ROOM_ROWS) {
        auto ch = state[row][col];
        if (ch>='A' and ch <='D') {
          Pos from{row,col};
          auto tos = expand_from(state,from);
          std::transform(tos.begin(),tos.end(),std::back_inserter(result),[&from](Pos const& to){
            return Move{to,from};
          });
          break;
        }
      }
    }
    for (auto col : HALLWAY_COLUMNS) {
        auto ch = state[1][col];
        if (ch>='A' and ch <='D') {
          froms.push_back(Pos{1,col});
          break;
        }
    }
    std::cout << "\ncandidates:";
    for (auto move : result) std::cout << state[move.from.row][move.from.col] << "{" << move.from.row << "," << move.from.col << "}"
      << "->{" << move.to.row << "," << move.to.col << "}";

    return result;
  }
  std::optional<Cost> best(State const& state) {
    std::optional<Cost> result{};
    if (is_end_state(state)) return 0;
    auto moves = expand(state);
    return result;
  }
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto init_state = parse(in);
      std::stringstream end_in{ pEnd };
      auto end_state = parse(end_in);
      auto cost = best(init_state);
      if (cost) result = cost.value();
      else std::cout << "\nFAILED - Not best cost found";
      return result;
  }
}
int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  answers.push_back({"Part 2 Test",part2::solve_for(part2::pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}
