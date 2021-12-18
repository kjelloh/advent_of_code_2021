#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <memory>
#include <optional>

extern std::vector<char const*> pTests;
extern char const* pData;

struct Result {
  size_t value;
  std::string caption;
};
using Answers = std::vector<std::pair<std::string,Result>>;

struct LeveledNumber {
  int level;
  int value;
};
using SnailFishNumber = std::vector<LeveledNumber>;

//struct SnailFishNumber {
//  std::shared_ptr<SnailFishNumber> left,right;
//};
using SnailFishNumbers = std::vector<SnailFishNumber>;

enum TestType {
  unknown_test
  ,sum_test
  ,undefined_test
};
struct Test {
  int type;
  std::vector<std::string> lines;
};
struct Model {
  std::vector<Test> tests;
  std::vector<std::string> lines;
};

bool contains(std::string const& key,std::string const& token) {
  return (token.find(key) != std::string::npos);
}
bool is_not_test_entries(auto stack) {
  if (contains("=",stack.back())) return false;
  if (contains("becomes",stack.back())) return false;
  return true;
}

Model parse(auto& in) {
  Model result{};
  std::string line{};
  std::vector<std::string> stack{};
  while (std::getline(in,line)) {
      stack.push_back(line);
  }
  if (is_not_test_entries(stack)) {
    std::copy(stack.begin(), stack.end(), std::back_inserter(result.lines));
  }
  else {
    TestType test_type{};
    if (contains("=",stack.back())) {
      test_type=sum_test;
    }
    else if (contains("becomes",stack.back())) {
      std::cout << "\nbecomes test NOT YET PARSED";
    }
    Test test{test_type,{}};
    std::copy(stack.begin(), stack.end(), std::back_inserter(test.lines));
    result.tests.push_back(test);
  }
  return result;
}

size_t magnitude(SnailFishNumber const& number) {
  std::cout << "magnitide NOT YET IMPLEMENTED";
  return 0;
}

class Tree {
public:
  struct Node {
    int index{};
    std::optional<int> value{};
    std::pair<std::shared_ptr<Node>,std::shared_ptr<Node>> childs{};
  };
  void insert(LeveledNumber const& ln){
    // insert a new node for provided number so that
    // the sequence of numbers are preserved.
    // NOTE: We expect this insert to be called in sequence
    // for numbers 0...n
    // NOTE: Teh inserted node MUST be at the depth in the tree
    // as defined by the 'level' property of the provided leveled number.
    // This ensures that tis tree represents the nesting structure of the
    // pairs of the corresponding SnailFishNumber.
    
    // TODO: Implement this insert :)
  }
private:
  std::shared_ptr<Node> root{std::make_shared<Node>()};
};

std::string to_string(SnailFishNumber const& snf) {
  // We need to re-create the binary tree from the vector
  // of leveled values.
  Tree tree{};
  std::string result{};
  for (auto const& ln : snf) {
    tree.insert(ln);
  }
  return result;
}

SnailFishNumber to_snailfish_Number(auto line) {
  SnailFishNumber result{};
  // Parse [[3,4],5] to {{2,3}{2,4}{1,5}} i.e., a flat array of level,value pairs
  int nest_level{0};
  for (auto const& ch : line) {
    if (ch=='[') {++nest_level; continue;}
    if (ch==']') {--nest_level; continue;}
    if (ch==',') continue;
    result.push_back(LeveledNumber{nest_level,ch-'0'});
  }
  return result;
}

