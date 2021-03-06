#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <optional>
#include <map>
#include <ostream>
#include <set>
#include <compare>

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

using Cost = size_t;
using Map = std::vector<std::string>;
struct Pos {
  int row,col;
  auto operator<=>(Pos const&) const = default;
  bool operator==(Pos const&) const = default;
};
struct Move {
  Pos from,to;
};
using State = Map;
using Visited = std::map<State,std::optional<Cost>>;

std::ostream& operator<<(std::ostream& os,Move const& move) {
  os << "{" << move.from.row << "," << move.from.col 
    << "}->{" << move.to.row << "," << move.to.col << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os,State state) {
  for (auto row : state) {
    os << "\n" << row;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os,std::pair<Move,Cost> const& step) {
  os << step.first;
  os << " cost " << step.second;
  return os;
}

std::ostream& operator<<(std::ostream& os,std::pair<State,Cost> state_cost) {
  auto& [state,cost] = state_cost;
  os << state << " acc cost " << cost; 
  return os;
}

/*
#...........#
###x#x#x#x###
0123456789012
*/
auto const ROOM_COLUMNS = {3,5,7,9}; 
/*
0123456789012
#xx.x.x.x.xx#
###A#B#C#D###
*/
// auto const LEFT_ALCOVE = {1,2};
// auto const RIGHT_ALCOVE = {10,11};
auto const BETWEEN_ROOMS = {4,6,8};
bool is_end_state(State const& state) {
  bool result{true};
  for (int col : ROOM_COLUMNS) {
    for (int row=2;state[row][col]!='#';row++) {
      char ch = state[row][col];
      if (result = result and (ch=='A'+(col-3)/2);!result) break;        
    }
    if (!result) break;
  }
  return result;
}
bool is_home( Pos const& pos,State const& state) {
  bool result{true};
  if (pos.row==1) return false;
  char type = 'A'+(pos.col-3)/2;
  for (int row=2;state[row][pos.col]!='#';row++) {
    char ch=state[row][pos.col];
    if (result = result and (ch =='.' or ch==type);!result) break;
  }
  return result;
}
std::optional<Pos> home_pos(char type,State const& state) {
  std::optional<Pos> result{};
  bool home_pos_exist{true};
  int col = 3+(type-'A')*2;
  Pos pos{};
  for (int row=2;state[row][col]!='#';row++) {
    char ch=state[row][col];
    if (ch=='.') pos = {row,col};
    else if (home_pos_exist = home_pos_exist and (ch == '.' or ch==type);!home_pos_exist) break;
  }
  if (home_pos_exist) result = pos;
  return result;
}
Cost move_cost(char type,Move const& move) {
  Cost result;
  int step_cost{0};
  switch (type) {
    case 'A':step_cost=1;break;
    case 'B':step_cost=10;break;
    case 'C':step_cost=100;break;
    case 'D':step_cost=1000;break;
    default: throw std::runtime_error("move_cost ??type??");
  }
  int dr{0};
  if (move.from.row>1 and move.to.row>1) dr = (move.from.row-1) + (move.to.row-1);
  else dr = std::max(move.from.row,move.to.row)-1;
  int dc = std::abs(move.to.col-move.from.col);
  result = step_cost*(dc+dr);
  return result;
}
class MoveSelector {
public:
using value_type = Move;
  MoveSelector(std::pair<State,Cost> const& state_cost) : m_state_cost{state_cost} {}
  MoveSelector& push_back(Move const& move) {
    auto& [state,cost] = m_state_cost;
    if (!this->blocked_move(move) and !this->will_not_work(move)) {
      this->steps.push_back({move,move_cost(state[move.from.row][move.from.col],move)});
    }
    return *this;
  }
  std::vector<std::pair<Move,Cost>> selected() {    
    return this->steps;
  }
private:
  std::vector<std::pair<Move,Cost>> steps{};
  std::pair<State,Cost> const& m_state_cost;
  bool blocked_move(Move const& move) {
    bool result{false};
    // Only pods at between room positions can block (given we only try to move to/from top pod in alcoves and rooms)
    auto& [state,cost] = m_state_cost;
    for (auto col : BETWEEN_ROOMS) {
      if (result = result or (state[1][col]!='.' and std::min(move.from.col,move.to.col) < col and std::max(move.from.col,move.to.col) > col);result) break;
    }
    return result;
  }
  bool will_not_work(Move const& move) {
    bool result{false};
    // TODO: Find some way to optimise away moves that will not work!
    return result;
  }

};
std::vector<std::pair<Move,Cost>> expand_from(std::pair<State,Cost> const& state_cost, Pos const& pos) {
  MoveSelector move_selector{state_cost};
  auto& [state,cost] = state_cost;
  auto home = home_pos(state[pos.row][pos.col],state);
  // Room or Hallway to home room
  if (home) {
    move_selector.push_back({pos,home.value()}); // prefer go home
  }
  if (pos.row>1) { // from room
    // to left alcove (possible top)
    if (state[1][2]=='.') {
      if (state[1][1]=='.') {
        move_selector.push_back({pos,Pos{1,1}});
      }
      move_selector.push_back({pos,Pos{1,2}});
    }
    // to right alcove (possible top)
    if (state[1][10]=='.') {
      if (state[1][11]=='.') {
        move_selector.push_back({pos,Pos{1,11}});
      }
      move_selector.push_back({pos,Pos{1,10}});
    }
    // to between rooms
    for (auto col : BETWEEN_ROOMS) {
      char ch = state[1][col];
      if (ch=='.') {
        // std::cout << " free {1," << col << "}";
        move_selector.push_back({pos,Pos{1,col}});
      }
    }
  }
  return move_selector.selected();
}
std::vector<std::pair<Move,Cost>> expand(std::pair<State,Cost> const& state_cost) {
  std::vector<std::pair<Move,Cost>> result;
  auto& [state,cost] = state_cost;
  // From room top
  for (auto col : ROOM_COLUMNS) {
    for (int row=2;state[row][col]!='#';row++) {
      auto ch = state[row][col];
      if (ch=='.') continue;
      if (ch>='A' and ch <='D') {
        if (is_home(Pos{row,col},state)) break;
        Pos from{row,col};
        auto steps = expand_from(state_cost,from);
        std::copy(steps.begin(),steps.end(),std::back_inserter(result));
        break;
      }
    }
  }
  // from left alcove top
  if (state[1][2]!='.') {
    auto steps = expand_from(state_cost,{1,2});
    std::copy(steps.begin(),steps.end(),std::back_inserter(result));
  }
  else if (state[1][1]!='.') {
    auto steps = expand_from(state_cost,{1,1});
    std::copy(steps.begin(),steps.end(),std::back_inserter(result));
  }
  // from right alcove top
  if (state[1][10]!='.') {
    auto steps = expand_from(state_cost,{1,10});
    std::copy(steps.begin(),steps.end(),std::back_inserter(result));
  }
  else if (state[1][11]!='.') {
    auto steps = expand_from(state_cost,{1,11});
    std::copy(steps.begin(),steps.end(),std::back_inserter(result));
  }
  // from between rooms
  for (auto col : BETWEEN_ROOMS) {
    char ch = state[1][col];
    if (ch!='.') {
      auto steps = expand_from(state_cost,{1,col});
      std::copy(steps.begin(),steps.end(),std::back_inserter(result));
    }
  }
  return result;
}

std::vector<std::pair<Move,Cost>> apply_strategy(std::pair<State,Cost> const& state_cost, std::vector<std::pair<Move,Cost>> const& potential_steps) {
  // Rearrange the order of the potential steps to enhance our finding of a solution
  // Asume potential_steps are valid steps (no self to self or move out of a room where pod is already home etc.)
  std::vector<std::pair<Move,Cost>> result{potential_steps};
  auto& [state,cost] = state_cost;
  // Invent a heuristic to assign more or less value to certain steps
  // NOTE: I implemented a hunch about what good steps are from the example solution in the puzzle descruption.
  //       As long as we do not drop any moves we are at least safe, but can make solviong the possible more or less time consuming

  // note: C++ issue with capturing a structured binding in the lambda below...
  // note: Structured binding introduces names, not variables (https://stackoverflow.com/questions/54842919/k-in-capture-list-does-not-name-a-variable?noredirect=1&lq=1 )
  // note: Not exatly sure about the semantics though (name vs variable?)
  // note: clang reports error while gcc accepts
  // note: the following code works in both gcc and clang (capture capture by reference to varaiable state from structured binding name state)
  std::sort(result.begin(),result.end(),[&state=state](auto const& step1,auto const& step2){
    int heuristic1{0},heuristic2{0};
    auto& [move1,cost1] = step1;
    auto& [move2,cost2] = step2;
    char type1 = state[move1.from.row][move1.from.col];
    char type2 = state[move1.from.row][move1.from.col];
    auto home1 = home_pos(type1,state);
    auto home2 = home_pos(type2,state);
    // Prefer to a home room (any order)
    if (home1) heuristic1 += 10000;
    if (home2) heuristic2 += 10000;
    // prefer from room that is not yet a home, D room before "lesser" rooms
    if (!home1) heuristic1 += 1000*move1.from.col; 
    if (!home2) heuristic2 += 1000*move2.from.col;
    // Prefer alcoves before blocking hallway positions
    if (move1.to.row==1) heuristic1 += 100+((move1.to.col<3)?10:0)+((move1.to.col>9)?10:0); 
    if (move2.to.row==1) heuristic2 += 100+((move2.to.col<3)?10:0)+((move2.to.col>9)?10:0);
    return heuristic1>heuristic2; // prefer steps with highest heuristic
  });
  return result;
}
std::pair<State,Cost> next(std::pair<State,Cost> const& state_cost,std::pair<Move,Cost> const& step) {
  // return the next state and the accumulated cost to reach next state from our search start
  auto& [state,cost] = state_cost;
  State stepped_state{state};
  auto [move,move_cost] = step;
  char ch = stepped_state[move.from.row][move.from.col]; 
  stepped_state[move.from.row][move.from.col] = '.';
  stepped_state[move.to.row][move.to.col] = ch;
  return {stepped_state,cost + move_cost}; // acc cost = stepped state and cost to reach this state
}
/*
 Note that there is an accumuluated cost past downwards the recursion.
 While the returned cost is the found cost to reach the end state.
 I though I could use the accumulated cost to give up on som recursuon but never had to do so...
 Can be a subject for improvement on this depth first search for "shortest path"
 */
std::optional<Cost> best(int stack_level,std::pair<State,Cost> const& state_cost,Visited& visited) {
  // return the best cost to reach the end state from provided state and cost to reach this state
  // NOTE: The cost in state_cost is the accumulated cost so far (cost from 0 to this state)
  static int call_count{-1};
  ++call_count;
  std::optional<Cost> result{};
  auto& [state,cost] = state_cost;
  if (stack_level==0) std::cout << "\nInit state " << state;
  if (call_count%10000==0) std::cout << '.' << std::flush;
  if (is_end_state(state)) return 0; // zero cost from here
  if (auto iter = visited.find(state);iter != visited.end()) return iter->second;
  auto potential_steps = expand(state_cost);
  auto strategic_steps = apply_strategy(state_cost,potential_steps);
  std::vector<Cost> costs{};
  for (auto const& step : strategic_steps) {
    auto& [move,step_cost] = step;
    auto next_state_cost = next(state_cost,step); // Next state and accumulated cost to reach this state
    auto best_cost = best(stack_level+1,next_state_cost,visited);
    if (best_cost) costs.push_back(step_cost + best_cost.value()); // best cost to end state from here
    visited[next_state_cost.first] = best_cost;
  }
  if (auto iter = std::min_element(costs.begin(),costs.end());iter!=costs.end()) result=*iter;
  if (stack_level==0) {
    std::cout << "\nBest cost: ";
    if (result) std::cout << " " << result.value();
    else std::cout << "\nFAILED to find end state";
  }
  return result;
}

namespace part1 {
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

  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto init_state = parse(in);
    Visited visited{};
    auto cost = best(0,{init_state,0},visited);
    if (cost) result = cost.value();
    else std::cout << "\nFAILED - No best cost found";
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
  #########)";
char const* pData = R"(#############
#...........#
###C#D#A#B###
  #D#C#B#A#
  #D#B#A#C#
  #B#A#D#C#
  #########)";

  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto init_state = parse(in);
    Visited visited{};
    auto cost = best(0,{init_state,0},visited);
    if (cost) result = cost.value();
    else std::cout << "\nFAILED - No best cost found";
    return result;
  }
}
int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(part1::pTest)});
  answers.push_back({"Part 1     ",part1::solve_for(part1::pData)});
  answers.push_back({"Part 2     ",part2::solve_for(part2::pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(part2::pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // Visual Studio fix if unchecking "Tools->Options->Debugging->Automatically close the console when debugging stops" does not work
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}
