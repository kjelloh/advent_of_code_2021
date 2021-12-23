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

int main(int argc, const char * argv[]) {
  {
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
    struct State {
      struct AmphipodTracker {
        char type;
        int row;
        int col;
        bool operator<(AmphipodTracker const& other) const { // to work with std::map/set
          for (int i=0;i<3;i++) {
            switch (i) {
              case 0: if (type != other.type) return type<other.type;
              case 1: if (row != other.row) return row<other.row;
              case 2: if (col != other.col) return col<other.col;
            }
          }
          return false; // this==other
        }
      };
      std::vector<AmphipodTracker> ap_trackers{};
      bool operator<(State const& other) const {return ap_trackers<other.ap_trackers;} // to work with std::map/set
    };
    // 2. A lowest_cost_so_far that maps a State to the pair (initiate new mappings as we process the frontier through the graph)
    using Cost = size_t;
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
    struct Burrow {
      std::vector<std::string> floor_plan{};
      size_t corridor_ix_left{};
      size_t corridor_ix_right{};
      std::array<int,4> room_column_ix{};
      size_t room_height{};
      State end_state() {
        // cretae a State with 'A','B','C' and 'D' in each of the rooms left to right
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
    // 5. Now we can create the Burrow and the init state from the input
    State init_state{};
    Burrow burrow{};
    std::istringstream in{pTest};
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
              ap_tracker.row = burrow_row;
              ap_tracker.col = ix;
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
    // Print
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
    // Print
    if (true) {
      std::cout << "\n<Init State>";
      for (auto const& ap_tracker : init_state.ap_trackers) {
        std::cout << "\n\t" << ap_tracker.type << "{row:" << ap_tracker.row << ",col:" << ap_tracker.col << "}";
      }
    }
    // OK, lets find the cheapest (shortest) path
    // Prerequisite: A defined start vertex "Start" and a defined end vertex "End".
    auto end_state = burrow.end_state();
    // Print
    if (true) {
      std::cout << "\n<End State>";
      for (auto const& ap_tracker : end_state.ap_trackers) {
        std::cout << "\n\t" << ap_tracker.type << "{row:" << ap_tracker.row << ",col:" << ap_tracker.col << "}";
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
          current_map.floor_plan[ap_tracker.row][ap_tracker.col] = ap_tracker.type;
        }
        for (auto const& ap_tracker : current.ap_trackers) {
          // find all possible moves of this amohipod type and current location on the current map
          // Exhaust the reachable positions from current ap_tracker position
          std::vector<std::pair<int,int>> reachable{};
          std::set<std::pair<int,int>> visited{};
          auto row = ap_tracker.row;
          auto col = ap_tracker.col;
          if (row==1) {
            // In the corridor ==> valid move is back to the correct room with a mate of the same type
            // A type 'X' to room column ix = with room_column_ix['X'-'A']
            // Any mate already in the same column (room) must be of the same correct type
          }
          else {
            // In a room ==> try moving out in the corridor
            // We can move to any side of the room entrance and then to the left and right until stopped by wall or occupied column
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