SnailFishNumbers to_snailfish_Numbers(auto lines) {
  SnailFishNumbers result{};
  for (auto const& line : lines) {
    result.push_back(to_snailfish_Number(line));
  }
  return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto puzzle_model = parse(in);
    if (puzzle_model.lines.size()==0) {
      std::cout << "\n\tis Test with entry count " << puzzle_model.tests.size();
      result.caption=" NOP";
    }
    else {
      std::cout << "\n\tis Puzzle with entry count " << puzzle_model.lines.size();
      SnailFishNumbers snailfishnumbers{};
      // A snailfish number is a pair p
      // add p1,p2 = [p1,p2]
      // reduce p = explode p* | split n*
      // p* is first level 4 pair in p
      // n* is first number >= 10
      // explode p =
      //            left_of_p.right += p.left
      //            right_of_p.left += p.right
      //            p is replaced by number 0
      // split n = [round_down(n/2),round_up(n/2)]
      
      // We need a datastructure to represent p so that we can
      // add p1,p2 (p can be a std::pair)
      // reduce p  (we need to be able to find level 4 p*, find pair left_of_p and right_of_p)
      // magnitude p (p can be a std::pair)
      
      // But lets try a representation of a number paired with its nest level?
      // [1,2] := {{1,1}{1,2}} i.e., level 1 value 1, level 1 value 2
      // [[3,4],5] := {{2,3}{2,4}{1,5}}
      // The sum becomes {{1,1},{1,2}} appended with {{2,3}{2,4}{1,5}} with all levels incremented
      // ={{2,1}{2,2}{3,3}{3,4}{2,5}} := [[1,2],[[3,4],5]]
      //
      // Explode is now a flat array manipulation
      // [[[[[9,8],1],2],3],4] := {{5,9}{5,8}{4,1}{3,2}{2,3}{1,4}}
      // The pair nested in level is easilly found as being on level 5
      // So the {5,9},{5,8} "explodes" to the '9' adds to the left and the '8' adds to the right
      // {{5,9}{5,8}{4,1}{3,2}{2,3}{1,4}} explodes to {{4,0}{4,9}{3,2}{2,3}{1,4}} := [[[[0,9],2],3],4]
      //
      // Split is now also a flat array manipulation
      // [[[[0,7],4],[15,[0,13]]],[1,1]] := {{4,0}{4,7}{3,4}{3,15}{4,0}{4,13}{2,1}{2,1}}
      // {{4,0}{4,7}{3,4}{3,15}{4,0}{4,13}{2,1}{2,1}} splits to {{4,0}{4,7}{3,4}{3,round_down(15/2)}{3,round_up(15/2)}{4,0}{4,13}{2,1}{2,1}}
      //
      // Finally the magnitude is an iterative reduction on the flat array (bottom level and up)
      // [[9,1],[1,9]] := {{2,9}{2,1}{2,1}{2,9}}
      // magnitude {{2,9}{2,1}{2,1}{2,9}} = magnitude {magnitude {2,9}{2,1} magnitude {2,1}{2,9}}
      // Note that magnitude reduces a pair into a number and thus a nuber with decreased level
      // = magnitude {{1,3*9+2*1}{1,3*1+2*9}} = magnitude {{1,29}{1,21}} = {0,3*29+2*21} = 129
      //
      auto snail_fish_numbers = to_snailfish_Numbers(puzzle_model.lines);
      
      // test that our representation preserves the nesting structure of the input snailfish numbers!
    
      {
        if (snail_fish_numbers.size() == puzzle_model.lines.size()) {
          for (int i=0;i<snail_fish_numbers.size();i++) {
            auto snf = snail_fish_numbers[i];
            auto snf_s = to_string(snf);
            auto line_s = puzzle_model.lines[i];
            std::cout << "\n" << line_s;
            std::cout << " -> {";
            for (auto const& ln : snf) {
              std::cout << "{level:" << ln.level << ",val:" << ln.value << "}";
            }
            std::cout << "}";
            std::cout << " -> "<< snf_s;
            // The representation is correct if our string representation is equal to the original input string :)
            if (snf_s == line_s) {
              std::cout << " egual :)";
            }
            else {
              std::cout << " ERROR ";
            }
          }
        }
        else {
          std::cout << "\nERROR lines count " << puzzle_model.lines.size() << " != numbers count " << snail_fish_numbers.size();
        }
      }
      
      result.value = magnitude(snailfishnumbers.back());
    }
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
  for (auto const& entry : pTests) {
    answers.push_back({"Part 1 Test",part1::solve_for(entry)});
  }
  answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second.value;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

std::vector<char const*> pTests = {
  "[1,2] + [[3,4],5] becomes [[1,2],[[3,4],5]]"
  ,"[[[[[9,8],1],2],3],4] becomes [[[[0,9],2],3],4]"
  ,"[7,[6,[5,[4,[3,2]]]]] becomes [7,[6,[5,[7,0]]]]"
  ,"[[6,[5,[4,[3,2]]]],1] becomes [[6,[5,[7,0]]],3]"
  ,"[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]] becomes [[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"
  ,"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]] becomes [[3,[2,[8,0]]],[9,[5,[7,0]]]]"
  ,R"([[[[4,3],4],4],[7,[[8,4],9]]] + [1,1] becomes
  after addition: [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]
  after explode:  [[[[0,7],4],[7,[[8,4],9]]],[1,1]]
  after explode:  [[[[0,7],4],[15,[0,13]]],[1,1]]
  after split:    [[[[0,7],4],[[7,8],[0,13]]],[1,1]]
  after split:    [[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]
  after explode:  [[[[0,7],4],[[7,8],[6,0]]],[8,1]]
  =[[[[0,7],4],[[7,8],[6,0]]],[8,1]])"
  ,R"([1,1]
[2,2]
[3,3]
[4,4]
=[[[[1,1],[2,2]],[3,3]],[4,4]])"
  ,R"([1,1]
[2,2]
[3,3]
[4,4]
[5,5]
=[[[[3,0],[5,3]],[4,4]],[5,5]])"
  ,R"([1,1]
