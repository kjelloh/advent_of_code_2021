#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

namespace python_inspired {

  /* Consider https://github.com/jonathanpaulson/AdventOfCode/blob/master/2021/4.py
   import sys

   infile = sys.argv[1] if len(sys.argv)>1 else '4.in'

   # parse input
   numbers = None
   B = []
   F = []
   board = []
   for line in open(infile):
       line = line.strip()
       if numbers is None:
           numbers = [int(x) for x in line.split(',')]
       else:
           if line:
               board.append([int(x) for x in line.split()])
           else:
               if board:
                   B.append(board)
               board = []
   B.append(board)
   ...
   
   */

    /*
        Consider https://github.com/jonathanpaulson/AdventOfCode/blob/master/2021/5.py
        Inspiration for making C++ code that is not so talkative...

import sys
from collections import defaultdict

infile = sys.argv[1] if len(sys.argv)>1 else '5.in'

G1 = defaultdict(int)
G2 = defaultdict(int)
for line in open(infile):
    start,end = line.split('->')
    x1,y1 = start.split(',')
    x2,y2 = end.split(',')
    x1 = int(x1.strip())
    y1 = int(y1.strip())
    x2 = int(x2.strip())
    y2 = int(y2.strip())

    dx = x2-x1
    dy = y2-y1

    for i in range(1+max(abs(dx),abs(dy))):
        x = x1+(1 if dx>0 else (-1 if dx<0 else 0))*i
        y = y1+(1 if dy>0 else (-1 if dy<0 else 0))*i
        if dx==0 or dy==0:
            G1[(x,y)] += 1
        G2[(x,y)] += 1

print(len([k for k in G1 if G1[k]>1]))
print(len([k for k in G2 if G2[k]>1]))
    */
}


namespace tokenize {
char const* pTest_day4 = R"(  7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1   
 
 22 13 17 11  0
  8  2 23  4 24
 21  9 14 16  7
  6 10  3 18  5
  1 12 20 15 19

  3 15  0  2 22
  9 18 13 17  5
 19  8  7 25 23
 20 11 10 24  4
 14 21 16 12  6

 14 21 17 24  4
 10 16 15  9 19
 18  8 23 26 20
 22 11 13  6  5
  2  0 12  3  7)";

char const* pTest_day5 = R"(0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2)";


  // Python> start,end = line.split('->')
  struct Splittable {
    using Splittables = std::vector<Splittable>;
    using T = std::string;
    T token{};
    operator std::string() const {return token;}
    operator char const*() const {return token.c_str();}
    size_t size() {return token.size();}

    Splittable strip() {
      auto first = std::find_if(std::begin(token), std::end(token), [](auto ch) {
        return (std::isspace(ch) == 0);
      });
      if (first!=std::end(token)) {
        auto last = std::find_if(std::rbegin(token), std::rend(token), [](auto ch) {
          return (std::isspace(ch) == 0);
        });
        return {T{first,last.base()}};
      }
      else return {T{}};
    }
    std::pair<Splittable,Splittable> split(T const& delim) const {
      if (auto pos = token.find(delim);pos!=T::npos) {
        std::cout << "\nsplitting " << token << " on " << delim;
        return {{token.substr(0,pos)},{token.substr(pos+delim.size(),token.size()-pos)}};
      }
      else return {{std::string{"#split error# on delim \""} + delim + "\" of " + token},{}};
    }
    std::pair<Splittable,Splittable> split(char delim) const {return split(T{delim});}
    Splittables splits(T const& delim) const {
      Splittables result{};
      auto temp = *this;
      while (temp.size()>0) {
        auto [left,right] = temp.split(delim);
        result.push_back(left);
        temp = right;
      }
      return result;
    }
    Splittables splits(char delim) const {return splits(T{delim});}

  };

  struct Lines {
    using T = Splittable::T;
    using Splittables = Splittable::Splittables;

    static Splittables open(std::istream& in) {
      Splittables result;
      T line{};
      while (std::getline(in, line)) {
        result.push_back(Splittable{line});
      }
      return result;
    }
    static Splittables open(char const* pData) {
      std::stringstream in{pData};
      return open(in);
    }
  };

  void test() {
    { // 2021 day 4
      // Python>for line in open(infile):
      for (auto& line : Lines::open(pTest_day4)) {
        std::cout << "\nline \"" << line << "\"";
        // Python:line = line.strip()
        line = line.strip();
        std::cout << "\nstripped line \"" << line << "\"";
        // Python>numbers = [int(x) for x in line.split(',')]
        auto numbers = line.splits(',');
        for (auto const& number : numbers) {
          std::cout << "|" << number;
        }
      }      
    }
    { // 2021 day 5
      // Python> for line in open(infile):
      for (auto const& line : Lines::open(pTest_day5)) {
        // Python> start,end = line.split('->')
        auto [start,end] = line.split(" -> ");
        // Python> x1,y1 = start.split(',')
        auto [sx1,sy1] = start.split(",");
        std::cout << "\n|" << sx1 << "|" << sy1 << "|";
        // Python>x2,y2 = end.split(',')
        auto [sx2,sy2] = end.split(",");
        std::cout << "\n|" << sx2 << "|" << sy2 << "|";
        // Python>x1 = int(x1.strip())
        auto x1 = std::stoi(sx1);
      }
    }
  }
}

namespace part1 {
  Result solve_for(char const* pData) {
      return {};
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      return {};
  }
}

int main(int argc, char *argv[])
{
  tokenize::test();
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  answers.push_back({"Part 1     ",part1::solve_for(pTest)});
  answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pTest)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\n";
  return 0;
}

char const* pTest = R"()";
char const* pData = R"()";
