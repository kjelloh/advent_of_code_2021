#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <map>

extern char const* pTest;
extern char const* pData;

using Result = uint64_t;
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

class GameState {
public:
    std::array<int,4> state{};
    GameState(int pos1, int score1,int pos2, int score2) : state{pos1,score1,pos2,score2} {}
    bool operator<(GameState const& other) const {return state < other.state;}
    int pos1() const {return state[0];}
    int pos2() const {return state[2];}
    int score1() const {return state[1];}
    int score2() const {return state[3];}
};
using Count = size_t;
using WinCounts = std::pair<Count,Count>;
using StateMap = std::map<GameState,WinCounts>; // Map a game state to winning counts

WinCounts win_counts(const int WIN_SCORE, GameState const& game_state,int player_in_turn, StateMap& memoized) {
    WinCounts result;
    if (game_state.score1()>=WIN_SCORE) {
        return {1,0}; // player 1 wins
    }
    else if (game_state.score2()>=WIN_SCORE) {
        return {0,1}; // player 2 wins
    }
    else if (auto iter = memoized.find(game_state);iter != memoized.end()) {
        return iter->second;
    }
    // Next possible state is player in turn throwing three times any of of 1,2 or 3
    else if (player_in_turn==1) {
        for (int player_move : {3,4,5,6,7,8,9}) {
            int new_player_position = (game_state.pos1() - 1 + player_move)%10 + 1;
            int new_score = game_state.score1() +  new_player_position;
            GameState new_state{new_player_position,new_score,game_state.pos2(),game_state.score2()};
            auto win_count = win_counts(WIN_SCORE,new_state,2,memoized);
            memoized[new_state] = win_count;
            // Sum the wins of all possible player move outcomes
            result.first += win_count.first;
            result.second += win_count.second;
        }
    }
    else if (player_in_turn==2) {
        for (int player_move : {3,4,5,6,7,8,9}) {
            int new_player_position = (game_state.pos2() - 1 + player_move)%10 + 1;
            int new_score = game_state.score2() +  new_player_position;
            GameState new_state{game_state.pos1(),game_state.score1(),new_player_position,new_score};
            auto win_count = win_counts(WIN_SCORE,new_state,1,memoized);
            memoized[new_state] = win_count;
            // Sum the wins of all possible player move outcomes
            result.first += win_count.first;
            result.second += win_count.second;
        }
    }
    return result;
}

namespace part2 {
    Result solve_for(int player_1_pos,int player_2_pos) {
        Result result{};
        // There is a limited set of end states (universes) where a player wins
        // The player that wins can be at position 1..10 and have a score of 20+(1..3) = 10*3 states
        // At the same time the player that looses can be at position 1..10 with a score of 0..19 = 10*19 states
        // But there is a relationship between the players states due to them taking turn throwing the dice.
        // The number of runs required for a player to win is between run_count*1 >= 20 and run_count*3 >=20, i.e., a run_count of  20..7 runs
        // This limits the possible states of the loosing player to a score of (all ones) = 6 up to a non-winning score 19 = 13 possible scores
        // so when the winning player is at any of the 30 winning states, the loosing player is at any of  10*13 = 130 states.
        // so there are 2*30*130 = 7800 states where a player wins.
        // But at what scores do a player win?
        // Imagine all game states {pos1,score1,pos2,score2} with board position 1..10 and score 0..23 for player 1 and 2
        // Then imagine we can count how many ways there is to reach each of these states?
        // Then the number of ways to reach a winning state for a specific is the sum of the ways to reach each winning state for that player.
        // We are looking for the winning count for the winner that wins most times.
        // Can we ask this question recursively?
        // Recursive question: given game state {pos1,score1,pos2,score2}, how many times does player 1 win and how many times' player 2?
        // We can then start with the initial state, generate all possible "next" game states and for each ask the question again.

        // Play all possible games
        const int WIN_SCORE = 21;
        GameState init_game_state{player_1_pos,0,player_2_pos,0};
        StateMap memoized{};
        auto [p1_win_count,p2_win_count] = win_counts(WIN_SCORE,init_game_state,1,memoized);
        std::cout << "\nmemoized size " << memoized.size();
        result = std::max(p1_win_count,p2_win_count);
        return result;
    }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(4,8)});
  // answers.push_back({"Part 1 Test",part1::solve_for(8,7)});
  answers.push_back({"Part 2 Test",part2::solve_for(4,8)});
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