[2,2]
[3,3]
[4,4]
[5,5]
[6,6]
=[[[[5,0],[7,4]],[5,5]],[6,6]])"
  ,R"([[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]
[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]
[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]
[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]
[7,[5,[[3,8],[1,4]]]]
[[2,[2,2]],[8,[8,1]]]
[2,9]
[1,[[[9,3],9],[[9,0],[0,7]]]]
[[[5,[7,4]],7],1]
[[[[4,2],2],6],[8,7]]
=[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]])"
  ,R"([[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]
+ [7,[[[3,7],[4,3]],[[6,3],[8,8]]]]
= [[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]])"
  ,R"([[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]
+ [[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]
= [[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]])"
  ,R"([[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]]
+ [[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]
= [[[[7,0],[7,7]],[[7,7],[7,8]]],[[[7,7],[8,8]],[[7,7],[8,7]]]])"
  ,R"([[[[7,0],[7,7]],[[7,7],[7,8]]],[[[7,7],[8,8]],[[7,7],[8,7]]]]
+ [7,[5,[[3,8],[1,4]]]]
= [[[[7,7],[7,8]],[[9,5],[8,7]]],[[[6,8],[0,8]],[[9,9],[9,0]]]])"
  ,R"([[[[7,7],[7,8]],[[9,5],[8,7]]],[[[6,8],[0,8]],[[9,9],[9,0]]]]
+ [[2,[2,2]],[8,[8,1]]]
= [[[[6,6],[6,6]],[[6,0],[6,7]]],[[[7,7],[8,9]],[8,[8,1]]]])"
  ,R"([[[[6,6],[6,6]],[[6,0],[6,7]]],[[[7,7],[8,9]],[8,[8,1]]]]
+ [2,9]
= [[[[6,6],[7,7]],[[0,7],[7,7]]],[[[5,5],[5,6]],9]])"
  ,R"([[[[6,6],[7,7]],[[0,7],[7,7]]],[[[5,5],[5,6]],9]]
+ [1,[[[9,3],9],[[9,0],[0,7]]]]
= [[[[7,8],[6,7]],[[6,8],[0,8]]],[[[7,7],[5,0]],[[5,5],[5,6]]]])"
  ,R"([[[[7,8],[6,7]],[[6,8],[0,8]]],[[[7,7],[5,0]],[[5,5],[5,6]]]]
+ [[[5,[7,4]],7],1]
= [[[[7,7],[7,7]],[[8,7],[8,7]]],[[[7,0],[7,7]],9]])"
  ,R"([[[[7,7],[7,7]],[[8,7],[8,7]]],[[[7,0],[7,7]],9]]
