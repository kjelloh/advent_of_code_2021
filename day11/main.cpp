#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

char const* pTest0 = R"(11111
19991
19191
19991
11111)";
extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using In = std::vector<std::string>;

In parse(auto& in) {
    In result{};
    std::string line{};
    while (in >> line) {
        result.push_back(line);
    }
    return result;
}

void flash(auto& grid, int r,int c) {
  std::cout << "\nflash:{" << r << "," << c << "}=> ++ " ;
  grid[r][c] = ' '; // mark as "flashed"
  for (int dr : {-1,0,1}) {
    for (int dc : {-1,0,1}) {
      if (r+dr >= 0 and r+dr < grid.size() and c+dc>=0 and c+dc < grid[0].size() and grid[r+dr][c+dc]!=' ') {
        ++grid[r+dr][c+dc];
        std::cout << "{" << r+dr << "," << c+dc << "}";
      }
    }
  }
}

void for_each(auto& grid, auto f) {
  for (int r=0;r<grid.size();r++) {
    for (int c=0; c<grid[0].size();c++) {
      f(r,c);
    }
  }
}

namespace part1 {
  Result solve_for(char const* pData,int const LOOP_COUNT) {
      Result result{};
      std::stringstream in{ pData };
      auto grid = parse(in);
      int flash_count{0};
      int step{0};
      while (true) {
        {
          // increment
          for_each(grid,[&grid](int r,int c){
            ++grid[r][c];
          });
          // flash until no more to flash
          while (true) {
            int cashed_flash_count{flash_count};
            // find all == 9+1 and "flash"
            for_each(grid,[&grid,&flash_count](int r,int c){
              if (grid[r][c] > '9') {
                // energy > '9' "to flash" and '0'-1 as "already flashed"
                flash(grid,r,c);
                ++flash_count;                  
              }
            });
            if (flash_count==cashed_flash_count) break;      
          }
          // print flashed grid (before resetting flashed to '0')
          // std::cout << "\n";
          // for_each(grid,[&grid](int r,int c){
          //     std::cout << " " << grid[r][c];
          // });
          // set energy of flashed to 0
          for_each(grid,[&grid](int r,int c){
              grid[r][c] = (grid[r][c] ==' ')?'0':grid[r][c];
          });
          // print flashed grid (after resetting flashed to '0')
          // std::cout << "\n";
          // for_each(grid,[&grid](int r,int c){
          //     std::cout << " " << grid[r][c];
          // });
        }
        // do loop checks (break if done)
        ++step;
        std::cout << "\nafter step: " << step << " flash_count: " << flash_count;
        if (step<LOOP_COUNT) continue;
        result = flash_count;
        break;
      }
      return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto grid = parse(in);
      int flash_count{0};
      int step{0};
      while (true) {
        int cashed_flash_count{flash_count};
        {
          // increment
          for_each(grid,[&grid](int r,int c){
            ++grid[r][c];
          });
          // flash until no more to flash
          while (true) {
            int cashed_flash_count{flash_count};
            // find all == 9+1 and "flash"
            for_each(grid,[&grid,&flash_count](int r,int c){
              if (grid[r][c] > '9') {
                // energy > '9' "to flash" and '0'-1 as "already flashed"
                flash(grid,r,c);
                ++flash_count;                  
              }
            });
            if (flash_count==cashed_flash_count) break;      
          }
          // print flashed grid (before resetting flashed to '0')
          // std::cout << "\n";
          // for_each(grid,[&grid](int r,int c){
          //     std::cout << " " << grid[r][c];
          // });
          // set energy of flashed to 0
          for_each(grid,[&grid](int r,int c){
              grid[r][c] = (grid[r][c] ==' ')?'0':grid[r][c];
          });
          // print flashed grid (after resetting flashed to '0')
          // std::cout << "\n";
          // for_each(grid,[&grid](int r,int c){
          //     std::cout << " " << grid[r][c];
          // });
        }

        ++step;
        std::cout << "\nafter step: " << step << " flash_count: " << flash_count;
        if ((flash_count - cashed_flash_count)<100) continue;
        result = step;
        break;
      }
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test small 2 steps",part1::solve_for(pTest0,2)});
  answers.push_back({"Part 1 Test full 10 steps",part1::solve_for(pTest,10)});
  answers.push_back({"Part 1 Test full 100 steps",part1::solve_for(pTest,100)});
  answers.push_back({"Part 1     ",part1::solve_for(pData,100)});
  answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526)";
char const* pData = R"(8258741254
3335286211
8468661311
6164578353
2138414553
1785385447
3441133751
3586862837
7568272878
6833643144)";
