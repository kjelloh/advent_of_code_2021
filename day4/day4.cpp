#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <array>
#include <type_traits>
#include <numeric>

template<int N>
struct Env {
    char const* pData;
    std::vector<int> draws{};
    using Row = std::array<int,N>;
    using Board = std::array<Row,N>;
    static int const BOARD_SIZE = N;

    std::vector<Board> boards{};
};

using Environment = Env<5>;

namespace tokenize {
    auto split_int(auto const& line,char delim) {
        std::vector<int> result{};
        std::stringstream in(line); //create string stream from the string
        while(in.good()) {
            std::string sv;
            std::getline(in, sv, delim);
            if (sv.size()>0) result.push_back(std::stoi(sv));
        }
        return result;
    } 
    auto split_csv_int(auto const& line) {
        return split_int(line,',');
    } 
    auto split_whitespace_int(auto const& line) {
        std::vector<int> result{};
        std::string item{};
        std::stringstream in(line);
        while (in >> item) result.push_back(std::stoi(item));
        return result;
    } 
}

extern char const* pTest;
extern char const* pData; 

Environment env{pTest};

void read_draws(std::istringstream& in) {
    std::string line{};
    in >> line;
    env.draws = tokenize::split_csv_int(line);
}

bool read_board(std::istringstream& in) {
    std::string line{};
    while (in.good()) {
        std::getline(in,line);
        if (line.size()>0) break;
    }
    env.boards.push_back({});
    for (int i = 0; i<env.BOARD_SIZE;i++) {
        if (line.size() == 0) {
            env.boards.pop_back();
            return false;
        }
        auto row = tokenize::split_whitespace_int(line);
        std::copy(std::begin(row),std::end(row),std::begin(env.boards.back()[i]));
        std::getline(in,line);
    }
    return true;
}

void read_boards(std::istringstream& in) {
    while (read_board(in));
}

void print_board(auto const& board) {
    std::cout << "\n<BOARD>";
    for (auto const& row : board) {
        std::cout << "\n";
        std::copy(std::begin(row),std::end(row),std::ostream_iterator<int>(std::cout," "));
    }
}

void print_boards() {
    std::cout << "\n<BOARDS>";
    for (auto const& board : env.boards) {
        print_board(board);        
    }
}

void print_draws() {
    std::cout << "\n<DRAWS> ";
    std::copy(std::begin(env.draws),std::end(env.draws),std::ostream_iterator<int>(std::cout," "));
}

void read_input() {
    std::istringstream in{env.pData};
    read_draws(in);
    read_boards(in);
    print_draws();
    print_boards();
}
void create_boards() {
    /* Done in read_input...*/
}

void print_board_state(auto& board,auto& bm) {
    std::cout << "\nBOARD STATE";
    for (int i = 0; i < env.BOARD_SIZE; i++) {
        std::cout << "\n";
        std::copy(std::begin(board[i]),std::end(board[i]),std::ostream_iterator<int>(std::cout," "));
        std::cout << "\t\t";
        std::transform(std::begin(bm[i]),std::end(bm[i]),std::ostream_iterator<std::string>(std::cout," "),[](auto b) {
             return (b?" X ":" - ");
        });
    }
    std::cout << "\n";
}

void print_board_states(auto& bms) {
    std::cout << "\nBOARD STATES";
    for (int i = 0; i<bms.size();i++) {        
        print_board_state(env.boards[i],bms[i]);
    }
}

void mark_board(auto& board,auto& bm, auto number) {
    for (int i = 0; i < env.BOARD_SIZE; i++) {
        for (int j = 0; j < env.BOARD_SIZE; j++) {
            if (board[i][j] == number) bm[i][j] = true;
        }
    }
}

void mark_boards(auto& bms,auto number) {
    std::cout << "\nCALLING NUMBER " << number;
    for (int i = 0; i<bms.size();i++) {
        mark_board(env.boards[i],bms[i],number);
        print_board_state(env.boards[i],bms[i]);
    }
}

bool is_winning_board(auto& bm) {
    bool result{};
    if (std::any_of(std::begin(bm),std::end(bm),[](auto& bm_row){
        return std::all_of(std::begin(bm_row),std::end(bm_row),[](auto& b) {
            return b;
        });
    })) result = true;
    else {
        auto acc = std::accumulate(std::begin(bm),std::end(bm),bm[0],[](auto& acc,auto& bm_row) {
            for (int i = 0; i<acc.size(); i++) acc[i]&=bm_row[i];
            // print column flags
            std::cout << "\nacc:";
            std::transform(std::begin(acc),std::end(acc),std::ostream_iterator<std::string>(std::cout," "),[](auto& b) {
                return (b?"X":"-");
            });
            return acc;
        });
        if (std::any_of(std::begin(acc),std::end(acc),[](auto b) {
            return b;
        })) result = true;
    }
    if (result) std::cout << "\nWinning Board!";
    return result;
}

void play() {
    using Mark = std::array<std::array<bool,Environment::BOARD_SIZE>,Environment::BOARD_SIZE>;
    using BoardsMark = std::vector<Mark>;
    BoardsMark bms{env.boards.size(),Mark{}};
    for (auto const& number : env.draws) {
        mark_boards(bms,number);
        if (std::any_of(std::begin(bms),std::end(bms),[](auto& bm) {
            return is_winning_board(bm);
        })) break;
        
    }
    print_board_states(bms);
}
auto calculate_answer() {return 0;}
void print_answer(auto answer) {std::cout << "\nanswer=" << answer;}

namespace part1 {
    void solve() {
        // Read input
        read_input();
        // create boards
        create_boards();
        // play
        play();
        // calculate answer
        auto answer = calculate_answer();
        // print answer
        print_answer(answer);
    }
}

int main(int argc, char *argv[]) {
    part1::solve();
}

char const* pTest = R"(19,7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

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
char const* pData = R"()";