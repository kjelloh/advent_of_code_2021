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
#include <memory>
#include <variant>
#include <stack>

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
  enum class SpaceID {
    unknown
    ,Hallway
    ,Room_A         // A stack
    ,Room_B         // a stack
    ,Room_C         // a stack
    ,Room_D         // a stack
    ,undefined
  };
  struct Pod {
    SpaceID home;
    Cost spent;
    auto operator<=>(Pod const&) const = default;
  };  
  using Alcove = std::stack<Pod>;
  using BetweenRooms = std::array<Pod,3>; // 0:between A,B, 1:between B,C 2: between C,D
  struct Hallway {
    Alcove left_alcove{},right_alcove{};
    BetweenRooms between_rooms{};
    auto operator<=>(Hallway const&) const = default;
  };
  class Room {
    public:
    Pod const& top() const {
      return pods.top();
    }
    Room& push(Pod const& pod) {
      pods.push(pod);
      if (this->space_id!=pod.home) wrong_occupants_count++;
      return*this;
    } 
    Pod pop() {
      Pod pod = pods.top();
      pods.pop();
      if (this->space_id!=pod.home) wrong_occupants_count--;
      return pod;
    } 
    auto operator<=>(Room const&) const = default;
    private:
    SpaceID space_id;
    int wrong_occupants_count{0};
    std::stack<Pod> pods;
  };
  using Space = std::variant<Hallway,Room>;
  struct Pos {
    Space space;
    int coord;
    auto operator<=>(Pos const&) const = default;
  };
  struct Move {
    Pos from,to;
    auto operator<=>(Move const&) const = default;
  };
  struct State {
    std::map<SpaceID,Space> spaces;
    auto operator<=>(const State&) const = default;
    bool operator==(State const&) const = default;
    State moved_to(Move const& move) const {
      return *this;
    }
  };

  struct PossibleMovesFromRoom {
    Room const& room;
    State const& state;
    std::vector<Move> operator()(Room const& room) const {
      // Room to room
      return {};}
    std::vector<Move> operator()(Hallway const& hallway) const {
      // Room to Hallway
      return {};}
  };

  struct PossibleMovesFromHallway {
    Hallway const& hallway;
    State const& state;
    std::vector<Move> operator()(Room const& room) const {
      // Hallway to room

      return {};}
    std::vector<Move> operator()(Hallway const& hallway) const {
      // Hallway to Hallway - none
      return {};}
  };

  struct PossibleMoves {
    State const& state;
    std::vector<Move> operator()(Room const& room) const {
      std::vector<Move> result{};
      for (auto const& [id,space] : state.spaces) {
        auto moves = std::visit(PossibleMovesFromRoom{room,state},space);
        std::copy(moves.begin(),moves.end(),std::back_inserter(result));
      }
      return {};
    }
    std::vector<Move> operator()(Hallway const& hallway) const {
      std::vector<Move> result{};
      for (auto const& [id,space] : state.spaces) {
        auto moves = std::visit(PossibleMovesFromHallway{hallway,state},space);
        std::copy(moves.begin(),moves.end(),std::back_inserter(result));
      }
      return result;
    }
  };

  std::vector<Move> possible_moves(State const& state) {
    std::vector<Move> result{};
    std::cout << "\npossible_moves NOT IMPLEMENTED";
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

      /*
        #############
        #...........#
        ###C#D#A#B###
          #D#C#B#A#
          #D#B#A#C#
          #B#A#D#C#
          #########

        #############
        #...........#   Hallway 1,2,x,4,x,6,x,8,x,10,11
        ### # # # ###   Room col 3,5,7,9
          # # # # #     Room row 2,3,4,5
          # # # # #
          # # # # #
          #########

      This pod can potentially go to
      
        Hallway col 1,2,x,4,x,6,x,8,x,10,11 = 7 pos
        A room A,B,C,D row 2,3,4,5 = 16 pos
        --------------------------------------------
        Total: 16+7 = 23 positions for 16 pods (leaving 7 positions free at any time)
      */
     for (auto const& [id,space] : state.spaces) {
       auto moves = std::visit(PossibleMoves{state},space);
       std::copy(moves.begin(),moves.end(),std::back_inserter(result));
     }
     return result;
  }
  std::vector<Move> apply_strategy(State const& state,std::vector<Move> candidate_moves) {
    std::vector<Move> result{candidate_moves};
    std::cout << "\napply_strategy NOP"; 
     /*
        Lets get some feel of possible and impossible moves

        #############
        #...........#
        ###C#D#A#B###
          #D#C#B#A#
          #D#B#A#C#
          #B#A#D#C#
          #########

          *) The pods at the bottom of each room matters! We see they are ALL in the wrong room.
          *) At each move only 1) the top pod in the wrong room can move and 2) a pod in the hallway that can go home
          *) Pods can't move a second time in the hallway.

        So initially pod C,D,A and B can move out in the hallway.
        Is there somewhere C can NOT move?

        #C..........# ok
        ### #D#A#B###

        #.C.........# ok
        ### #D#A#B###

        #xx.......xx# Always OK to pop into an alcove if there is room (will never block other pods)
        ### #D#A#B###

        #...C.......# NOT ok! (We have to empty 4 occupnads in room C into only 3 non-blocking spots)
        ### #D#A#B### General rule: If there are more wrong occupants in home room than unblocking spots to move to -> INVALID
          #D#C#B#A#
          #D#B#A#C#
          #B#A#D#C#

        #...C.......# NOT ok! (We have to empty 4 occupnads in room C into only 3 non-blocking spots)
        ### #D#A#B###
          #D#C#B#A#
          #D#B#A#C#
          #B#A#D#C#

        AHA - There is only three spots between rooms.
        *) Between A,B is INVALID if the count of wrong occupants in home > number of non-blocking spots for occupants to move to.

        #.....C.....# NOT ok! (We have to empty 4 occupnads in room C into only 3 non-blocking spots)
        ### #D#A#B###
          #D#C#B#A#
          #D#B#A#C#
          #B#A#D#C#

        #.......C...# Must empty A,B,A,D into 4 empty spots (potentially OK)
        ### #D#A#B###
          #D#C#B#A#
          #D#B#A#C#
          #B#A#D#C#

      */
    return result;
  }
  using Memoized = std::map<State,std::optional<Cost>>;
  
  // Recursive "find the lowest cost to re-arrange pods to reach end state"
  std::optional<Cost> best(State const& state,State const& end,Memoized& memoized) {
    std::optional<Cost> result;
    if (memoized.find(state) != memoized.end()) return memoized[state];
    if (state==end) return 0;
    auto move_candidates = possible_moves(state);
    auto moves = apply_strategy(state,move_candidates);
    std::set<std::optional<Cost>> costs;
    for (auto const& move : moves) {
      costs.insert(best(state.moved_to(move),end,memoized));
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
