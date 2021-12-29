//
//  main.cpp
//  day23
//
//  Created by Kjell-Olov Högdal on 2021-12-23.
//
#include <iostream>
#include <array>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <unordered_map>

const char* pTest = R"(#############
#...........#
###B#C#B#D###
  #A#D#C#A#
  #########)";
const char* pData = R"(#############
#...........#
###C#D#A#B###
  #B#A#D#C#
  #########)";
using Result = size_t;

struct Pos {
  int row,col;
  bool operator<(Pos const& other) const {return (row!=other.row)?row<other.row:col<other.col;}
  bool operator==(Pos const& other) const {return (row!=other.row)?row==other.row:col==other.col;}
};
struct State {
  struct AmphipodTracker {
    char type;
    Pos pos;
    bool operator<(AmphipodTracker const& other) const { // to work with std::map/set
      return (type!=other.type?type<other.type:pos<other.pos);
    }
    bool operator==(AmphipodTracker const& other) const {
      return (type!=other.type?type==other.type:pos==other.pos);
    }
  };
  std::vector<AmphipodTracker> pods{};
  bool operator<(State const& other) const {return pods<other.pods;} // to work with std::map/set
  bool operator==(State const& other) const {return pods == other.pods;}
};
// custom specialization of std::hash for State injected into namespace std
template<>
struct std::hash<State>
{
    std::size_t operator()(State const& state) const noexcept {
      size_t result;
      for (auto const& pod : state.pods) {
        result ^= std::hash<char>{}(pod.type);
        result ^= std::hash<int>{}(pod.pos.row);
        result ^= std::hash<int>{}(pod.pos.col);
      }
      return result;
    }
};
using Cost = size_t;
struct Burrow {
  std::vector<std::string> floor_plan{};
  size_t corridor_ix_left{};
  size_t corridor_ix_right{};
  std::array<int,4> room_column_ix{};
  size_t room_height{};
  State end_state() {
    // create a State with 'A','B','C' and 'D' in each of the rooms left to right
    State result{};
    for (int row_ix = 0;row_ix<room_height;row_ix++) {
      for (int room_ix=0;room_ix<room_column_ix.size();room_ix++) {
        int col_ix = room_column_ix[room_ix];
        result.pods.push_back({static_cast<char>('A'+room_ix),row_ix+2,col_ix});
      }
    }
    return result;
  }
};
using PuzzleModel = std::pair<State,Burrow>;

PuzzleModel parse(std::istream& in) {
  State init_state{};
  Burrow burrow{};
  std::string line;
  int burrow_row{0};
  while (std::getline(in, line)) {
    switch (burrow_row) {
      case 0: {// first line with uppe wall of burrow
        burrow.floor_plan.push_back(line);
      } break;
      case 1: { // second line with the "corridor"
        burrow.corridor_ix_left = line.find_first_of(".");
        burrow.corridor_ix_right = line.find_last_of(".");
        std::transform(line.begin(),line.end(),line.begin(),[](char ch){return (ch=='.')?' ':ch;});
        burrow.floor_plan.push_back(line);
      } break;
      case 2:
      case 3: {
        burrow.room_height++;
        int room_ix{0};
        for (int ix=0;ix<line.length();ix++) {
          char ch = line[ix];
          if (ch>='A' and ch<='D') {
            burrow.room_column_ix[room_ix++] = ix;
            State::AmphipodTracker pod{};
            pod.type = ch;
            pod.pos.row = burrow_row;
            pod.pos.col = ix;
            init_state.pods.push_back(pod);
          }
        }
        std::transform(line.begin(),line.end(),line.begin(),[](char ch){return (ch!='#')?' ':'#';});
        burrow.floor_plan.push_back(line);
      } break;
      default:
        burrow.floor_plan.push_back(line);
        break;
    }
    burrow_row++;
  }
  // Log
  if (true) {
    std::cout << "\n<burrow floor plan>";
    for (auto const& row : burrow.floor_plan) {
      std::cout << "\n" << row;
    }
    std::cout << "\ncorridor ix: " << burrow.corridor_ix_left << " .. " << burrow.corridor_ix_right;
    std::cout << "\nroom height: " << burrow.room_height;
    std::cout << "\nroom ixs: ";
    for (auto const& col : burrow.room_column_ix) {
      std::cout << " " << col;
    }
  }
  // Log
  if (true) {
    std::cout << "\n<Init State>";
    for (auto const& pod : init_state.pods) {
      std::cout << "\n\t" << pod.type << "{row:" << pod.pos.row << ",col:" << pod.pos.col << "}";
    }
  }
  return {init_state,burrow};
}

