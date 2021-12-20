#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <set>
#include <fstream>

// Same as pTest BUT index 0 is # and index 512 is .
// This mimics the bevagious of puzzle data where the pixles in the infinite area
// will toggle between # and . ;)
char const* pTest2 = R"(#.#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#...

#..#.
#....
##..#
..#..
..###)";
extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using Coordinate = int;
using Position = std::vector<int>;

class Image {
public:
  int pixel_count() {return m_pixels.size();}
  std::pair<Position,Position> const& frame() const {return m_frame;};
  bool pixel_is_bright(Position const& pixel) const {
    if (is_infinite_position(pixel)) {
      return m_infinite_pixel_is_bright;
    }
    else {
      if (m_pixels.find(pixel) != m_pixels.end()) return true;
      return false;
    }
  }
  Image const& insert(Position const& pixel) {
    // std::cout << "\ninsert {" << pixel[0] << "," << pixel[1] << "}";
    m_pixels.insert(pixel);
    m_frame.first[0] = std::min(m_frame.first[0],pixel[0]);
    m_frame.second[0] = std::max(m_frame.second[0],pixel[0]);
    m_frame.first[1] = std::min(m_frame.first[1],pixel[1]);
    m_frame.second[1] = std::max(m_frame.second[1],pixel[1]);
    // std::cout << "\nframe:{" << m_frame.first[0] << "," << m_frame.first[1] << "}"
    // << "-> {" << m_frame.second[0] << "," << m_frame.second[1] << "}";
    return *this;
  }
  bool is_infinite_position(Position const& pixel) const {
    if (pixel[0] < (m_frame.first[0]) or pixel[0] > (m_frame.second[0])) return true;
    if (pixel[1] < (m_frame.first[1]) or pixel[1] > (m_frame.second[1])) return true;
    return false;
  }
  Position infinite_position() const {
    return {m_frame.first[0]-2,m_frame.first[1]-2}; // guaranteed to be in "deep space" (outside expanding frame)
  }
  void set_infinite_pixel(bool is_bright_pixel) {
    m_infinite_pixel_is_bright = is_bright_pixel;
  }
private:
  std::set<Position> m_pixels{};
  std::pair<Position,Position> m_frame{{0,0},{0,0}};
  bool m_infinite_pixel_is_bright{false};
};

struct Model {
  std::bitset<512> algorithm{};
  Image image{};
};

Model parse(auto& in) {
    Model result{};
    std::string line{};
    int state{0};
    Coordinate x{0},y{0};
    Position ul{0,0},lr{0,0};
    while (in >> line) {
      std::cout << "\nin:" << line;
      if (line.size()>0) {
        switch (state) {
          case 0: {
            std::string bs{};
            std::transform(line.begin(),line.end(),std::back_inserter(bs),[](char ch) {return (ch=='#')?'1':'0';});
            result.algorithm = std::bitset<512>(bs);
            std::cout << "\nalgorithm = " << result.algorithm.to_string();
            state=1;
          }
          break;
          case 1: {
            // x horizontal +right, y vertical + down
            for (char ch : line) {
              if (ch=='#') {
                // std::cout << "\n{" << x << "," << y << "} " << ch;
                result.image.insert(Position{x,y});
              }
              ++x;
            }
            x=0;
            ++y;
          }
          break;
        }
      }
    }
    return result;
}

void print_image(Image const& image) {
  // Print image inside area upper-left corner to lower right corner
  const int X_BORDER = 4;
  const int Y_BORDER = 4;
  auto x_min = image.frame().first[0] - X_BORDER;
  auto y_min = image.frame().first[1] - Y_BORDER;
  auto x_max = image.frame().second[0] + X_BORDER;
  auto y_max = image.frame().second[1] + Y_BORDER;
  std::cout << "\nframe {" << x_min << ","<< y_min << "} -> {" << x_max << "," << y_max << "}";
  for (int y = y_min;y<= y_max;y++) {
    std::cout << "\n";
    for (int x = x_min;x <= x_max ;x++) {
      if (image.pixel_is_bright({x,y})) {
        std::cout << "#";
      }
      else std::cout << '.';
    }
  }
}

