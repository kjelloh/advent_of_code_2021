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
#include <set>

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
  struct Pos {
    int row,col;
    auto operator<=>(const Pos&) const = default;
  };
  enum class Type {
    unknown
    ,A = 'A'
    ,B = 'B'
    ,C = 'C'
    ,D = 'D'
  };
  struct Pod {
    Type type;
    Pos pos;
    auto operator<=>(const Pod&) const = default;
  };  
  struct Hallway {
    int const row{1},left_col{1},right_col{1};
  };
  struct Room {
    int col;
    int entry_row,bottom_row;
  };
  class Burrow {
    public:
    private:
    std::array<Room,4> m_rooms;
  };
  struct State {
    std::set<Pod> pods;
    auto operator<=>(const State&) const = default;
  };

  void dummy() {
    std::set<State> ps{};

  }

  // bool operator<(State const& s1,State const& s2) {
  //   std::cout << "\noperator< State NOT IMPLEMENTED"; 
  //   return s1.pods<s2.pods;
  // }
  // bool operator==(State const& s1,State const& s2) {
  //   std::cout << "\noperator== State NOT IMPLEMENTED"; 
  //   return s1.pods==s2.pods;
  // }
  std::vector<State> possible_moves(State const& state) {
    std::vector<State> result{};
    std::cout << "\npossible_moves NOT IMPLEMENTED";
    // for (auto const& pod : state.pods) {

    // }
    return result;
  }
  std::vector<State> apply_strategy(State const& state,std::vector<State> candidate_moves) {
    std::vector<State> result{candidate_moves};
    std::cout << "\napply_strategy NOP"; 
    return result;
  }
  using Memoized = std::map<State,std::optional<Cost>>;
  // Recursive "find the lowest cost to re-arrange pods to reach end state"
  std::optional<Cost> best(State const& visitee,State const& end,Memoized& memoized) {
    std::optional<Cost> result;
    if (memoized.find(visitee) != memoized.end()) return memoized[visitee];
    if (visitee==end) return 0;
    /*
    Amphipods will never stop on the space immediately outside any room. 
    They can move into that space so long as they immediately continue moving. 
    (Specifically, this refers to the four open spaces in the hallway that are directly above an amphipod starting position.)
    
    Amphipods will never move from the hallway into a room
      unless that room is their destination room 
      and that room contains no amphipods which do not also have that room as their own destination. 
      
    If an amphipod's starting room is not its destination room, it can stay in that room until it leaves the room.
     (For example, an Amber amphipod will not move from the hallway into the right three rooms, and will only move into the leftmost room if that room is empty or if it only contains other Amber amphipods.)

    Once an amphipod stops moving in the hallway, it will stay in that spot until it can move into a room.
     (That is, once any amphipod starts moving, any other amphipods currently in the hallway are locked in place
      and will not move again until they can move fully into a room.)
    */
    auto frontier = possible_moves(visitee);
    frontier = apply_strategy(visitee,frontier);
    std::set<std::optional<Cost>> costs;
    for (auto const& next : frontier) {
      costs.insert(best(next,end,memoized));
    }
    auto min_iter = std::min_element(costs.begin(),costs.end());
    if (min_iter != costs.end()) result = *min_iter;
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
    State init_state{},end_state{};
    Memoized memoized{};
    auto best_cost = best(init_state,end_state,memoized);
    if (best_cost) result = best_cost.value();
    else std::cout << "\nFAILED - no best cost found";
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
