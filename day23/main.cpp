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
};
struct State {
  struct AmphipodTracker {
    char type;
    Pos pos;
    bool operator<(AmphipodTracker const& other) const { // to work with std::map/set
      return (type!=other.type?type<other.type:pos<other.pos);
    }
  };
  std::vector<AmphipodTracker> ap_trackers{};
  bool operator<(State const& other) const {return ap_trackers<other.ap_trackers;} // to work with std::map/set
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
        result.ap_trackers.push_back({static_cast<char>('A'+room_ix),row_ix+2,col_ix});
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
            State::AmphipodTracker ap_tracker{};
            ap_tracker.type = ch;
            ap_tracker.pos.row = burrow_row;
            ap_tracker.pos.col = ix;
            init_state.ap_trackers.push_back(ap_tracker);
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
    for (auto const& ap_tracker : init_state.ap_trackers) {
      std::cout << "\n\t" << ap_tracker.type << "{row:" << ap_tracker.pos.row << ",col:" << ap_tracker.pos.col << "}";
    }
  }
  return {init_state,burrow};
}

void investigate();

int main(int argc, const char * argv[]) {
  // Investigate algorithmic solutions
  if (true) {
    investigate();
  }
  // Find the final solution
  if (false) {
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
    //      remove the vertex with the lowest "lowest cost so far" from the set "unvisited" and call it "current"
    //      expand "current" to a vector "frontier" of all vertecies connected to it that we have not yet visitied
    //      ensure allvertecies in "frontier" are present in "unvisited" (if not done initially and we generate the graph as-we-go)
    //      for each vertex "connected" in the "frontier"
    //        if: "current" "lowest cost so far" + cost to "connected" < "connected" "lowest cost so far"
    //          then: update "connected" "lowest cost so far" to "current" "lowest cost so far" + cost to "connected"
    //          and: update "connected" "from" to current
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
    auto step_cost = [](char type,int step_count)->Cost {
      // the step cost depends on the type of amphipod and the number of steps taken
      Cost single_step_cost{1};
      for (int i=0;i<'A'-type;i++) single_step_cost *= 10; // costs 'A' = 1, 'B' = 10, 'C'=100 'D'=1000
      return step_count*single_step_cost;
    };
    // 4. We need the burrow for the amphipods to move around in
    // See Burrow
    // 5. Now we can create the Burrow and the init state from the input
    std::istringstream in{pTest};
    auto [init_state,burrow] = parse(in);
    // OK, lets find the cheapest (shortest) path
    // Prerequisite: A defined start vertex "Start" and a defined end vertex "End".
    auto end_state = burrow.end_state();
    // Log
    if (true) {
      std::cout << "\n<End State>";
      for (auto const& ap_tracker : end_state.ap_trackers) {
        std::cout << "\n\t" << ap_tracker.type << "{row:" << ap_tracker.pos.row << ",col:" << ap_tracker.pos.col << "}";
      }
    }
    //    Create an empty set of Vertices called "visitied"
    std::set<State> visited{};
    //    Create a set of vertices called "unvisited" and put potentially "all" vertices, but at least the start vertex,in it.
    auto least_cost = [&lowest_cost_so_far](State const& s1,State const& s2) {
      return (lowest_cost_so_far[s1].first>lowest_cost_so_far[s2].first);
      // Note: std::priority_queue default to have "largest" element first + expects compare function to return "less than"!
      // This we implement this compare function as "greater than" to fool the priority queue to
      // put the State with the lowest cost first.
      // ...blows my mind...
    };
    std::priority_queue<State,std::vector<State>,decltype(least_cost)> unvisited(least_cost);
    //    while "unvisited" contains vertecies
    while (unvisited.size()>1) {
    //      remove the vertex with the lowest "lowest cost so far" from the set "unvisited" and call it "current"
      auto current = unvisited.top();
      unvisited.pop();
    //      expand "current" to a vector "frontier" of all vertecies connected to it that we have not yet visitied
      std::vector<std::pair<Cost,State>> frontier{}; // Carry the Cost to move to the candidate state for the update phase below
      {
        // Create a map with all the amphipods in their current lcoations
        Burrow current_map{burrow};
        for (auto const& ap_tracker : current.ap_trackers) {
          current_map.floor_plan[ap_tracker.pos.row][ap_tracker.pos.col] = ap_tracker.type;
        }
        using Path = std::vector<Pos>;
        using CandidateMoves = std::vector<Path>;
        CandidateMoves candidate_moves{};
        for (auto const& ap_tracker : current.ap_trackers) {
          // find all possible moves of this amohipod type and current location on the current map
          // Exhaust the reachable positions from current ap_tracker position
          
          /*
           211223 KoH
           
           Consider the following approach
           
           if in a room:
              First generate the path to the doorway
              Then generate all paths going right in the hallway (all the way to the end wall)
              Then generate all path going left in the hallway (all the way to the end wall))
              Finally generate all paths to the room that is the correct room for it.
              = A vector of path exhausting the move space regardless of allowed on blocing amphipods
           
           If in a corridor:
              Generate all paths to the correct room for the amphipod.
           
           Second: Sort the vector of paths after the length of the paths
           Third: Filter the paths to the ones that are NOT BLOCKED (cut off the path passing a blocking amphipod
           
           NOTE: We already have the map with all amphipods on it so it it easy to detect blocking paths :)
           
           Now we have all the possible and valid moves = we have all the possible new states for this amphipod.
           The cost of each such new state is the length of the path x the step_cost for the amphipod type :)
           
           */
          
          
          std::vector<std::pair<int,int>> reachable{};
          std::set<std::pair<int,int>> visited{};
//          auto row = ap_tracker.row;
//          auto col = ap_tracker.col;
          auto pos = ap_tracker.pos;
          if (pos.row==1) {
            // In the corridor ==> valid move is back to the correct room with a mate of the same type
            // A type 'X' to room column ix = with room_column_ix['X'-'A']
            // Any mate already in the same column (room) must be of the same correct type
            int target_column{0};
            switch (ap_tracker.type) {
              case 'A': target_column=burrow.room_column_ix[0]; break;
              case 'B': target_column=burrow.room_column_ix[1]; break;
              case 'C': target_column=burrow.room_column_ix[2]; break;
              case 'D': target_column=burrow.room_column_ix[3]; break;
            }
            Path path{};
            // Path to correct doorway
            if (pos.col<target_column) {
              for (int col=pos.col;col!=target_column;col++) path.push_back({pos.row,col});
            }
            else {
              for (int col=pos.col;col!=target_column;col--) path.push_back({pos.row,col});
            }
            // paths to possible row in room
            for (int dr = 1;dr<burrow.room_height;dr++) {
              path.push_back({pos.row+dr,target_column});
              candidate_moves.push_back(path);
            }
            // Correct room-mates?
            int wrong_room_mate_count{0};
            for (auto const& path : candidate_moves) {
              if (current_map.floor_plan[path.back().row][path.back().col]!=' ') {
                if (current_map.floor_plan[path.back().row][path.back().col]!=ap_tracker.type) {
                  ++wrong_room_mate_count;
                  break;
                }
              }
            }
            if (wrong_room_mate_count>0) candidate_moves.clear(); // no moves into room possible
            else {
              // filter out unreachable/blocked paths
              auto blocked = std::find_if(candidate_moves.begin(), candidate_moves.end(), [&current_map](auto const& path) {
                return current_map.floor_plan[path.back().row][path.back().col]!=' ';
              });
              candidate_moves.resize(std::distance(candidate_moves.begin(), blocked)-1); // cut the unreachable
            }
          }
          else {
            // In a room ==> try moving out in the corridor
            // We can move to any side of the room entrance and then to the left and right until stopped by wall or occupied column
            // Path to door-way
            Path path{};
            for (int dr=1;dr<pos.row-1;dr++) {
              path.push_back({pos.row-dr,pos.col});
            }
            // move left
            Path left_path{path};
            for (int dc=1;dc<=pos.col-burrow.corridor_ix_left;dc++) {
              left_path.push_back({1,pos.col-dc});
              candidate_moves.push_back(left_path);
            }
            // move right
            Path right_path{path};
            for (int dc=1;dc<=burrow.corridor_ix_right-pos.col;dc++) {
              left_path.push_back({1,ap_tracker.pos.col+dc});
              candidate_moves.push_back(left_path);
            }
          }
        }
      }
    //      for each vertex (connected) in the "frontier"
      for (auto const& connected : frontier) {
        if (lowest_cost_so_far.find(connected.second) == lowest_cost_so_far.end()) {
          // init a new mapping
          std::pair<Cost,State> entry{std::numeric_limits<Cost>::max(),connected.second};
        }
        // TODO: Store the cost-so-far in the State (so we don't have to handle this pair<Cost,State>)?
    //        if: "current" "lowest cost so far" + cost to "connected" < "connected" "lowest cost so far"
        if (lowest_cost_so_far[current].first+connected.first < lowest_cost_so_far[connected.second].first) {
    //          then: update "connected" "lowest cost so far" to "current" "lowest cost so far" + cost to "connected"
          lowest_cost_so_far[connected.second].first = lowest_cost_so_far[current].first+connected.first;
    //          and: update "connected" "from" to current
          lowest_cost_so_far[connected.second].second = current;
        }
      } //      end-of-each-in-frontier
      //      ensure all vertecies in "frontier" are present in "unvisited" (if not done initially = we generate the graph as we step)
      for (auto const& connected : frontier) {
        unvisited.push(connected.second); // TODO: Store the cost-so-far in the State?
        // Note: We insert them here after they have been updated with the cost so far to have them
        // correctly ordered by cost in the queue
      }
    //      put "current" in "visited"
      visited.insert(current);
    } //    end-of-while "unvisitied"
    //    The map "cost_so_far" now maps "End" to the lowest cost to reach it from Start :)
    // TODO: We expect the unvisited to contain the end node (as we break the loop when visitied is 1 in length)
    if (unvisited.size() != 1) {
      std::cout << "\nERROR - unvisited.size() is " << unvisited.size();
    }
    else {
      auto result = lowest_cost_so_far[unvisited.top()].first;
      std::cout << "\nLowest cost " << result;
    }
    
    
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
    for (auto const& pod : init_state.ap_trackers) {
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
    for (auto const& ap_tracker : current.ap_trackers) {
      current_map.floor_plan[ap_tracker.pos.row][ap_tracker.pos.col] = ap_tracker.type;
    }

    int pix = 0;
    auto pod = current.ap_trackers[pix];
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
  if (true) {
    // Try to eshaust an expansion step by step from a pos position until
    // all alterantives are blocked (flood fill using step rules)

    auto current = init_state;
    
    // Create a map with all the amphipods in their current lcoations
    Burrow current_map{burrow};
    for (auto const& ap_tracker : current.ap_trackers) {
      current_map.floor_plan[ap_tracker.pos.row][ap_tracker.pos.col] = ap_tracker.type;
    }
    // Choose pod 0
    int pix = 0;
    auto cp = current.ap_trackers[pix];
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
}
