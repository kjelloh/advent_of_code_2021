#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <map>

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using OceanFloor = std::vector<std::string>;
using Model = std::vector<OceanFloor>;

Model parse(auto& in) {
    Model result{};
    std::string line{};
    result.push_back({});
    while (std::getline(in,line)) {
      if (line.find(".") == std::string::npos) {
        // Not an ocean floor row
        // We are parsing an example with a sequence of example floors
        // initiate a new floor unless we already have
        if (result.back().size()>0) result.push_back({});
      }
      else {
        // An ocean floor row
        // Add it to the current ocean floor at the back of result
        result.back().push_back(line);
      }
    }
    return result;
}

struct Pos {
  size_t row, col;
  bool operator<(Pos const& other) const {
    return (row != other.row) ? row < other.row : col < other.col;
  }
};
using Transform = std::map<Pos, Pos>;

Result solve(OceanFloor& current_floor) {
  Result result{ 0 };
  auto max_row = current_floor.size() - 1;
  auto max_col = current_floor[0].size() - 1;

  std::pair<Transform, Transform> next_transform{};
  int step_count{ 0 };
  do {
    next_transform = {};
    // transform position of '>'
    for (size_t row = 0; row <= max_row; row++) {
      for (size_t col = 0; col <= max_col; col++) {
        if (current_floor[row][col] == '>') {
          auto next_col = (col == max_col) ? 0 : col + 1; // Wrap around
          if (current_floor[row][next_col] == '.') {
            next_transform.first[Pos{ row, col }] = { row,next_col };
          }
        }
      }
    }
    // Transform position of 'v'
    for (size_t row = 0; row <= max_row; row++) {
      for (size_t col = 0; col <= max_col; col++) {
        if (current_floor[row][col] == 'v') {
          auto next_row = (row == max_row) ? 0 : row + 1; // Wrap around
          if (current_floor[next_row][col] == '.') {
            // may be moved-to from the left >?
            auto prev_col = (col == 0) ? max_col : col - 1;
            if (next_transform.first.contains(Pos{ next_row,prev_col }) == false) {
              next_transform.second[{row, col}] = { next_row,col };
            }
          }
          else {
            // May be moved from occupying > moving right?
            if (next_transform.first.contains(Pos{ next_row,col }) == true) {
              next_transform.second[{row, col}] = { next_row,col };
            }
          }
        }
      }
    }
    // Apply next transform
    for (auto const& transform : { next_transform.first,next_transform.second }) {
      for (auto const& me : transform) {
        if (current_floor[me.second.row][me.second.col] == '.') {
          current_floor[me.second.row][me.second.col] = current_floor[me.first.row][me.first.col];
          current_floor[me.first.row][me.first.col] = '.';
        }
        else {
          std::cout << "\nERROR - Move failed. Not free {row:" << me.second.row << ".col:" << me.second.col << "}";
        }
      }
    }
    step_count++;
  } while (next_transform.first.size() > 0 or next_transform.second.size() > 0);
  result = step_count;
  return result;
}

