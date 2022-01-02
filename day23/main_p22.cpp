#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <optional>
#include <map>
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
std::vector<std::string> pTestStates{
  R"(#############
#...........#
###B#C#B#D###
  #D#C#B#A#
  #D#B#A#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#..........D#
###B#C#B#.###
  #D#C#B#A#
  #D#B#A#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#A.........D#
###B#C#B#.###
  #D#C#B#.#
  #D#B#A#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#A........BD#
###B#C#.#.###
  #D#C#B#.#
  #D#B#A#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#A......B.BD#
###B#C#.#.###
  #D#C#.#.#
  #D#B#A#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#AA.....B.BD#
###B#C#.#.###
  #D#C#.#.#
  #D#B#.#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#AA.....B.BD#
###B#.#.#.###
  #D#C#.#.#
  #D#B#C#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#AA.....B.BD#
###B#.#.#.###
  #D#.#C#.#
  #D#B#C#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#AA...B.B.BD#
###B#.#.#.###
  #D#.#C#.#
  #D#.#C#C#
  #A#D#C#A#
  #########)"
  ,R"(#############
#AA.D.B.B.BD#
###B#.#.#.###
  #D#.#C#.#
  #D#.#C#C#
  #A#.#C#A#
  #########)"
  ,R"(#############
#AA.D...B.BD#
###B#.#.#.###
  #D#.#C#.#
  #D#.#C#C#
  #A#B#C#A#
  #########)"
  ,R"(#############
#AA.D.....BD#
###B#.#.#.###
  #D#.#C#.#
  #D#B#C#C#
  #A#B#C#A#
  #########)"
  ,R"(#############
#AA.D......D#
###B#.#.#.###
  #D#B#C#.#
  #D#B#C#C#
  #A#B#C#A#
  #########)"
  ,R"(#############
#AA.D......D#
###B#.#C#.###
  #D#B#C#.#
  #D#B#C#.#
  #A#B#C#A#
  #########)"
  ,R"(#############
#AA.D.....AD#
###B#.#C#.###
  #D#B#C#.#
  #D#B#C#.#
  #A#B#C#.#
  #########)"
  ,R"(#############
#AA.......AD#
###B#.#C#.###
  #D#B#C#.#
  #D#B#C#.#
  #A#B#C#D#
  #########)"
  ,R"(#############
#AA.......AD#
###.#B#C#.###
  #D#B#C#.#
  #D#B#C#.#
  #A#B#C#D#
  #########)"
  ,R"(#############
#AA.......AD#
###.#B#C#.###
  #.#B#C#.#
  #D#B#C#D#
  #A#B#C#D#
  #########)"
  ,R"(#############
#AA.D.....AD#
###.#B#C#.###
  #.#B#C#.#
  #.#B#C#D#
  #A#B#C#D#
  #########)"
  ,R"(#############
#A..D.....AD#
###.#B#C#.###
  #.#B#C#.#
  #A#B#C#D#
  #A#B#C#D#
  #########)"
  ,R"(#############
#...D.....AD#
###.#B#C#.###
  #A#B#C#.#
  #A#B#C#D#
  #A#B#C#D#
  #########)"
  ,R"(#############
#.........AD#
###.#B#C#.###
  #A#B#C#D#
  #A#B#C#D#
  #A#B#C#D#
  #########)"
  ,R"(#############
#..........D#
###A#B#C#.###
  #A#B#C#D#
  #A#B#C#D#
  #A#B#C#D#
  #########)"
  ,R"(#############
#...........#
###A#B#C#D###
  #A#B#C#D#
  #A#B#C#D#
  #A#B#C#D#
  #########)"
};
char const* pTest0 = R"(#############
#...........#
###A#B#D#C###
  #A#B#C#D#
  #A#B#C#D#
  #A#B#C#D#
  #########)";

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

  namespace impl {
    std::ostream& operator<<(std::ostream& os,State state) {
      os << "\n";
      for (auto row : state) {
        os << "\n" << row;
      }
      return os;
    }
  }

  std::ostream& operator<<(std::ostream& os,std::pair<Move,Cost> const& step) {
    using namespace impl;
    os << step.first;
    os << " cost " << step.second;
    return os;
  }

  std::ostream& operator<<(std::ostream& os,std::pair<State,Cost> state_cost) {
    using namespace impl;
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
  bool is_home( Pos const& pos,State const& state) {
    bool result{true};
    if (pos.row==1) return false;
    char type = 'A'+(pos.col-3)/2;
    for (auto row : ROOM_ROWS) {
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
    for (auto row : ROOM_ROWS) {
      char ch=state[row][col];
      if (ch=='.') pos = {row,col};
      else if (home_pos_exist = home_pos_exist and (ch == '.' or ch==type);!home_pos_exist) break;
    }
    if (home_pos_exist) result = pos;
    // if (result) std::cout << " " << type << " has home";
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
      // std::cout << "\npush_back";
      if (!this->blocked_move(move) and !this->will_not_work(move)) {
        this->steps.push_back({move,move_cost(state[move.from.row][move.from.col],move)});
        // std::cout << "\npush_back count:" << steps.size();
      }
      return *this;
    }
    std::vector<std::pair<Move,Cost>> selected() {    
      auto& [state,cost] = m_state_cost;
      // std::cout << "\nselected:";
      // for (auto& [move,cost] : steps) std::cout << state[move.from.row][move.from.col] << "{" << move.from.row << "," << move.from.col << "}"
      //   << "->{" << move.to.row << "," << move.to.col << "}";
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
      // if (result) std::cout << " blocked " << move;
      return result;
    }
    bool will_not_work(Move const& move) {
      bool result{false};
      auto& [state,cost] = m_state_cost;
      // TODO: Find some way to optimise away moves that will not work!
      return result;
    }

  };
  std::vector<std::pair<Move,Cost>> expand_from(std::pair<State,Cost> const& state_cost, Pos const& pos) {
    MoveSelector move_selector{state_cost};
    // std::cout << "\nexpand_from {" << pos.row << "," << pos.col << "}";
    auto& [state,cost] = state_cost;
    auto home = home_pos(state[pos.row][pos.col],state);
    if (home) {
      // std::cout << "\npushed home";
      move_selector.push_back({pos,home.value()}); // prefer go home
    }
    else if (pos.row>1) {
      // room to left alcove
      if (state[1][2]=='.') {
        if (state[1][1]=='.') {
          move_selector.push_back({pos,Pos{1,1}});
        }
        move_selector.push_back({pos,Pos{1,2}});
      }
      // room to right alcove
      if (state[1][10]=='.') {
        if (state[1][11]=='.') {
          move_selector.push_back({pos,Pos{1,11}});
        }
        move_selector.push_back({pos,Pos{1,10}});
      }
      // Room to between rooms
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
    for (auto col : ROOM_COLUMNS) {
      for (auto row : ROOM_ROWS) {
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
    for (auto col : HALLWAY_COLUMNS) {
        auto ch = state[1][col];
        if (ch>='A' and ch <='D') {
          Pos from{1,col};
          auto moves = expand_from(state_cost,from);
          std::copy(moves.begin(),moves.end(),std::back_inserter(result));
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
    std::sort(result.begin(),result.end(),[&state](auto const& step1,auto const& step2){
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
      // if (heuristic1>heuristic2) std::cout << "\n" << step1 << " h:" << heuristic1 << " before " << step2 << " h:" << heuristic2;
      // else std::cout << "\n" << step2 << " h:" << heuristic2 << " before " << step1 << " h:" << heuristic1;
      return heuristic1>heuristic2; // prefer steps with highest heuristic
    });
    // std::cout << "\nstrategic: ";
    // for (auto const& step : result) std::cout << step;
    return result;
  }
  std::pair<State,Cost> next(std::pair<State,Cost> const& state_cost,std::pair<Move,Cost> const& step) {
    // std::cout << "\nnext " << step;
    auto& [state,cost] = state_cost;
    State stepped_state{state};
    auto [move,move_cost] = step;
    char ch = stepped_state[move.from.row][move.from.col]; 
    stepped_state[move.from.row][move.from.col] = '.';
    stepped_state[move.to.row][move.to.col] = ch;
    return {stepped_state,cost+move_cost};
  }
  std::optional<Cost> best(int stack_level,std::pair<State,Cost> const& state_cost,State const& end_state,Visited& visited) {
    if (stack_level==0) std::cout << "\nInit state " << state_cost;
    // std::cout << "\nbest[" << stack_level << "]";
    static int call_count{0};
    std::optional<Cost> result{};
    ++call_count;
    auto& [state,cost] = state_cost;
    // if (call_count>3) return result;
    if (state == end_state) {
      std::cout << state_cost << std::endl;
      throw std::runtime_error("\n************** END STATE OK *****************");
      return 0;
    }
    if (is_end_state(state)) return 0;
    if (auto iter = visited.find(state);iter != visited.end()) {
      // std::cout << " visited";
      return iter->second;
    }
    auto potential_steps = expand(state_cost);
    auto strategic_steps = apply_strategy(state_cost,potential_steps);
    // std::cout << "\nsteps count " << strategic_steps.size();
    // for (auto const& step : strategic_steps) std::cout << step;
    std::vector<Cost> costs{};

    if (call_count%10000==0) std::cout << "\n" << call_count;

    for (auto const& step : strategic_steps) {
      // std::cout << "\n" << step;
      auto next_state_cost = next(state_cost,step); 
      auto next_cost = best(stack_level+1,next_state_cost,end_state,visited);
      if (next_cost) costs.push_back(cost + next_cost.value());
      visited[next_state_cost.first] = next_cost;
    }
    if (auto iter = std::min_element(costs.begin(),costs.end());iter!=costs.end()) result=*iter;
    // if (result) std::cout << "best " << result.value();
    if (stack_level==0) {
      using namespace impl;
      std::cout << "\n<Cost to state>" << end_state;
      if (result) std::cout << " " << result.value();
      else std::cout << "\nFAILED to find end state";
    }
    return result;
  }
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto init_state = parse(in);
      std::stringstream end_in{ pEnd };
      auto end_state = parse(end_in);
      for (int i=1;i<pTestStates.size();i++) {
      // for (int i=10;i<11;i++) {
        std::cout << "\n\nTEST " << i;
        std::stringstream in{ pTestStates[i-1] };
        auto init_state = parse(in);
        std::stringstream end_in{pTestStates[i]};
        auto end_state = parse(end_in);
        Visited visited{};
        try {
          auto cost = best(0,{init_state,0},end_state,visited);
          if (cost) result = cost.value();
          else std::cout << "\nFAILED - No best cost found";
        }
        catch (std::exception const& e) {
          std::cout << e.what();
        }
      }
      return result;
  }
}
int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(part2::pTest0)});
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
