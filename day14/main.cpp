#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <map>
#include <numeric>
#include <stdint.h>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;
struct Transform {
    std::pair<std::string, char> rule;
};
struct Model {
    std::string polymer_template;
    std::vector<Transform> transforms;
};
std::pair<std::string, std::string> split(std::string const& line,std::string const& delim) {
    auto pos = line.find(delim);
    auto left = line.substr(0,pos);
    auto right = line.substr(pos + delim.size());
    return { left,right };
}
Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in,line)) {
        // std::cout << "\nline " << line;
        if (line.size() > 0) {
            if (line.find("->") == std::string::npos) {
                result.polymer_template = line;
                std::cout << "\ntemplate:" << line;
            }
            else {
                auto [pair, element] = split(line, " -> ");
                std::cout << "\n{pair:" << pair << ",element:" << element << "}";
                result.transforms.push_back({ {pair,element[0]} });
            }
        }
    }
    return result;
}
std::string to_log_string(std::string const& polymer) {
    const size_t TRUNCATE_SIZE = 40;
    const size_t TAIL_SIZE = 10;
    auto polymer_size = polymer.size();
    std::string result{polymer.substr(0,TRUNCATE_SIZE)};
    if (polymer_size > TRUNCATE_SIZE) {
        result += "...";
        size_t tail_count = std::min(TAIL_SIZE, polymer_size-TRUNCATE_SIZE-3);
        result += polymer.substr(polymer_size-tail_count);
    }
    return result;
}
namespace part1 {
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto puzzle_model = parse(in);
    std::cout << "\ntemplate:" << puzzle_model.polymer_template;
    for (auto const& transform : puzzle_model.transforms) {
        std::cout << "\n{key:" << transform.rule.first << ",insert:" << transform.rule.second;
    }
    std::string polymer{ puzzle_model.polymer_template };
    for (int step = 1; step <= 10;step++) {
        std::string transformed_polymer{polymer[0]};
        for (int pos = 1; pos < polymer.size(); pos++) {
            std::string key{ polymer.substr(pos-1,2) };
            for (auto const& transform : puzzle_model.transforms) {
                if (key == transform.rule.first) {
                    transformed_polymer += transform.rule.second;
                    break;
                }
            }
            transformed_polymer += polymer[pos];
        }
        polymer = transformed_polymer;
        std::cout << "\nAfter step " << step << "\t: [" << polymer.size() << "]" << to_log_string(polymer);
    }
    using Counts = std::map<char, size_t>;
    auto counts = std::accumulate(polymer.begin(), polymer.end(), Counts{}, [](auto acc,char ch) {
        ++acc[ch];
        return acc;
    });
    using MaxMinFreq = std::pair<size_t, size_t>;
    MaxMinFreq init{0,SIZE_MAX };
    auto [max_freq, min_freq] = std::accumulate(counts.begin(), counts.end(), init, [](auto acc,auto entry) {
        acc.first = std::max(acc.first,entry.second);
        acc.second = std::min(acc.second, entry.second);
        return acc;
    });
    std::cout << "\nmax:" << max_freq << " min:" << min_freq;
    result = max_freq - min_freq;
    return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)";
char const* pData = R"(CHBBKPHCPHPOKNSNCOVB

SP -> K
BB -> H
BH -> S
BS -> H
PN -> P
OB -> S
ON -> C
HK -> K
BN -> V
OH -> F
OF -> C
SN -> N
PF -> H
CF -> F
HN -> S
SK -> F
SS -> C
HH -> C
SO -> B
FS -> P
CB -> V
NK -> F
KK -> P
VN -> H
KF -> K
PS -> B
HP -> B
NP -> P
OO -> B
FB -> V
PO -> B
CN -> O
HC -> B
NN -> V
FV -> F
BK -> K
VC -> K
KV -> V
VF -> V
FO -> O
FK -> B
HS -> C
OV -> F
PK -> F
VV -> S
NH -> K
SH -> H
VB -> H
NF -> P
OK -> B
FH -> F
CO -> V
BC -> K
PP -> S
OP -> V
VO -> C
NC -> F
PB -> F
KO -> O
BF -> C
VS -> K
KN -> P
BP -> F
KS -> V
SB -> H
CH -> N
HF -> O
CV -> P
NB -> V
FF -> H
OS -> S
CS -> S
KC -> F
NS -> N
NV -> O
SV -> V
BO -> V
BV -> V
CC -> F
CK -> H
KP -> C
KH -> H
KB -> F
PH -> P
VP -> P
OC -> F
FP -> N
HV -> P
HB -> H
PC -> N
VK -> H
HO -> V
CP -> F
SF -> N
FC -> P
NO -> K
VH -> S
FN -> F
PV -> O
SC -> N)";
