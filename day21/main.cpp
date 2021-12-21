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

using Model = std::vector<std::string>;

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (in >> line) {
        result.push_back(line);
    }
    return result;
}

class Dice {
public:
    int roll_count{0};
    int roll() {
        roll_result = (roll_result)%100 + 1;
        ++roll_count;
        return roll_result;
    }
private:
    int roll_result{0};
};

class Player {
public:
    Player(int id,int start_position) : player_id{id}, player_position{start_position} {}
    int player_id{};
    int score{0};
    int player_position{};
    int play(Dice& dice) {
//        std::cout << "\n<player " << player_id << ">";
//        std::cout << "\n\tdice roll ";
        int player_move{0};
        for (int i=0;i<3;i++) {
            auto dice_roll = dice.roll();
//            std::cout << " " << dice_roll;
            player_move += dice_roll;
        }
//        std::cout << " = move " << player_move;
        player_position = (player_position - 1 + player_move)%10 + 1;
//        std::cout << "\n\tnew position " << player_position;
        score += player_position;
//        std::cout << "\n\tscore " << score;
        return score;
    }
};

namespace part1 {
  Result solve_for(int player_1_pos,int player_2_pos) {
      Result result{};
      int const WINNING_SCORE = 1000;
      int turn{0};
      Dice dice{};
      Player player_1{1,player_1_pos};
      Player player_2{2,player_2_pos};
      while (true) {
          if (turn%2 == 0) {
              if (player_1.play(dice)>=WINNING_SCORE) {
                  result = player_2.score * dice.roll_count;
                  break;
              }
          }
          else {
              if (player_2.play(dice)>=WINNING_SCORE) {
                  result = player_1.score * dice.roll_count;
                  break;
              }
          }
          ++turn;
      }
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

namespace part2 {
    Result solve_for(int player_1_pos,int player_2_pos) {
        Result result{};
        return result;
    }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(4,8)});
  answers.push_back({"Part 1 Test",part1::solve_for(8,7)});
  // answers.push_back({"Part 2 Test",part2::solve_for(4,8)});
  // answers.push_back({"Part 2 Test",part2::solve_for(8,7)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"()";
char const* pData = R"()";
