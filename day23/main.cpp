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
      };
      std::vector<AmphipodTracker> ap_trackers{};
    };
    // 2. A lowest_cost_so_far that maps a State to the pair
    using Cost = size_t;
    std::map<State,Cost> lowest_cost_so_far{};
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
    };
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
    {
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
    {
      std::cout << "\n<Init State>";
      for (auto const& ap_tracker : init_state.ap_trackers) {
        std::cout << "\n\t" << ap_tracker.type << "{row:" << ap_tracker.row << ",col:" << ap_tracker.col << "}";
      }
    }
  }
  std::cout << "\n";
  return 0;
}