auto const step_cost = [](char type,int step_count)->Cost {
  // the step cost depends on the type of amphipod and the number of steps taken
  Cost single_step_cost{1};
  for (int i=0;i<type-'A';i++) single_step_cost *= 10; // costs 'A' = 1, 'B' = 10, 'C'=100 'D'=1000
  return step_count*single_step_cost;
};

std::set<Pos> free_space(Pos const& pos,Burrow const& map) {
  // How many positions are reachable (unblocked from reaching) from provided pos on provided map?
  std::set<Pos> visited{};
  std::vector<Pos> frontier{};
  // Keep expanding the current frontier (flood fill) until
  // all alternatives are blocked
  frontier.push_back(pos);
  while (frontier.size()>0) {
    auto fp = frontier.back();
    frontier.pop_back();
    visited.insert({fp});
    // Step from frontier pos (fp)
    for (auto dr : {-1,0,1}) {
      for (auto dc : {-1,0,1}) {
        Pos np{fp.row+dr,fp.col+dc};
        // std::cout << " try{row:" << np.row << ",col:" << np.col << "}";
        if (std::abs(dr)==std::abs(dc)) continue; // 0,0 and diagonal not allowed
        if (map.floor_plan[np.row][np.col] != ' ') continue; // blocked
        if (visited.find(np) != visited.end()) continue; // skip visited
        // std::cout << "ACCEPT";
        frontier.push_back(np);
      }
    }
  }
  // Log
  if (false) {
    std::cout << "\n\t";
    for (auto const& pos : visited) {
      std::cout << "\nvisited: {" << pos.row << "," << pos.col << "}";
    }
  }
  visited.erase(pos);
  return visited;
}

void investigate();

