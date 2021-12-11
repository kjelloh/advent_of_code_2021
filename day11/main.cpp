#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

extern char const* pTest0;
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

class Octupussy {
public:
  Octupussy(int energy) : m_energy{energy} {}
  int energy() const {return m_energy;}
  std::string to_string() const {return std::to_string(m_energy);}
private:
  int m_energy;
};

class Grid {
public:
  using Row = std::vector<Octupussy>;
  using Octupussies = std::vector<Row>;
  Grid(In in) {
    for (auto const& e : in) {
      Row row{};
      for (auto const& energy : e) {
        row.push_back(energy-'0');
      }
      m_octupussy.push_back(row);
    }
  }
  std::string to_string() {
    std::string result{};
    for (auto const& row : m_octupussy) {
      result += "\n";
      for (auto const& oct : row) {
        result += " ";
        result += oct.to_string();
      }
    }
    return result;
  }

private:
  Octupussies m_octupussy;
};

namespace part1 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto in_data = parse(in);
      Grid grid{in_data};
      std::cout << grid.to_string();
      return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto in_data = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test small",part1::solve_for(pTest0)});
  // answers.push_back({"Part 1 Test large",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\nPress <enter>...";
  std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest0 = R"(11111
19991
19191
19991
11111)";
char const* pExample1 = R"(Before any steps:
11111
19991
19191
19991
11111

After step 1:
34543
40004
50005
40004
34543

After step 2:
45654
51115
61116
51115
45654)";
char const* pExample2 = R"(Before any steps:
5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526

After step 1:
6594254334
3856965822
6375667284
7252447257
7468496589
5278635756
3287952832
7993992245
5957959665
6394862637

After step 2:
8807476555
5089087054
8597889608
8485769600
8700908800
6600088989
6800005943
0000007456
9000000876
8700006848

After step 3:
0050900866
8500800575
9900000039
9700000041
9935080063
7712300000
7911250009
2211130000
0421125000
0021119000

After step 4:
2263031977
0923031697
0032221150
0041111163
0076191174
0053411122
0042361120
5532241122
1532247211
1132230211

After step 5:
4484144000
2044144000
2253333493
1152333274
1187303285
1164633233
1153472231
6643352233
2643358322
2243341322

After step 6:
5595255111
3155255222
3364444605
2263444496
2298414396
2275744344
2264583342
7754463344
3754469433
3354452433

After step 7:
6707366222
4377366333
4475555827
3496655709
3500625609
3509955566
3486694453
8865585555
4865580644
4465574644

After step 8:
7818477333
5488477444
5697666949
4608766830
4734946730
4740097688
6900007564
0000009666
8000004755
6800007755

After step 9:
9060000644
7800000976
6900000080
5840000082
5858000093
6962400000
8021250009
2221130009
9111128097
7911119976

After step 10:
0481112976
0031112009
0041112504
0081111406
0099111306
0093511233
0442361130
5532252350
0532250600
0032240000)";
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
