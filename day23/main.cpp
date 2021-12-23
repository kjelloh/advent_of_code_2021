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
Cost sort_out(Amphipods const& amphipods,Burrow const& burrow) {
  // Return the lowest cost of ordering provided burrow
  // Deep search (brute force)
  if (is_ordered(amphipods)) return 0;
  else {
    // Examine potential moves
    std::vector<std::pair<Cost,Amphipods>> frontiere{};
    for (auto const& amphipod : amphipods) {
      for (auto dr : {-1,0,1}) {
        for (auto dc : {-1,0,1}) {
          auto rr = amphipod.row+dr;
          auto cc = amphipod.col+dc;
          // Is this a valid move?
          // I.e.,
          // *) Is it a wall?
          // *) Is it occupied?
          // *) Can we stay here (not in doorway to a room)?
          // Then --> push cost for the move and new antipods constellation as candidate into frontiere
        }
      }
      /*
       Amphipods will never stop on the space immediately outside any room. They can move into that space so long as they immediately continue moving. (Specifically, this refers to the four open spaces in the hallway that are directly above an amphipod starting position.)
       */
      /*
       Amphipods will never move from the hallway into a room unless that room is their destination room and that room contains no amphipods which do not also have that room as their own destination. If an amphipod's starting room is not its destination room, it can stay in that room until it leaves the room. (For example, an Amber amphipod will not move from the hallway into the right three rooms, and will only move into the leftmost room if that room is empty or if it only contains other Amber amphipods.)
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