int main(int argc, const char * argv[]) {
  // Investigate algorithmic solutions
  if (false) {
    investigate();
  }
  // Find the final solution
  if (true) {
    // Observation: We are looking for the lowest cost to sort out the amphipods
    // in the burrow using steps of moves with ascociates cost.
    // So we have steps to take one after the other, i.e., we have a path of steps.
    // Each step changes the position of amphipods which we can call the current State.
    // Now we need to not squint very much to see that States forms nodes of a graph
    // where there is a cost to "step" from node to node until we reach the end node :)
    // ==> We have a shortest path problem!
    
    // General shortest path finding algorithm
    // Given 1: A graph with vertices connected by weighted edges.
    // Given 2: A defined start vertex "Start" and a defined end vertex "End".
    // Given 3: A mapping "cost so far" of each vertex to a pair of data.
    //          First being "lowest cost so far", the lowest cost so far to come to mapped vertex from "Start"
    //          Second being "from", the vertex we came from with the recorded "lowest cost so far"
    //          Set "lowest cost so far" to "higher than anything we will encounter".
    //          Set "parent" to Start (or None)
    // Algorithm:
    //
    //    Create an empty set of Vertices called "visitied"
    //    Create a set of vertices called "unvisited" and put potentially "all" vertices, but at least the start vertex,in it.
    //    while "unvisited" contains vertecies
    //      remove the vertex with the lowest "lowest cost so far" from the set "unvisited" and call it "visitee"
    //      expand "visitee" to a new vector "frontier" of all vertecies connected to it that we have not yet visitied
    //      ensure allvertecies in "frontier" are present in "unvisited" (if not done initially and we generate the graph as-we-go)
    //      for each vertex "connected" in the "frontier"
    //        if: "visitee" "lowest cost so far" + cost to "connected" < "connected" "lowest cost so far"
    //          then: update "connected" "lowest cost so far" to the new lowest cost
    //          and: update "connected" "from" to visitee
    //      end-of-each-in-frontier
    //      put "current" in "visited"
    //    end-of-while "unvisitied"
    //    The map "cost_so_far" now maps "End" to the lowest cost to reach it from Start :)

    // Ok, for our burrow with amphipods we need.
    // 1. A State that defines where each amphipod currently is.
    // See State above

    // 2. A lowest_cost_so_far that maps a State to the pair (initiate new mappings as we process the frontier through the graph)
    std::map<State,std::pair<Cost,State>> lowest_cost_so_far{};

    // 3. An "edge" as a "step cost" to go from one State to the next
    //    Note: In our case we generate the edges as we go but we still need the function to get us the cost from relevant factors
    // See step_cost fucntion above

    // 4. We need the burrow for the amphipods to move around in
    // See Burrow

    // 5. Now we can create the Burrow and the init state from the input
    std::istringstream in{pTest};
    auto [start_state,burrow] = parse(in);
    // OK, lets find the cheapest (shortest) path
    // Prerequisite: A defined start vertex "Start" and a defined end vertex "End".
    auto end_state = burrow.end_state();
    // Log
    if (true) {
      std::cout << "\n<End State>";
      for (auto const& pod : end_state.pods) {
        std::cout << "\n\t" << pod.type << "{row:" << pod.pos.row << ",col:" << pod.pos.col << "}";
      }
    }
    //    Create an empty set of Vertices called "visitied"
    std::unordered_set<State> visited{};
    //    Create a set of vertices called "unvisited" and put potentially "all" vertices, but at least the start vertex,in it.
    auto least_cost = [&lowest_cost_so_far](State const& s1,State const& s2) {
      return (lowest_cost_so_far[s1].first>lowest_cost_so_far[s2].first);
      // Note: std::priority_queue default to have "largest" element first + expects compare function to return "less than"!
      // This we implement this compare function as "greater than" to fool the priority queue to
      // put the State with the lowest cost first.
      // ...blows my mind...
    };
    std::priority_queue<State,std::vector<State>,decltype(least_cost)> unvisited(least_cost);
    // Insert our start state into unvisited
    unvisited.push(start_state);
    //    while "unvisited" contains vertecies
    while (unvisited.size()>0) {
    //      remove the vertex with the lowest "lowest cost so far" from the set "unvisited" and call it "visitee"
      auto visitee_state = unvisited.top();
      unvisited.pop();
    //      expand "current" to a vector "frontier" of all vertecies connected to it that we have not yet visitied
      std::unordered_map<State,Cost> frontier{};
      {
        // Create a map with all the amphipods in their current lcoations
        Burrow visitee_map{burrow};
        for (auto const& visitee_pod : visitee_state.pods) {
          visitee_map.floor_plan[visitee_pod.pos.row][visitee_pod.pos.col] = visitee_pod.type;
        }
        // Log
        {
          std::cout << "\n<visitee map>";
          for (auto const& row : visitee_map.floor_plan) {
            std::cout << "\n" << row;
          }
        }
        // Expand visitee pod into all possible states to move to
        for (int pix=0;pix<visitee_state.pods.size();pix++) {
          auto visitee_pod = visitee_state.pods[pix];
          // Exhaust the reachable positions from visitee pod position
          auto reachable = free_space(visitee_pod.pos, visitee_map);
          // A pod in a room or in a hallway is allowed to go into its correct room right away.
          // Expand frontier with the valid move for this pod back to its correct room
          std::set<Pos> room_pos{};
          std::copy_if(reachable.begin(), reachable.end(), std::inserter(room_pos, room_pos.end()), [&visitee_map](Pos const& pos){
            // return true if in a room
            if (pos.row==1) return false;
            return true;
          });
          // Filter room_pos for a position in correct room and correct or no room mate
          std::optional<Pos> no_home{};
          std::optional<Pos> home = std::accumulate(
              room_pos.begin()
            , room_pos.end()
            , no_home
            ,[&visitee_map,&visitee_pod](auto acc, Pos const& pos)->std::optional<Pos>{
              if (pos.col != visitee_map.room_column_ix[visitee_pod.type-'A']) return acc; // wrong room
              if (visitee_map.floor_plan[pos.row+1][pos.col] == ' ') return acc; // not at bottom of room
              if (visitee_map.floor_plan[pos.row+1][pos.col] == '#') return pos; // empty room ok
              if (visitee_map.floor_plan[pos.row+1][pos.col] == visitee_pod.type) return pos; // correct room mate
              return acc; // nop
          });
          if (home) {
            // Pod can go home
            auto dr = std::abs(home->row - visitee_pod.pos.row);
            auto dc = std::abs(home->col - visitee_pod.pos.col);
            auto cost = step_cost(visitee_pod.type, dr+dc);
            // add to frontier
            auto new_state = visitee_state;
            new_state.pods[pix].pos = home.value();
            frontier[new_state] = cost;
          }
          else if (visitee_pod.pos.row!=1) {
            // Pod can go into the hallway
            std::set<Pos> hallway_pos{};
            // Filder on valid hallway position
            std::copy_if(reachable.begin(), reachable.end(), std::inserter(hallway_pos, hallway_pos.begin()), [&visitee_map](Pos const& pos){
              if (std::any_of(visitee_map.room_column_ix.begin(), visitee_map.room_column_ix.end(), [&pos] (int col){
                return (pos.col==col);
              })) return false;
              return true;
            });
            // Expand frontier with valid hallway positions
            std::transform(hallway_pos.begin(),hallway_pos.end(),std::inserter(frontier,frontier.end())
                      ,[&pix,&visitee_pod,&visitee_state](Pos const& pos)->std::pair<State,Cost> {
              std::pair<State,Cost> result{};
              auto dr = std::abs(pos.row - visitee_pod.pos.row);
              auto dc = std::abs(pos.col - visitee_pod.pos.col);
              auto cost = step_cost(visitee_pod.type, dr+dc);
              // add to frontier
              auto new_state = visitee_state;
              return {new_state,cost};
            });
          }
        }
      }
    //      for each vertex (front_visitee) in the "frontier"
      for (auto const& front_visitee : frontier) {
        if (lowest_cost_so_far.find(front_visitee.first) == lowest_cost_so_far.end()) {
          // Expand our cost map with an entry for connected state
          lowest_cost_so_far[front_visitee.first] = {std::numeric_limits<Cost>::max(),{}}; // init cost and parent
        }
    //        if: "current" "lowest cost so far" + cost to "to visit" < "connected" "lowest cost so far"

        auto new_connected_cost = lowest_cost_so_far[visitee_state].first+front_visitee.second;
        if (new_connected_cost < lowest_cost_so_far[front_visitee.first].first) {
    //          then: update "connected" "lowest cost so far" to "current" "lowest cost so far" + cost to "connected"
          lowest_cost_so_far[front_visitee.first].first = new_connected_cost;
    //          and: update "connected" "from" to current
          lowest_cost_so_far[front_visitee.first].second = visitee_state;
        }
      } //      end-of-each-in-frontier
      //      ensure all vertecies in "frontier" are present in "unvisited" (if not done initially = we generate the graph as we step)
      for (auto const& front_visitee : frontier) {
        unvisited.push(front_visitee.first);
        // Note: We insert them here after they have been updated with the cost so far to have them
        // correctly ordered by cost in the queue
      }
    //      put "current" in "visited"
      visited.insert(visitee_state);
    } //    end-of-while "unvisitied"
    //    The map "cost_so_far" now maps "End" to the lowest cost to reach it from Start :)
    auto result = lowest_cost_so_far[end_state].first;
    std::cout << "\nLowest cost " << result;
    
    
  }
  std::cout << "\n";
  return 0;
}

