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
    auto operator<=>(Pod const&) const = default;
  };  
  using Alcove = std::stack<std::optional<Pod>>; // allow for no pod in botom of alcove
  using BetweenRooms = std::array<std::optional<Pod>,3>; // 0:between A,B, 1:between B,C 2: between C,D
  struct Hallway {
    Alcove left_alcove{},right_alcove{};
    BetweenRooms between_rooms{};
    auto operator<=>(Hallway const&) const = default;
  };
  class Room {
    public:
    Room(SpaceID id,std::vector<SpaceID> occupants) : room_id{id} {
      for (auto const& id : occupants) {
        this->push(Pod{id});
      }
    }
    SpaceID id() const {return this->room_id;}
    bool accept(Pod const& pod) const {
      return (wrong_occupants_count==0 and pod.home==this->room_id);
    }
    int size() {return static_cast<int>(pods.size());}
    bool has_wrong_occupants() const {return wrong_occupants_count>0;}
    Pod const& top() const {
      return pods.top();
    }
    Room& push(Pod const& pod) {
      pods.push(pod);
      if (this->room_id!=pod.home) wrong_occupants_count++;
      return*this;
    } 
    Pod pop() {
      Pod pod = pods.top();
      pods.pop();
      if (this->room_id!=pod.home) wrong_occupants_count--;
      return pod;
    } 
    int occupant_count() const {return static_cast<int>(pods.size());}
    auto operator<=>(Room const&) const = default;
    private:
    SpaceID room_id{};
    int wrong_occupants_count{0};
    std::stack<Pod> pods;
  };
  using Space = std::variant<Hallway,Room>;
  struct Pos {
    Space const& space;
    int coord;
    auto operator<=>(Pos const&) const = default;
  };
  struct Move {
    Pos from,to;
    auto operator<=>(Move const&) const = default;
  };
  struct State {
    State(std::vector<std::string> tokens={}) {
      this->spaces[SpaceID::Hallway] = Hallway{};
      if (tokens.size()>0) {
        std::vector<Room> rooms{{SpaceID::Room_A,{}},{SpaceID::Room_B,{}},{SpaceID::Room_C,{}},{SpaceID::Room_D,{}}};

        for (int tix=tokens.size()-1;tix>=0;tix--) {
          auto token = tokens[tix]; // reverse iteration to push pods into rooms in correct LIFO order
          std::vector<char> ids{};
          std::copy_if(token.begin(),token.end(),std::back_inserter(ids),[](char ch){
            return (ch>='A' and ch<='D');
          });
          if (ids.size()!=0 and ids.size()!=4) std::cout << "\nERROR - Four room occupants not found";
          for (int i=0;i<ids.size();i++) rooms[i].push(Pod{static_cast<SpaceID>(ids[i])});
        }
        for (auto const& room : rooms) {
          if (room.occupant_count()!=4) std::cout << "\nERROR - Room does not have four occupants";
          this->spaces[room.id()] = room;
        }
      }
      else {
        for (auto id : {SpaceID::Room_A,SpaceID::Room_B,SpaceID::Room_C,SpaceID::Room_D}) {
          spaces[id] = Room{id,{id,id,id,id}};
        }
      }
    }
    std::map<SpaceID,Space> spaces;
    auto operator<=>(const State&) const = default;
    bool operator==(State const&) const = default;
    bool free_path(Move const& move) const {
      return false;
    }

    State moved_to(Move const& move) const {
      std::cout << "\nmoved_to";
      State result{*this};
      if (!this->free_path(move)) std::cout << "\nERROR - can't make a blocked move";
      // We shall remove the pod from move.from.space and put it at move.to.space
      if (std::holds_alternative<Hallway>(move.from.space)) {
        Hallway from = std::get<Hallway>(move.from.space);        
        if (std::holds_alternative<Room>(move.to.space)) {
          std::cout << "\nhallway to room - NOT YET IMPLEMENTED";
          Room to = std::get<Room>(move.to.space);
        }
        else if (std::holds_alternative<Hallway>(move.to.space)) {
          std::cout << "\nERROR - move from hallway to hallway ??";
        }
      }
      else if (std::holds_alternative<Room>(move.from.space)) {
        Room from = std::get<Room>(move.from.space);
        Pod pod = from.top();
        from.pop();
        if (std::holds_alternative<Room>(move.to.space)) {
          Room to = std::get<Room>(move.to.space);
          std::cout << "\nmove room to room NOT YET IMPLEMENTED";
        }
        else if (std::holds_alternative<Hallway>(move.to.space)) {
          Hallway to = std::get<Hallway>(move.from.space);
          std::cout << "\nmove room to hallway";
          switch (move.to.coord) {
            case 0: 
              if (to.left_alcove.size()!=0) std::cout << "\nERROR - move to left alcove 0. Is blocked/occupied";
              if (to.left_alcove.size()==0) to.left_alcove.push(pod); 
              break;
            case 1:
              if (to.left_alcove.size()>1) std::cout << "\nERROR - move to left alcove 1. Is occupied";
              if (to.left_alcove.size()==0) to.left_alcove.push(std::nullopt);
              to.left_alcove.push(pod); 
              break;
            case 3: 
              if (to.between_rooms[0]) std::cout << "\nERROR - move to between rooms AB. Is occupied";
              if (!to.between_rooms[0]) to.between_rooms[0] = pod;
              break;
            case 5: 
              if (to.between_rooms[1]) std::cout << "\nERROR - move to between rooms BC. Is occupied";
              if (!to.between_rooms[1]) to.between_rooms[1] = pod;
              break;
            case 7: 
              if (to.between_rooms[2]) std::cout << "\nERROR - move to between rooms CD. Is occupied";
              if (!to.between_rooms[2]) to.between_rooms[2] = pod;
              break;
            case 9: 
              if (to.right_alcove.size()>1) std::cout << "\nERROR - move to right alcove 1. Is occupied";
              if (to.right_alcove.size()==0) to.right_alcove.push(std::nullopt);
              to.right_alcove.push(pod); 
              break;
            case 10: 
              if (to.right_alcove.size()!=0) std::cout << "\nERROR - move to right alcove 0. Is blocked/occupied";
              if (to.right_alcove.size()==0) to.right_alcove.push(pod); 
              break;
            default: std::cout << "\nERROR - move.to.coord " << move.to.coord;
          }
        }
      }
      else std::cout << "\nERROR - move from space??";
      return result;
    }
  };

  std::ostream& operator<<(std::ostream& os,State const& state) {
    std::string hallway_s{"           "};
    std::vector<std::string> rooms_s(5,"           ");
    for (auto const& [id,space] : state.spaces) {
      switch (id) {
        case SpaceID::Hallway: {
          Hallway hallway = std::get<Hallway>(space);
          if (hallway.left_alcove.size()==0) {
            hallway_s[0]='.';
            hallway_s[1]='.';
          }
          else if (hallway.left_alcove.size()==1) {
            if (hallway.left_alcove.top()) hallway_s[0] = static_cast<char>(hallway.left_alcove.top().value().home);
            hallway_s[1] = '.';
          }
          else if (hallway.left_alcove.size()==2) {
            if (hallway.left_alcove.top()) hallway_s[1] = static_cast<char>(hallway.left_alcove.top().value().home);
            hallway_s[0] = '?';
          }
          if (hallway.between_rooms[0]) hallway_s[3] = static_cast<char>(hallway.between_rooms[0].value().home);
          else hallway_s[3] = '.';
          if (hallway.between_rooms[1]) hallway_s[5] = static_cast<char>(hallway.between_rooms[0].value().home);
          else hallway_s[5] = '.';
          if (hallway.between_rooms[2]) hallway_s[7] = static_cast<char>(hallway.between_rooms[0].value().home);
          else hallway_s[7] = '.';
          if (hallway.right_alcove.size()==0) {
            hallway_s[9]='.';
            hallway_s[10]='.';
          }
          else if (hallway.right_alcove.size()==1) {
            if (hallway.left_alcove.top()) hallway_s[10] = static_cast<char>(hallway.left_alcove.top().value().home);
            hallway_s[9] = '.';
          }
          else if (hallway.right_alcove.size()==2) {
            if (hallway.left_alcove.top()) hallway_s[9] = static_cast<char>(hallway.left_alcove.top().value().home);
            hallway_s[10] = '?';
          }
        }
        break;
        case SpaceID::Room_A: {
          Room room = std::get<Room>(space);
          for (int i=0;i<4;i++) {
            if (i<4-room.size()) rooms_s[i][2] = '.';
            else if (i==4-room.size()) rooms_s[i][2] = static_cast<char>(room.top().home);
            else rooms_s[i][2] = '?';
          }
          rooms_s[4][2] = (room.has_wrong_occupants())?'!':'=';
        }
        break;
        case SpaceID::Room_B: {
          Room room = std::get<Room>(space);
          for (int i=0;i<4;i++) {
            if (i<4-room.size()) rooms_s[i][4] = '.';
            else if (i==4-room.size()) rooms_s[i][4] = static_cast<char>(room.top().home);
            else rooms_s[i][4] = '?';
          }
          rooms_s[4][4] = (room.has_wrong_occupants())?'!':'=';
        }
        break;
        case SpaceID::Room_C: {
          Room room = std::get<Room>(space);
          for (int i=0;i<4;i++) {
            if (i<4-room.size()) rooms_s[i][6] = '.';
            else if (i==4-room.size()) rooms_s[i][6] = static_cast<char>(room.top().home);
            else rooms_s[i][6] = '?';
          }
          rooms_s[4][6] = (room.has_wrong_occupants())?'!':'=';
        }
        break;
        case SpaceID::Room_D: {
          Room room = std::get<Room>(space);
          for (int i=0;i<4;i++) {
            if (i<4-room.size()) rooms_s[i][8] = '.';
            else if (i==4-room.size()) rooms_s[i][8] = static_cast<char>(room.top().home);
            else rooms_s[i][8] = '?';
          }
          rooms_s[4][8] = (room.has_wrong_occupants())?'!':'=';
        }
        break;
        default: std::cout << "\nERROR - can't os << ??space?? ";
      }
    }
    os << "\n" << hallway_s;
    for (auto const& row : rooms_s) {
      os << "\n" << row;
    } 
    return os;
  }

  struct PossibleMovesFromRoom {
    Room const& room;
    State const& state;
    std::vector<Move> operator()(Room const& room) const {
      // Room to room
      std::vector<Move> result{};
      std::cout << "\nroom->room NOT IMPLEMENTED";
      std::cout << "\nresult.size() = " << result.size();
      return result;
    }
    std::vector<Move> operator()(Hallway const& hallway) const {
      std::vector<Move> result{};
      // Room to Hallway
      std::cout << "\nroom->hallway";
      if (room.has_wrong_occupants()) {
        std::cout << "\nhas wrong occupants";
        switch (room.id()) {
          case SpaceID::Room_A:
          case SpaceID::Room_B:
          case SpaceID::Room_C:
          case SpaceID::Room_D: {
            for (int i=0;i<2-hallway.left_alcove.size();i++) {
              std::cout << "\nMove to left alcove " << i;
              Pos to{hallway,i};
              result.push_back(Move{Pos{room},to});
            }
            for (int i=0;i<2-hallway.right_alcove.size();i++) {
              std::cout << "\nMove to right alcove " << i;
              Pos to{hallway,11-i};
              result.push_back(Move{Pos{room},to});
            }
            for (int i=0;i<hallway.between_rooms.size();i++) {
              auto pod = hallway.between_rooms[i];
              if (!pod) {
                std::cout << "\nMove to between rooms " << i;
                Pos to{hallway,3+2*i};
                result.push_back(Move{Pos{room},to});
              }
            }
          } break;
          default:
            std::cout << "\nRoom ID??";
            break;
        }
      }
      std::cout << "\nresult.size() = " << result.size();
      return result;
    }
  };

  struct PossibleMovesFromHallway {
    Hallway const& hallway;
    State const& state;
    std::vector<Move> operator()(Room const& room) const {
      std::vector<Move> result{};
      // Hallway to room
      std::cout << "\nhallway->room";
      for (auto const& [id,space] : state.spaces) {
        if (std::holds_alternative<Room>(space)) {
          auto const& room = std::get<Room>(space);
          // Hallway to room?
          if (hallway.left_alcove.size()>0) {
            if (!hallway.left_alcove.top()) std::cout << "\nERROR - top Pod in left alcove is NULL";
            if (room.accept(hallway.left_alcove.top().value())) {
              Pos from{hallway,static_cast<int>(hallway.left_alcove.size()-1)}; // hallway coord 0,1,2,...10
              Pos to{room}; // room coord given by pod stack size
              result.push_back(Move{from,to});
            }
          }
          if (hallway.right_alcove.size()>0) {
            if (!hallway.right_alcove.top()) std::cout << "\nERROR - top Pod in right alcove is NULL";
            if (room.accept(hallway.left_alcove.top().value())) {
              Pos from{hallway,static_cast<int>(11-hallway.left_alcove.size())}; // hallway coord 0,1,2,...10
              Pos to{room}; // room coord given by pod stack size
              result.push_back(Move{from,to});
            }
          }
          for (int i=0;i<hallway.between_rooms.size();i++) {            
            if (hallway.between_rooms[i]) {
              Pod pod = hallway.between_rooms[i].value();
              if (room.accept(pod)) {
                Pos from{hallway,3+2*i}; // i=0,1,2 => hallway coord 3,5,7
                Pos to{room};
                result.push_back(Move{from,to});
              }
            }
          }
        }
      }
      std::cout << "\nresult.size() = " << result.size();
      return result;
    }
    std::vector<Move> operator()(Hallway const& hallway) const {
      // Hallway to Hallway - none
      std::cout << "\nhallway->hallway NOP";
      return {};
    }
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
    std::cout << "\n\nbest";
    std::cout << state;
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
    State init_state{tokens},end_state{};
    Memoized memoized{};
    auto best_cost = best(init_state,end_state,memoized);
    if (best_cost) result = best_cost.value();
    else std::cout << "\nFAILED - no best cost found";
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time-start_time;
    std::cout << "\nElapsed time: " << elapsed_seconds.count() << "s";      
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