Result index(Position const& pixel,Image const& image) {
  Result result;
  std::string bs{};
  std::vector<Position> deltas = {
    {-1,-1}
    ,{0,-1}
    ,{1,-1}
    ,{-1,0}
    ,{0,0} // include origo!
    ,{1,0}
    ,{-1,1}
    ,{0,1}
    ,{1,1}
  };
  for (Position dp : deltas) {
    auto x = pixel[0] + dp[0];
    auto y = pixel[1] + dp[1];
    if (image.pixel_is_bright({x,y})) bs += '1';
    else bs += '0';
  }
  result = std::bitset<9>{bs}.to_ulong();
  // // Log
  // if (true) {
  //   std::cout << "\nindex(" << pixel[0] << "," << pixel[1] << ") = " << bs << " = " << result;;
  // }
  return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto puzzle_model = parse(in);
      std::cout << "\nInitial Image";
      print_image(puzzle_model.image);
      Image image{puzzle_model.image};
      for (int i=1;i<=2;i++) {
        Image next_image{};
        // First set the pixel for all pixels in the infinite area if the image
        auto infinite_ix = index(image.infinite_position(),image);
        next_image.set_infinite_pixel(puzzle_model.algorithm[512-infinite_ix-1]);
        for (int y = image.frame().first[1]-1;y<=image.frame().second[1]+1;y++) {
          for  (int x = image.frame().first[0]-1;x<=image.frame().second[0]+1;x++) {
            auto ix = index(Position{x,y},image);
            if (puzzle_model.algorithm[512-ix-1]) {
              next_image.insert(Position{x,y});
            }
          }
        }
        image = next_image;
        std::cout << "\nafter step: " << i; 
        print_image(image);
      }
      result = image.pixel_count();
      return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto puzzle_model = parse(in);
      // std::cout << "\nInitial Image";
      // print_image(puzzle_model.image);
      Image image{puzzle_model.image};
      for (int i=1;i<=50;i++) {
        Image next_image{};
        // First set the pixel for all pixels in the infinite area if the image
        auto infinite_ix = index(image.infinite_position(),image);
        next_image.set_infinite_pixel(puzzle_model.algorithm[512-infinite_ix-1]);
        for (int y = image.frame().first[1]-1;y<=image.frame().second[1]+1;y++) {
          for  (int x = image.frame().first[0]-1;x<=image.frame().second[0]+1;x++) {
            auto ix = index(Position{x,y},image);
            if (puzzle_model.algorithm[512-ix-1]) {
              next_image.insert(Position{x,y});
            }
          }
        }
        image = next_image;
        // std::cout << "\nafter step: " << i; 
        // print_image(image);
      }
      result = image.pixel_count();
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test   ",part1::solve_for(pTest)});
  // answers.push_back({"Part 1 Test2  ",part1::solve_for(pTest2)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###)";
char const* pData = R"(##...###...##.#..###.#...#....#..###.##.###...###.##.##.####.#######..######....#.###.##....#.##...#..........#####..##..#..##...##....####.###..#..####.##.##.#....#.##.#.#.#.#..##.##.#...##..##...#.#...#...#..#.#...#.##...#..######.#.##..#.#.#...##.###..########...........#.#.###.#.#..##...#..#..###..#..##.#.#...##..##..###..#####...#.####....#.###....###.##.##...##.#..#.#..####....#..##...##.#####.###.###.###.....##...#.#..#.######.##.##.......###...#.....#...##..#..#.###.#..#..########.##......###..##.#.

###.###...##..#.......#...#...#.#.#.#...#.##.#...##...###..#..#.#.#.#...#..#.#...#...#.##.#.#######.
#...##....#......######.##.#####.#...#.#..#####....##.##...#...###.#.###.##.#..#....#.####...#.....#
.#.#..#.###.##.###....##......##.##.###.#...###.##...###...#.###....##.##...##...##....#..##.###.#.#
##.###.#.#####.#######..#...#.#...#.###....#..#.#......####..##.###....#..#....#.#..#.#.#.##...##.#.
#...........#...#..#..###...###.#..........#.###..#....##...#..#.##..#....###.####..###..#.###.....#
.###.........##..##....#...##...#.....###.#....#.##.#.###.##....#.#.....##.###.....###.###....##.#..
#######.#...#...#..####...#.#####.#...#..#.##......#..##...####.##.###....#.###.#.#.###.####.#..#...
.####..######.#...#.#......#.####..#...#####....#...#.###...........##....##..####..#...###....#.#.#
#.#..##.###.###.#..##..#..###..#.#.#.#...#..##..#####..#.##.....##.##.##..#.##...##.##......#..####.
.#####..#.#.###....#.##.##.####.#...##..#...#.#.#.##.##...##...#.#..#.....#.#.##.##.###.#.........##
.####...#.####...#####..##.####.#.#####...##..###.###.##.#....#.#.#.....###.#.#...#....#.##.#....###
.###.##.#.##..#...#..#.#.###..#.#.##....####..##.###.####........##..###.#..####.#..#####...####.###
##.####..##.##.#.#.####..#...#.....#######...###..#####.#...#.#####.#.##..##.#.##.#...###....#.#...#
.####.....#....#...#.#.#.#.#.##...#..#.#####.#..#.#..##.####.###..#..##...#..##.#..#.#..##.....####.
...##.#...#..###.#####..##.###..#.#.#.#...###..#####.#.#...#..######..#..###########......#...#.##.#
#.#.#####..####..#.####..###......##.#.####..#.##.#..#.#.#...#.#..#...##.##....##.#.#..##..##..####.
#...#######.#..#..####.#..#...#.#......#.##.##..####.##..#..##.##.###.#..###..#...#.##.##.#..#.##.#.
.###..#.##.........#.#..###.####.##.##.#..#######..#.##.#.#...#..#....##.#..##..#.#.###.#..##....#.#
##..###...#...##.##..##.#..##.###.##.#.##########.######.##.##.##.##..#......#.####.......#.#..#..#.
..#.##.#..##...#.....##.##..##.#.#..####.#####....#.#####.........##.#..##.#..##...#...###..#.###...
##..#.#.....#.##..##...##...#......##...#.#..##..##..##....#..#.###...###.###.#.#.#....###.##..##..#
.####..##.###...#.....#.#.###..####..#...##.##.##.#.###..##.##.###....#.##.#####.##.#.##.####..###.#
#.###.##..#.#####...##.#.#.####.##.###.##..##..##.##.###.####.....##.###....#...#.##.#######.##.#.##
..#....#...#....##.##..###.#.####.#.#...##.##....#....####.##..#...##.###..#.....#.....####.###.#.##
##.#...##..##...#.#...##.#.#.#..#.##.##.#.#..#.###.###..#...##...#.####...######.#..#......#...##..#
###.#....#.#...#.#.#..#####..#....#...####.#....#...##.#.##...##.##.#.#####.#..##.#...#...#.#..#.#.#
##...#.#..#.##.#.##..#.#.##.###.##..#.###.##..##..##.#..#....#.####.#..#......########.#.###.#.#...#
##.....#..##...##.##.####.#...##..#.##...#.#.##..#..####..##....######.##...##.####.##..#.##...###..
..###.#.##.#.##.#.#......#.#####..#.#.####.###...##..##..##....#.#...#.##.##..###....#.....#####..##
..#......#.#.#....#.#####.##.##.###..##...##.##.#..##.##....##..##....###....#..##.####...#.#.#.##..
.#.....##.#...#.#..##...#.##..#....#..###...#####..#....#.##..#..##.##.####..#.#...##.##...##..##..#
##.##..###...####....#.#####..#.###...#.###.....######..##.##.##..#.#.#..#..#######..#.#..#.#.#....#
###.##.#...#...###.##.#.#.#..#.####.....#..#.##.##...#.####..##...#.##.#...#...###.#.#...#.#.##..##.
######..#...###..#.....##...#..#######.....#...#.#...##.#..#.###.###.#.#.#...##..###.#.###..###.#.#.
#.#.##.#.#.#####.#######.#..##..#####.#.###..###.#.#...#...#.#....##.###.#..#...#####...###..#...#.#
##.#.....##...##.###..#####.####...##.#.#.#........##.######.#....#.#..#######.###..##...#.#..##.#..
######..#..##...#.##.###...####.#####......#..#####.#.#..#.##.#####.#..#...#.#.#...........###..#.#.
.#####.#####...##.#.#.##.#.#.###.#.##.##.##..###..#..#.##.##.......##...##.#..#.#.#.###.#...#..#....
##..#.######....##.###.##...##..#.#.##..#.##...#.###..#.#....#.....##...###.#.###..######..###.##.#.
.#.#.#.#...###.##....###..#.#.#.#.....#.#.#.####.####.###..##...#.#....#.#....##..#.##.##..##.####..
##.##..#.#....#...#.##.##.....#...#.####.####..#..###.####.#...#.###.....#..#.####.#.......#.#...###
#..#..####..#.#..##..#########...#..########..#..######..#..##..###.#####.#..#.#####.#......#..###..
...#.#.....#....#####..##.##.#.##.###.##.#...#...####....#...####....#####.##...#.##...##.#.#..####.
...#...##..#..#######..#######..#..##.#.##...##.##.#..#..######.#.#.##.###.#.###.####..####.####..#.
###.##.#.##..####.#..#.#...###...#..###..##.####...###...#...#####..###..##.#..##...####..#.##.###.#
..##.#.#.#.#..#..#.#...#.#.#..####...#.#.###..##..##..#..#.#.##...##.##.###.##..#.#.#....##...#.##.#
.....#.#..##....#.#..#.##..#.##...#.#...##..#..####.##.#.##.##...#....#.#.####.###...######.##.....#
#.#.#..#......##.......#...#.#...####..######.##.##..#.###.##..##.#...#.###.....###.#....##.#.#..#.#
#..#####....###..####..##.##.#..#.##.##.#..####.##....#...###.##..####..##..#####......#....#.##...#
#..#.#.#.#.#.##..#.##.#.#....###.#....#.#.######.##.#.......#.######..#...#........#..##.##.##.#####
.#####..##.###.##...##...##.##.#...#..#.###..##.##....##.##..#.#..#.##..#.##..........#...#.#..##..#
#...###.#####..###..#.#######..####..##...#.###.###.###..#.##..###....##.##..#...####.####.#.#...#..
##.#....####.#.#.#.#.#..##..##..##..#....#..#...#.#...##.#.###.#.####.#####....#..##..##.#..#.#.##..
.##...####.##.#.#.#.#..#####....##..##..#..####...##..#...#.#.......#.#.#...#.........##..##.##.#...
..#...#..#####.##.......#####.####..#####...#..##.##..#.#...##..#..#.#.....#.##.##.##....##..#.#####
##....#..##.####.#.......#.##.#.#.#...#.##.#.#....########.###...#.######.###.........##.#........##
#...##.##...#..#..##.#.#...#########.##...##.#.####.####......##.#..#.#.#.#..##........#..##.#######
#.###.#..#.##.#.##.#######.##.##.##..###...#.#.....#.##.#.##.###..##..##..##.#.....##.#...#.....##..
.##.#.##..##..##..###.....#...###....#..#..#....#...#.#....###.#.###..#.#####.###..#.##..###..#..#..
.##.##...#.##..#...##..####...###.####.##.#.####..#....####.#..##..#....##...#......#...####.#.#...#
#..#..####..##...#.####...##....##..####....#.#..####.#.#.##...##.#######.###.##.#.....#.####.#.#..#
....#..##.#.#.#.##.#....##..#..###...##....#..###..#########...###.#.#.#.##..##.....###.####.#.....#
.....####.###.#.####..####.#..##....#..#...#..#...###....#.#.#..###.#.###...##..#....#.#.#.#...###..
###.#.#.#..#.##..#.#.#.#..#####.#.####.#..##.....##.#.##.##.#.##..#.##..#..#.#.#..##..##.#......####
.###..####...#..#.#.##...##.#.#..#....##..#.....#.#.#.#..#.#...#...###.#..#.#.....#...#####...###...
#..##.#.###.#..#.#...#....###.###.#.##.##.##.##..##.#..#.#.###.##..#.#....#..##.####.##.####.#.#..#.
#..##.#....#..#.###...#.##.#..#.#...#.#####..#.###......#.#..#.##..###.##.#..#.......########.###.#.
###.##...#..#..#.##.#..#......#.....#####.###....##.......##...####..##.##..#.##....#..#.###.##....#
...#....#.###.##..#..###...##.###..##..#..#...#...####..#.###.#.#..#####.#.####..###..#.###..##..#..
##.#..#..####.#..#.#.#.#.###...###.##..##.##.#.##.#...##..#.#.####..####..##.##.######.#...#.####...
.#....##.##.##.....###...##.##.#.#.#.###...##.##..#.##.###.....##..######....#.#...#.#..####....#.##
.#.######.#.#...###.#.####.####...###.###..#.#...#.....##.....##...###.##....##.####..##.#.##.....#.
#..##.##.#..##.#......###...####..#..###...##.#####.#....#.##..#.#.##.#.#.#....###..##.##.#.###.###.
####.#..#.##..##.#..#.#.##..######..##.#..#..##.#######.####.#.#..####.....##....###.###.###.#..#..#
#..#.###.##..###.##.#..##.##..####.#.....#.....#..#.#.....#......#..#.##..#.#.#......#..##.###.#..##
......#.#.###.#.##..#...#.###.#..##.###.#...###.##..#..#####.##..##.##.##.###.##.######.#.##...#.###
.....#..###.....#.#.##........#.#...###.#.###.###..#.#.##....#..##.#......#.#..#....####.#.#.#..####
##..#....#.##...#####....#.####.##.#.##..###.##...#.#....#..###.#..#..##.#....#..##.#.##..###....#..
....###..##...#.##..#.#..####.##.##..#.....##.##..###...##.#...#.#####....#..###.#..#.#.##.#.####..#
..#..#.....#..####.......#.#..##..#..##########...##..###......#.....#....##..#..##...#....#.#.#..#.
###.#.#....##..##.###...#.#.....#.......##.###.###...........####...##..##.#...####...#.###..#..##.#
.##...#..###...##.#####.####..#####..##.....##..#.#...##.#...##.#.#.#.#...###..#...#..##..#.#..##.#.
.##....#...##..#.##..####.#....#..##.##..#....#######.####...#.#..#...####..#..###.###..##.###.#.#.#
.........##...#.####...#....#...##.###############.#..#.##..####.#.#.#.....##..#.###..###...#..####.
#...####..####.##...####.#.#...####.####..###..#.....#.###.#.##...#...####.#.#.#....##.#...#..##....
#.#.....##.#.####......#.#.##.##.######..#..###...#.....#..#...##.#..##.#..#...#####.###..#.####.###
.##.###.#..##....##.#.#.....##....##..#.#####.######.###.#####.#..#..#####..##....#.###.#####.#####.
#.#...#...#####.###.#....#.#.###.....#.....##...#...#.#....##.##..###..##.####.#....##..#######.###.
....#...###.###.##.##.....#..#.##.##..#..##...#..#....###......#########.#...#.##..#.###.##..##.##.#
#..##...####.###....##..#.#.#.#..####..#.###.#.#####.##.##.####.##....##..#...##.#...#.####....#####
#..#...###..##.####....##..#.#..#.##.####.###..#..###.##..####.##..##.###.####.##......#..#.###...##
#.#....###.#..#.....#..#.##..###.#...#.....#.###..####.##....##.#..##...###.#####....#.##....#..##.#
..#..#..##....#...######.#.##...#......####..#.#....#.##.##..##..#..#..#..##...##..##.#####....#..##
##.#..###.##..#####..##.##.#...#..##.#.#....###.#.###.#......#.#.#....######..###.###.##.####.#.###.
...##.#...####.###.....#########.....#.###..##..........##..####.###.#.##.#.#..##....#...##.###.###.
.######..###..##...#.####.....#.###.####.....####.#.##.#.##.###.#....##.#..#.#.#..##.###...#.#.###..
#..###.#..##.#.##..###..##..#########..#.#......#..##.#..#.##..###.###.#...#....#....###..#.....#..#
##..##.###......#.#....#..##.#...##...###.###.######..#....####.#.####.#..###..###.####.##.##....##.
####.#...#...#####..#...##....####..##.#.#....#####.##......#.#.#...#...#...#..##..#.#...#.#...##.#.
###.#.##....##.#####.###...##..##..#####..#.##..#......#.##...#.#....###...##.##..###...#..#..##.##.)";