+ [[[[4,2],2],6],[8,7]]
= [[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]])"
  ,R"(the magnitude of [9,1] is 3*9 + 2*1 = 29)"
  ,R"(the magnitude of [1,9] is 3*1 + 2*9 = 21)"
  ,R"(the magnitude of [[9,1],[1,9]] is 3*29 + 2*21 = 129)"
  ,R"([[1,2],[[3,4],5]] becomes 143)"
  ,R"([[[[0,7],4],[[7,8],[6,0]]],[8,1]] becomes 1384)"
  ,R"([[[[1,1],[2,2]],[3,3]],[4,4]] becomes 445)"
  ,R"([[[[3,0],[5,3]],[4,4]],[5,5]] becomes 791)"
  ,R"([[[[5,0],[7,4]],[5,5]],[6,6]] becomes 1137)"
  ,R"([[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]] becomes 3488)"
  ,R"([[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]
=[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]
=4140)"
};
char const* pData = R"([[[[3,0],[0,0]],1],4]
[[[[3,4],0],[7,7]],[1,6]]
[[[[2,0],5],7],[[[3,1],[2,6]],[[0,8],6]]]
[[[[5,5],0],1],[[[0,0],1],[[0,6],[0,9]]]]
[[0,[0,[1,7]]],[3,[1,[7,6]]]]
[[[9,[5,2]],[[5,2],[6,8]]],[[[7,0],7],[[2,3],[9,4]]]]
[[[[3,8],7],[[0,7],[2,0]]],[0,[[2,9],0]]]
[[[7,[2,2]],[3,4]],[6,7]]
[8,[[[3,3],8],[[7,1],[6,7]]]]
[[9,[9,8]],[[1,[9,1]],[2,5]]]
[[[7,8],[[1,2],[2,6]]],[[9,7],[6,[7,0]]]]
[[[3,3],[[5,6],5]],[[[2,8],1],9]]
[[[2,[5,0]],[[9,9],[4,0]]],[0,5]]
[[[9,3],[[9,4],[5,8]]],[[[3,2],[7,1]],[[3,8],1]]]
[[3,2],[[6,[0,9]],[8,3]]]
[[[5,7],[[7,4],[4,6]]],[[[9,8],3],3]]
[[[4,[2,8]],9],[[[8,5],[9,7]],[[8,9],[2,6]]]]
[[[1,[2,4]],6],[[8,[5,2]],[[0,7],[4,1]]]]
[[[[4,3],6],[[6,4],[4,2]]],[[9,0],[[5,9],9]]]
[[[[3,0],6],[4,[7,5]]],4]
[[[[1,0],[7,1]],0],[[[8,5],8],2]]
[[[[2,9],[4,1]],[[8,9],[3,3]]],[9,[[0,7],2]]]
[[1,[4,[4,2]]],[[[3,5],[8,8]],2]]
[[[8,[1,4]],[[6,5],5]],[[7,[4,7]],4]]
[[[[0,5],2],[[9,2],0]],0]
[[[[6,2],[2,4]],[0,[7,3]]],[9,[8,[5,9]]]]
[[8,0],2]
[[[[0,2],2],[[9,2],[8,1]]],[[[7,6],[5,3]],6]]
[[[[8,7],[5,3]],[[3,0],8]],[[[8,4],[2,2]],[[8,1],2]]]
[[[[1,5],[4,6]],[[4,0],[2,4]]],[[1,1],[[0,7],[7,3]]]]
[[7,2],[[7,[6,7]],[8,5]]]
[[[9,7],[[6,6],9]],8]
[[4,2],[[[1,0],[9,1]],[[0,7],[8,0]]]]
[[[[5,9],5],[8,9]],[[2,4],[[5,2],[8,3]]]]
[[[[4,5],[7,0]],[4,5]],[[7,[6,4]],[[1,7],[6,3]]]]
[[2,0],4]
[[2,[[5,1],[2,1]]],[[5,[7,2]],[[2,3],[7,0]]]]
[[4,[4,9]],[9,[6,8]]]
[[[[6,1],[1,5]],[0,[4,0]]],[[[7,0],2],4]]
[[[[3,3],[2,2]],[[2,4],2]],[[8,[1,1]],4]]
[[[[1,5],8],[[9,4],[7,7]]],[[[8,7],[7,2]],[0,[7,3]]]]
[9,[[7,[0,4]],4]]
[4,[0,8]]
[[[[2,6],1],[8,[8,4]]],[[8,2],[1,[8,4]]]]
[[7,[8,[8,8]]],[4,1]]
[[0,6],[[7,[5,9]],[[7,1],8]]]
[4,6]
[[[[3,2],[5,6]],[0,7]],[8,[7,[9,5]]]]
[[[3,7],[4,5]],6]
[[[0,[3,9]],[9,1]],6]
[[[[7,3],8],[6,7]],[[1,0],[1,7]]]
[[[5,[4,8]],2],[[[7,1],6],[[0,3],2]]]
[[1,0],[[1,2],[[2,0],1]]]
[[8,[[6,1],[7,1]]],0]
[[9,[2,0]],[[7,[6,2]],4]]
[[[9,[9,4]],[[4,8],3]],[[9,0],[[2,2],[0,6]]]]
[[[7,5],[[2,9],6]],[[2,4],[[1,1],[8,2]]]]
[[[1,[6,3]],[[2,2],[1,8]]],[[[7,3],[6,0]],[4,[7,6]]]]
[6,5]
[[3,[9,[4,4]]],[[6,9],[4,5]]]
[[[4,[1,8]],[[4,0],6]],[[[9,0],[8,3]],[[8,6],[3,2]]]]
[[[8,[1,2]],[[3,9],6]],[[3,0],1]]
[[1,[2,[4,0]]],6]
[0,[[[1,3],[9,1]],[[3,8],[9,4]]]]
[2,[2,[[2,7],[7,8]]]]
[[[3,0],[[4,6],2]],[9,2]]
[[[5,[2,2]],[[2,7],[9,9]]],[[3,[4,4]],[8,[9,8]]]]
[[[[7,5],[7,9]],[[8,5],6]],[[1,[8,4]],[8,2]]]
[[[6,4],[5,5]],[[[8,1],5],[[6,4],[6,9]]]]
[[[[8,9],0],[[4,6],7]],[[[3,9],[6,4]],[8,[7,4]]]]
[4,[[7,7],4]]
[[[[4,9],[1,2]],[8,[4,7]]],[[8,[4,8]],[0,[5,4]]]]
[1,[7,9]]
[[[5,[2,0]],[[4,3],[6,8]]],[9,9]]
[[[[3,9],9],[4,3]],[1,[3,[8,1]]]]
[[[[8,7],[6,1]],[3,9]],[5,[[8,0],4]]]
[[[[8,2],[4,6]],[6,[9,9]]],[1,[[7,7],4]]]
[[7,5],[[5,0],[0,3]]]
[[[6,0],[9,1]],[[[4,3],[5,0]],[[9,5],[0,0]]]]
[8,[[3,6],3]]
[[[[9,3],7],[1,3]],[[[6,4],[8,4]],[1,5]]]
[[[[3,8],2],[5,4]],[[[1,8],5],[2,[2,7]]]]
[[2,9],[6,[0,2]]]
[[2,[7,9]],[[4,1],[[9,2],[0,7]]]]
[[0,[6,4]],[[9,2],[0,[0,7]]]]
[[[[7,2],[8,6]],[6,2]],[[[1,6],[2,2]],1]]
[[1,6],[[[4,3],[8,2]],[3,[9,4]]]]
[[9,[7,3]],[[[7,0],4],[[1,7],[2,2]]]]
[[7,[5,[9,8]]],[[[7,5],[7,6]],[7,[9,8]]]]
[[[[6,1],[4,3]],4],[[[5,9],4],2]]
[[[[5,1],[2,5]],0],[[7,[5,7]],[[4,4],9]]]
[9,2]
[4,[[[6,6],5],7]]
[[8,[[7,3],[0,7]]],8]
[[[3,4],[[2,3],0]],[[[9,6],[1,1]],[4,[0,4]]]]
[[[[3,3],[2,3]],[2,5]],[[4,[2,7]],3]]
[[[8,[0,3]],2],[4,4]]
[[[3,5],[[2,1],[3,4]]],[[0,3],4]]
[[[[4,1],4],2],[[[3,7],2],[[8,1],3]]]
[[[[0,6],[7,3]],[5,[3,9]]],[7,[[4,1],8]]])";
