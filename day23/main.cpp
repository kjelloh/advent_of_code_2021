//
//  main.cpp
//  day23
//
//  Created by Kjell-Olov Högdal on 2021-12-23.
//
#include <iostream>
#include <deque>
#include <array>
#include <optional>
#include <algorithm>
#include <vector>
#include <limits>
#include <cmath>

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
using Cost = Result;
using Burrow = std::vector<std::string>;
struct Amphipod {
  char type;
  int row;
  int col;
};
using Amphipods = std::vector<Amphipod>;

bool is_ordered(Amphipods const& amphipods) {
  return true;
}
int step_cost(char type) {
  int result=1;
  for (int i=0;i<'A'-type;i++) result *=10;
  return result;
}
Cost sort_out(Amphipods const& amphipods,Burrow const& burrow) {
  // Return the lowest cost of ordering provided burrow
  // Deep search (brute force)
  if (is_ordered(amphipods)) return 0;
  else {
    // Examine potential moves
    Burrow map{burrow};
    for (auto const& amphipod : amphipods) {
      map[amphipod.row][amphipod.col] = '#'; // mark as occupied on map
    }
    std::vector<std::pair<Cost,Amphipods>> frontiere{};
    for (auto const& amphipod : amphipods) {
      // An amphipod in a room can go out in the hallway
      // An amphipod in the hallway can go back into its room with or withouts it correct room mate
      int move_cost{0};
      if (amphipod.row == 1) {
        // In hallway, try moving back to correct room
        /*
         Amphipods will never move from the hallway into a room unless that room is their destination room and that room contains no amphipods which do not also have that room as their own destination. If an amphipod's starting room is not its destination room, it can stay in that room until it leaves the room. (For example, an Amber amphipod will not move from the hallway into the right three rooms, and will only move into the leftmost room if that room is empty or if it only contains other Amber amphipods.)
         */
        int room_col = 3 + static_cast<int>(amphipod.type-'A')*2; // room column 3,5,7,9
      }
      else {
        // in room, try all moves out of room
        if (!std::any_of(amphipods.begin(), amphipods.end(), [&amphipod](Amphipod const& other){
          return (amphipod.row-1 == other.row and amphipod.col == other.col);
        })) {
          // Free to move out into the hallway
          move_cost += step_cost(amphipod.type) * (amphipod.row-1);
        }
        auto doorway_cost{move_cost}; // cash cost to doorway
        // move all possible ways left and push cost and constellation as candidate for future moves
        move_cost += step_cost(amphipod.type); // move left (can't stand in doorway)
        for (int i=amphipod.col-1;map[1][i]==' ';i--) {
          move_cost += step_cost(amphipod.type);
          // CONVOLUTED! We need to move amphipod in amphipods and use for new candidate to frontiere...
        }
        // move all possibel ways right and push cost and constellation as candidate for future moves
      }
      /*
       Amphipods will never stop on the space immediately outside any room. They can move into that space so long as they immediately continue moving. (Specifically, this refers to the four open spaces in the hallway that are directly above an amphipod starting position.)
       */
      /*
       Once an amphipod stops moving in the hallway, it will stay in that spot until it can move into a room. (That is, once any amphipod starts moving, any other amphipods currently in the hallway are locked in place and will not move again until they can move fully into a room.)
       */

    }
    Result lowest_cost{std::numeric_limits<Cost>::max()};
    for (auto const& [cost,amphipods] : frontiere) {
      lowest_cost = cost + sort_out(amphipods, burrow);
    }
    return lowest_cost;
  }
}

int main(int argc, const char * argv[]) {
  {
    // puzzle example
    /*
     ###B#C#B#D###
       #A#D#C#A#
     */
    Burrow burrow{
       "#############"
      ,"#           #"
      ,"### # # # ###"
      ,"  # # # # #  "
      ,"  #########  "};
    Amphipods amphipods{};
    amphipods.push_back(Amphipod{'B',2,3});
    amphipods.push_back(Amphipod{'A',3,3});
    amphipods.push_back(Amphipod{'C',2,5});
    amphipods.push_back(Amphipod{'D',3,5});
    amphipods.push_back(Amphipod{'B',2,7});
    amphipods.push_back(Amphipod{'C',3,7});
    amphipods.push_back(Amphipod{'D',2,9});
    amphipods.push_back(Amphipod{'A',3,9});
  }
  return 0;
}