namespace part1 {
  Result solve_for(std::string const file_name) {
      Result result{};
      auto current_path = std::filesystem::current_path();
      // log
      if (false) {
        std::cout << "\n" << current_path;
      }
      std::filesystem::path path{ file_name };
      if (current_path.filename() == "vs") path = std::filesystem::path{".."} / path; // Visual Studio Build
      std::cout << "\npath:" << path;
      std::ifstream in{ path };
      auto puzzle_model = parse(in);
      // log
      if (false) {
        for (auto const& floor : puzzle_model) {
          std::cout << "\n<floor>";
          for (auto const& row : floor) {
            std::cout << "\nrow:" << row;
          }
        }
      }
      auto current_floor = puzzle_model.front();
      auto max_row = current_floor.size() - 1;
      auto max_col = current_floor[0].size() - 1;
      if (puzzle_model.size() == 1) {
        // Actual puzzle
        result = solve(puzzle_model.front());
      }
      else if (puzzle_model.size() > 1) {
        // We have a sequence of example floors to test against 
        int ex_floor_index{0};

        // Expect the number of possible moves are much smaller than the map size?
        // Operate on a set of moves instead of the actual map?
        // 1. create a set of moves for >
        // 2. expand with a set of moves for v (based on map + moves)
        // Transform the floor from left to right and up to down.
        // In this way we can look on the map to see if a > can move (next to left is free).
        // And we can look into the future for v to see if any > will move from or move to a spot on the map 
        
          // Log
        {
          std::cout << "\nInitial state ";
          for (auto const& row : current_floor) {
            std::cout << "\n" << row;
          }
        }
        std::pair<Transform,Transform> next_transform{};
        int step_count{ 0 };
        do {
          next_transform = {};
          // transform position of '>'
          for (size_t row = 0; row <= max_row; row++) {
            for (size_t col = 0; col <= max_col; col++) {
              if (current_floor[row][col] == '>') {
                auto next_col = (col == max_col) ? 0 : col + 1; // Wrap around
                if (current_floor[row][next_col] == '.') {
                  next_transform.first[Pos{row, col}] = { row,next_col };
                }
              }
            }
          }
          // Transform position of 'v'
          for (size_t row = 0; row <= max_row; row++) {
            for (size_t col = 0; col <= max_col; col++) {
              if (current_floor[row][col] == 'v') {
                auto next_row = (row == max_row) ? 0 : row + 1; // Wrap around
                if (current_floor[next_row][col] == '.') {
                  // may be moved-to from the left >?
                  auto prev_col = (col == 0) ? max_col : col - 1;
                  if (next_transform.first.contains(Pos{ next_row,prev_col }) == false) {
                    next_transform.second[{row, col}] = { next_row,col };
                  }
                }
                else {
                  // May be moved from occupying > moving right?
                  if (next_transform.first.contains(Pos{ next_row,col}) == true) {
                    next_transform.second[{row, col}] = { next_row,col };
                  }
                }
              }
            }
          }
          // Apply next transform
          for (auto const& transform : { next_transform.first,next_transform.second }) {
            for (auto const& me : transform) {
              if (current_floor[me.second.row][me.second.col] == '.') {
                current_floor[me.second.row][me.second.col] = current_floor[me.first.row][me.first.col];
                current_floor[me.first.row][me.first.col] = '.';
              }
              else {
                std::cout << "\nERROR - Move failed. Not free {row:" << me.second.row << ".col:" << me.second.col << "}";
              }
            }
          }
          step_count++;
          // Log
          {
            std::cout << "\nafter step " << step_count;
            for (auto const& row : current_floor) {
              std::cout << "\n" << row;
            }
          }
          {
            bool example_expected{ false };
            switch (step_count) {
              case 1:
              case 2:
              case 3:
              case 4:
              case 5: example_expected = true;  break;
              case 10: example_expected = true; break;
              case 20: example_expected = true; break;
              case 30: example_expected = true; break;
              case 40: example_expected = true; break;
              case 50: example_expected = true; break;
              case 55: example_expected = true; break;
              case 56: example_expected = true; break;
              case 57: example_expected = true; break;
              case 58: example_expected = true; break;
            }
            if (example_expected) {
              ex_floor_index++;
              if (ex_floor_index >= puzzle_model.size()) {
                std::cout << "\nFAILED - Exhausted test input at count " << ex_floor_index;
                break;
              }
              else if (current_floor == puzzle_model[ex_floor_index]) {
                std::cout << "\npassed";
              }
              else {
                std::cout << "\nFAILED";
                for (size_t row = 0; row <= max_row; row++) {
                  for (size_t col = 0; col <= max_col; col++) {
                    if (current_floor[row][col] != puzzle_model[ex_floor_index][row][col]) {
                      std::cout << "\n\t{row:" << row << ",col:" << col << "} != " << puzzle_model[ex_floor_index][row][col];
                    }
                  }
                }
                break;
              }
            }
          }
        } while (next_transform.first.size()>0 or next_transform.second.size()>0);
        result = step_count;
      }

      return result;
  }
}

namespace part2 {
  Result solve_for(std::string const file_name) {
    Result result{};
    std::ifstream in{ file_name };
    auto data_model = parse(in);
    return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  //answers.push_back({"Part 1 Test",part1::solve_for("day25.ex")});
  answers.push_back({ "Part 1 Test",part1::solve_for("day25.ex2") });
  answers.push_back({ "Part 1 Test",part1::solve_for("day25.in") });
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\nPress <enter>...";
  std::cin.get();
  std::cout << "\n";
  return 0;
}