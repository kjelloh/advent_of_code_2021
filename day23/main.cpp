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
using Room = std::array<std::optional<char>,2>;
using Rooms = std::array<Room,4>;
using Hallway = std::array<std::optional<char>,11>;
struct Burrow {
  Hallway hallway{};
  Rooms rooms{};
  bool is_ordered() const {
    if (std::any_of(hallway.begin(), hallway.end(), [](auto o){return o;})) return false;
    for (int i=0;i<rooms.size();i++) {
      for (int j=0;j<rooms[0].size();j++) {
        if (rooms[i][j] != static_cast<char>('A'+i)) return false;
      }
    }
    return true;
  }
};

Cost solve(Burrow burrow) {
  // Return the lowest cost of ordering provided burrow
  // Deep search (brute force)
  if (burrow.is_ordered()) return 0;
  else {
    Result lowest_cost{0};
    // Examine potential moves
    std::vector<Burrow> frontiere{};
    /*
     Amphipods will never stop on the space immediately outside any room. They can move into that space so long as they immediately continue moving. (Specifically, this refers to the four open spaces in the hallway that are directly above an amphipod starting position.)
     */
    for (int i=0;i<4;i++) {
      auto hallway_ix = 3+(i*2); // room 0,1,2,3 hallway index 3,5,7,9
      
    }
    /*
     Amphipods will never move from the hallway into a room unless that room is their destination room and that room contains no amphipods which do not also have that room as their own destination. If an amphipod's starting room is not its destination room, it can stay in that room until it leaves the room. (For example, an Amber amphipod will not move from the hallway into the right three rooms, and will only move into the leftmost room if that room is empty or if it only contains other Amber amphipods.)
     */
    /*
     Once an amphipod stops moving in the hallway, it will stay in that spot until it can move into a room. (That is, once any amphipod starts moving, any other amphipods currently in the hallway are locked in place and will not move again until they can move fully into a room.)
     */
    return lowest_cost;
  }
}

int main(int argc, const char * argv[]) {
  if (false) {
    // Test is_ordered
    Room room0{'A','A'};
    Room room1{'B','B'};
    Room room2{'C','C'};
    Room room3{'D','D'};
    Rooms rooms{room0,room1,room2,room3};
    Hallway hallway{};
    Burrow burrow0{hallway,rooms};
    std::cout << "\nis_ordered " << burrow0.is_ordered();
  }
  if (true) {
    // Test puzzle example
    Room room0{'B','A'};
    Room room1{'C','D'};
    Room room2{'B','C'};
    Room room3{'D','A'};
    Rooms rooms{room0,room1,room2,room3};
    Hallway hallway{};
    Burrow burrow0{hallway,rooms};
    auto cost = solve(burrow0);
  }
  return 0;
}