void investigate() {
  using Path = std::vector<Pos>;
  std::istringstream in{pTest};
  auto [init_state,burrow] = parse(in);
  if (false) {
    // expand a position into possible moves out into the corridor
    using CandidateMoves = std::vector<Path>;
    for (auto const& pod : init_state.pods) {
      CandidateMoves candidate_moves{};
      std::cout << "\nexpanding : " << pod.type << "{row:" << pod.pos.row << ",col:" << pod.pos.col << "}";
      auto pp = pod.pos;
      Path path{};
      // Move to doorway
      for (int dr=1;dr<=pp.row-1;dr++) {
        path.push_back({pp.row-dr,pp.col});
      }
      // move left
      Path left_path{path};
      for (int dc=1;dc<=pp.col-burrow.corridor_ix_left;dc++) {
        left_path.push_back({1,pp.col-dc});
        candidate_moves.push_back(left_path);
      }
      // move right
      Path right_path{path};
      for (int dc=1;dc<=burrow.corridor_ix_right-pp.col;dc++) {
        right_path.push_back({1,pp.col+dc});
        candidate_moves.push_back(right_path);
      }
      // Log
      if (true) {
        for (auto const path : candidate_moves) {
          std::cout << "\n\tpath:";
          for (auto const& pos : path) {
            std::cout << " {row:" << pos.row << ",col:" << pos.col << "}";
          }
        }
      }
    }
  }
  if (false) {
    // Try the approach to expand a frontier of possible moves for pod 0 (row:2 col:3)
    // by using a set of visited positions to prevent back-tarcking,
    // and a map showing where the other pos are to prevent moving to blocked positions.

    auto current = init_state;
    
    // Create a map with all the amphipods in their current lcoations
    Burrow current_map{burrow};
    for (auto const& pod : current.pods) {
      current_map.floor_plan[pod.pos.row][pod.pos.col] = pod.type;
    }

    int pix = 0;
    auto pod = current.pods[pix];
    auto pp = pod.pos;
    std::set<Pos> visited{};
    std::vector<Path> frontier{};
    // Can we move a single step?
    for (auto dr : {-1,0,1}) {
      for (auto dc : {-1,0,1}) {
        if (std::abs(dr)==std::abs(dc)) continue; // 0,0 and diagonal not allowed
        if (current_map.floor_plan[pp.row+dr][pp.col+dr] != ' ') continue; // blocked
        Pos pos{pp.row+dr,pp.col+dc};
        frontier.push_back({});
        frontier.back().push_back(pos);
        visited.insert({pos});
      }
    }
    for (auto const& path : frontier) {
      std::cout << "\nreachable: {" << path.back().row << "," << path.back().col << "}";
    }
  }
  if (false) {
    // Try to eshaust an expansion step by step from a pos position until
    // all alterantives are blocked (flood fill using step rules)

    auto current = init_state;
    
    // Create a map with all the amphipods in their current lcoations
    Burrow current_map{burrow};
    for (auto const& pod : current.pods) {
      current_map.floor_plan[pod.pos.row][pod.pos.col] = pod.type;
    }
    // Choose pod 0
    int pix = 0;
    auto cp = current.pods[pix];
    auto cpp = cp.pos;
    std::set<Pos> visited{},reachable{};
    std::vector<Pos> frontier{};
    // Keep expanding the current frontier (flood fill) until
    // all alternatives are blocked
    frontier.push_back(cpp);
    while (frontier.size()>0) {
      auto fp = frontier.back();
      frontier.pop_back();
      visited.insert({fp});
      // Step from frontier pos (fp)
      std::cout << "\n\t";
      for (auto dr : {-1,0,1}) {
        for (auto dc : {-1,0,1}) {
          Pos np{fp.row+dr,fp.col+dc};
          // std::cout << " try{row:" << np.row << ",col:" << np.col << "}";
          if (std::abs(dr)==std::abs(dc)) continue; // 0,0 and diagonal not allowed
          if (current_map.floor_plan[np.row][np.col] != ' ') continue; // blocked
          if (visited.find(np) != visited.end()) continue; // skip visited
          // std::cout << "ACCEPT";
          frontier.push_back(np);
          reachable.insert(np);
        }
      }
    }
    for (auto const& pos : reachable) {
      std::cout << "\nreachable: {" << pos.row << "," << pos.col << "}";
    }
  }
  if (false) {
    // Now investigate how many combinations of moves for pod 0 and then pod 1 we have
    auto current_state = init_state;
    // Create a map with all the amphipods in their current lcoations
    Burrow current_map{burrow};
    for (auto const& pod : current_state.pods) {
      current_map.floor_plan[pod.pos.row][pod.pos.col] = pod.type;
    }
        
    // Choose pod 0
    int pix = 0;
    auto cp = current_state.pods[pix];
    auto cpp = cp.pos;
    auto reachable0 = free_space(cpp, current_map);
    // Log
    {
      for (auto const& pos : reachable0) {
        std::cout << "\ntreachable 0: {" << pos.row << "," << pos.col << "}";
      }
    }
    for (auto const& np : reachable0) {
      auto next_state = current_state;
      next_state.pods[0].pos = np;
      // Move pod 0 to new position np
      auto next_map = burrow;
      // draw the map with all pods in place
      for (auto const& pod : next_state.pods) {
        next_map.floor_plan[pod.pos.row][pod.pos.col] = pod.type;
      }
      // log
      {
        std::cout << "\npod 0 at {row:" << np.row << ",col:" << np.col << "}";
        std::cout << "\n<floor plan>";
        for (auto const& row : next_map.floor_plan) {
          std::cout << "\n" << row;
        }
      }
      auto reachable1 = free_space(next_state.pods[4].pos, next_map);
      // log
      if (true) {
        for (auto const& pos : reachable1) {
          std::cout << "\n\treachable 1: {" << pos.row << "," << pos.col << "}";
        }
      }
    }
  }
  if (true) {
    // Investogate the ways to move pod 0 out into the corridor
    // and then back into room 0 again
    auto current_state = init_state;
    // Create a map with all the amphipods in their current lcoations
    Burrow current_map{burrow};
    for (auto const& pod : current_state.pods) {
      current_map.floor_plan[pod.pos.row][pod.pos.col] = pod.type;
    }
        
    // Choose pod 0
    int pix = 0;
    auto pod = current_state.pods[pix];
    auto pos = pod.pos;
    std::set<Pos> corridor{};
    {
      // Find all ways for pod_0 top move out into the corridor
      auto reachable = free_space(pos, current_map);
      // Filter to valid positions in corridor
      std::copy_if(reachable.begin(),reachable.end(),std::inserter(corridor,corridor.end()),[&current_map](Pos const& pos){
        if (pos.row != 1) return false;
        if (pos.col<current_map.corridor_ix_left) return false;
        if (pos.col>current_map.corridor_ix_right) return false;
        for (auto doorway_ix : current_map.room_column_ix) {
          if (pos.col == doorway_ix) return false;
        }
        return true;
      });
      // Log
      {
        std::cout << "\corridor:";
        for (auto pos : corridor) {
          std::cout << " {row:" << pos.row << ",col:" << pos.col << "}";
        }
      }
    }
    {
      // For each position in the hallway, find the cost to move back to room 0
      for (auto pos : corridor) {
        auto next_state = current_state;
        auto next_map = burrow;
        // move pos to hallway
        next_state.pods[pix].pos = pos;
        // Draw the new map
        for (auto const& pod : next_state.pods) {
          next_map.floor_plan[pod.pos.row][pod.pos.col] = pod.type;
        }
        // Log
        {
          std::cout << "\n<map>";
          for (auto const& row : next_map.floor_plan) {
            std::cout << "\n" << row;
          }
        }
        auto reachable = free_space(pos, next_map);
        // Log
        {
          std::cout << "\nreachable:";
          for (auto pos : reachable) {
            std::cout << " {row:" << pos.row << ",col:" << pos.col << "}";
          }
        }
        // Filter on position in room 0
        std::set<Pos> room_0{};
        std::copy_if(reachable.begin(), reachable.end(), std::inserter(room_0,room_0.end()), [&next_map](Pos const& pos){
          if (pos.row==1) return false;
          if (pos.col!=next_map.room_column_ix[0]) return false;
          return true;
        });
        // Log
        {
          std::cout << "\n<room 0> ";
          for (auto pos : room_0) {
            std::cout << " {row:" << pos.row << ",col:" << pos.col << "}";
          }
        }
        // What is the cost to move to any position in room_0?
        struct StepCost {
          char type;
          Pos from,to;
          Cost cost;
        };
        std::vector<StepCost> costs;
        for (auto room_pos : room_0) {
          auto dr = std::abs(pos.row-room_pos.row);
          auto dc = std::abs(pos.col-room_pos.col);
          auto cost = step_cost(pod.type, dr+dc);
          costs.push_back({pod.type,pos,room_pos,cost});
        }
        // Log
        {
          std::cout << "\n<costs>";
          for (auto cost : costs) {
            std::cout << "\n\tpod " << cost.type << "{row:" << cost.from.row << ",col:" << cost.from.col << "}"
            << " -> {row:" << cost.to.row << ",col:" << cost.to.col << "} = " << cost.cost;
          }
        }
      }
    }
  }

}
