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
    using Row = std::array<int,N>;
    using Board = std::array<Row,N>;
    using Mark = std::array<std::array<bool,N>,N>;
    using BoardsMark = std::vector<Mark>;

    char const* pData;
    std::vector<int> draws{};
    std::vector<Board> boards{};
    BoardsMark bms{};
    std::vector<bool> winning_board{};
    int last_call{};
    int last_winning_board{};

    static int const BOARD_SIZE = N;
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

// Environment env{pTest};
Environment env{pData};

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
    std::cout << "\nBOARD STATES...";
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
    env.last_call = number;
}

void mark_boards2(auto& bms,auto number) {
    std::cout << "\nCALLING NUMBER " << number;
    for (int i = 0; i<bms.size();i++) {
        if (env.winning_board[i] == false) {
            std::cout << "\nmarking board " << i;
            mark_board(env.boards[i],bms[i],number);
            print_board_state(env.boards[i],bms[i]);
        }
        else {
            std::cout << "\nSkipped Board " << i;
        }
    }
    env.last_call = number;
}

bool is_winning_board(auto& bm) {
    std::cout << "\nis_winning_board:";
    bool result{false};
    if (std::any_of(std::begin(bm),std::end(bm),[](auto& bm_row){
        return std::all_of(std::begin(bm_row),std::end(bm_row),[](auto& b) {
            return b;
        });
    })) result = true;
    else {
        auto acc = std::accumulate(std::begin(bm),std::end(bm),bm[0],[](auto& acc,auto& bm_row) {
            for (int i = 0; i<acc.size(); i++) acc[i]&=bm_row[i];
            return acc;
        });
        if (std::any_of(std::begin(acc),std::end(acc),[](auto b) {
            return b;
        })) result = true;
    }
    if (result) std::cout << "YES";
    else std::cout << "NO";
    return result;
}

void play1() {
    env.bms = Environment::BoardsMark{env.boards.size(),Environment::Mark{}};
    for (auto const& number : env.draws) {
        mark_boards(env.bms,number);
        if (std::any_of(std::begin(env.bms),std::end(env.bms),[](auto& bm) {
            return is_winning_board(bm);
        })) break;
        
    }
    print_board_states(env.bms);
}

int board_in_play_index() {
    int result = env.boards.size(); // end
    for (int i=0;i<env.winning_board.size(); i++) {
        if (env.winning_board[i] == false) {
            result = i;
            break;
        }
    }
    return result;
}

int boards_in_play() {    
    int result{0};
    for (int i=0;i<env.winning_board.size();i++) {
        if (env.winning_board[i] == false) ++result;
    }
    std::cout << "\nboards in play = " << result;
    return result;
}

void play2() {
    env.bms = Environment::BoardsMark(env.boards.size(),Environment::Mark{});
    env.winning_board = std::vector<bool>(env.boards.size(),false);
    for (auto const& number : env.draws) {
        mark_boards2(env.bms,number);        
        for (int i=0;i<env.winning_board.size();i++) {
            if (env.winning_board[i] == false) {
                env.winning_board[i] = is_winning_board(env.bms[i]);
                if (boards_in_play()==0) {
                    env.last_winning_board = i;
                    std::cout << "\nLast winning board " << env.last_winning_board;
                    return;
                }
            }
        }
    }
}


size_t sum_unmarked_numbers(auto& board, auto& bm) {
    std::cout << "\nsum_unmarked_numbers";
    size_t result{};
    for (int i=0;i<board.size();i++) {
        for (int j=0;j<board[i].size();j++) {
            if (bm[i][j] == false) {
                std::cout << " sum:" << board[i][j];
                result += board[i][j];
            }
        }

    }
    std::cout << "\nsum=" << result;
    return result;
}

auto calculate_answer() {
    std::cout << "\nsum_unmarked_numbers";
    size_t result{0};
    bool there_is_a_winning_board{false};
    for (int i=0;i<env.boards.size();i++) {
        if (is_winning_board(env.bms[i])) {
            // Sum all unmarked numbers on this winning board
            auto sum = sum_unmarked_numbers(env.boards[i],env.bms[i]);
            std::cout << "\nlast call = " << env.last_call;
            result = sum * env.last_call;
            there_is_a_winning_board = true;
            break;
        }
    }
    return result;
}

auto calculate_answer2() {
    std::cout << "\nsum_unmarked_numbers";
    size_t result{0};
    std::cout << "\nlast call = " << env.last_call;
    auto sum = sum_unmarked_numbers(env.boards[env.last_winning_board],env.bms[env.last_winning_board]);
    result = sum * env.last_call;
    return result;
}

void print_answer(auto answer) {std::cout << "\nanswer=" << answer;}

namespace part1 {
    void solve() {
        // Read input
        read_input();
        // create boards
        create_boards();
        // play
        play1();
        // calculate answer
        auto answer = calculate_answer();
        // print answer
        print_answer(answer);
    }
}

namespace part2 {
    void solve() {
        // Read input
        read_input();
        // create boards
        create_boards();
        // play
        play2();
        // calculate answer
        auto answer = calculate_answer2();
        // print answer
        print_answer(answer);
    }
}

int main(int argc, char *argv[]) {
    // part1::solve();
    part2::solve();
    std::cout << "\n";
}

char const* pTest = R"(7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

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
char const* pData = R"(27,14,70,7,85,66,65,57,68,23,33,78,4,84,25,18,43,71,76,61,34,82,93,74,26,15,83,64,2,35,19,97,32,47,6,51,99,20,77,75,56,73,80,86,55,36,13,95,52,63,79,72,9,10,16,8,69,11,50,54,81,22,45,1,12,88,44,17,62,0,96,94,31,90,39,92,37,40,5,98,24,38,46,21,30,49,41,87,91,60,48,29,59,89,3,42,58,53,67,28

31 23 52 26  8
27 89 37 80 46
97 19 63 34 79
13 59 45 12 73
42 25 22  6 39

27 71 24  3  0
79 42 32 72 62
99 52 11 92 33
38 22 16 44 39
35 26 76 49 58

39 19 82 53 57
52 98 69 77 23
 1 40 18 66 83
34 85 28 48 16
15 93 38 96 27

74 50 88 84 99
34  2 11 25 17
57  4 19 83  1
59 77 42 36 33
73 22 23 37 55

98 91 56 84 78
45 21 24 83 40
46 58  8 67  4
33 97 55  7 86
 2 68 64 27 69

68 29 14 49 26
 4 21 87 71 32
58  5 17 46 93
45 96  8 83  2
78 91  9 20 42

49 81 19 48 37
38 23 45 82 92
93 99 67 66 42
40 74 25 56 16
21 47 26 75 61

53 66 72 30 34
55 82 77  6 92
60 56  8 22 88
 5 71 49 29 74
28  2 32 84 73

52 31 24 68 41
48 82 19 29 65
51 91 97 39 80
 3 55 43 40 38
20 89 53 45 75

29 74 19 89 18
32 88 93 46 63
91  4 94 64  5
57 54 49 36 40
97 81 39 77  1

 7 57 94 84 39
92  3 28 15 75
88 45 65 81 63
86  4 89 37 71
 8 13 66 42 85

60 66 35 47 98
96 27 40 51 39
 3 64 25 28 74
58 17 97 59 29
95 31 18 44 37

 3 31 97 85 71
79 82 22 61 98
87 14 17 66 75
36 89 88 83 63
44  8 81 25 48

73 84 28 90 94
25 19 44 10 23
 8 59 17  9 93
20 77 97 64  6
98 82 27 70 91

18 51 16 99  2
58 22 89 13 19
39 66 91  8 32
49 24 85 94 42
45 70 10 86  4

23 81 66 13 34
25 80 97  5 42
79 35  2 78  9
 0  6 91 94 45
21 90 76 50 56

50 92  2 96 75
85 82 80 97 31
61 35 55 27 56
74 42  9 29 90
86 15 88 47  1

18 20 54 92 62
45 22 32 61 75
 1 38 50 81 42
82  4 21 77 65
27 51 56 39 48

36 10 62 28 70
94 99 34 54  6
15  1 41 13 12
92 52  2 63 82
90 64 29 69 32

23 77 33 90 17
45 78  5 67 28
57 73 89 81 21
49 64 37 15 14
 7 59  4 43 16

81 92 25 28 90
93 72 43 94 26
24  9 13 74 10
21  2 36 32 51
87 97 55 86 71

82 71 99 17 90
69 95 65 55 10
 9 92 39 62 78
59 13 61 24 44
 8 31 58  0 57

17 83 55 99 27
79  4 33 76  7
81 43 44 49 72
 2 48 97 20 77
47 60 35 16 63

93 95 94  1 98
61 57 84 55 22
85 40 65 46 59
21 15 63 77  7
13 99 49  3 96

 8 21 14 45 41
65 63 82 62 28
91 44 22 79 96
20 75 86  3 26
74 11 42 59 36

 5 52 43 92 99
46 63 10 45 81
13 66 21 32 89
25 28 96 40 88
27 18 31 73 34

 3 26 43 32 36
68 87 67 65 99
73 61 20 90  7
21 52  2 82 10
58 49 56 16 80

97 25 93 63 32
87 14  5 22 76
89 92 91  3 51
 0 24 95 69 20
96 11 10  1 55

95 86 44 75 70
59 76 45  2 99
 1 34 71 81 41
87 14 33 84 96
 8 38  9 82 68

27 71 70 75 76
25 87  2 79 96
20 88 50 37 32
48 94 63 86 22
15  6 34 78 59

30 89 51 31 77
74 10 86 71 84
29 54 58 44  5
11 90 26 50 63
64 62 20 40 46

37  9 46 23 31
68 21 25 36 90
17 33  6 50 30
11 89 20 47 60
26 59 34 62 77

84 52 40 97  7
88 30 42 58 94
64 10  2 90 83
44 35 77 91 47
14 74  9 78 53

86 14  0 39 24
87 69 58  8 73
88 74 27 40 51
63 54 55 93 61
16 66 15 21 48

43 70  9 81 42
36 54 99 34 95
98 19 90 25 44
69 56 18 77 49
58 16 67 75 57

36 44 14 98 23
31  5 83 46  3
45 21 41 11 60
33 81 88 92 65
13 51 48 59 71

12  5 70 87 32
42 18 90 73 88
68 29 76 38 55
67 62 15 77 34
39 27 51 54 19

87  8 92 93 88
77 54 15  1 43
35 97 26 21 29
13 46 96 69 47
51 38 91 32 63

73 99 30 15 16
42 58 21 88 44
45 13 27 68  9
36  6 81 53  5
78 76 11 60  1

57 76 50 78 31
45 42 68 53 16
 9 88 89 19 21
96 61 97 69 34
98 87 33 82  0

 4 16 89 57 64
46 75 77 65 23
71 42 96 52 38
 1 21 93  0 35
59 80 53 36 58

97 62 35  1 88
98 60 17 45 94
12 43 65 23 19
71 52  3 40 59
50 76 61 20 22

92 65 38 93 13
55 26 10 46 29
85 23 19 74 34
60 14 27 36 18
53  4 52 49 17

99 56 93 70 28
25  0 77 80 57
91 50 72 76 23
53 58 95 78 59
75 85 90 44  9

30  8  5 60  6
28 35 59 70 96
20 99 98 81 79
94 78 27 71  4
 7 34 43 46 51

93 65 22 69 33
92 49 75 35 11
58 39 62 86 83
64  4 76 48 82
74  1 56 95 31

 1 78 98 90 55
80 14 36 99  7
85  8 10  9 92
76 11 40 70 62
43 53 74 35 58

46 78 35 28 49
84 73 65 25 34
40 59 66 36 67
16 22 29  0 45
20 56 39 88 91

32 58 35 25 79
78 94 57 38 14
89 87 68 48 76
 7 67 40 51 33
95 31 43 93 92

38 21 82 31 23
54 16 77 37 42
73 99  7 34 90
71 26  5 91 52
22 27 47 85 62

 2 86 28 37 55
 1 82  9 36 31
52 98 89 30 60
13 17 63 38 57
73 50 42 20 12

56  3 67 62 35
59 39 19 22 27
21 58 57 41 54
75 13 82 50 32
23  5 99 66 10

 7 19 45 66 78
38 57 40 73 87
58 30 99 53 83
64  1  8 56 95
70 77 16 18 82

72 83 95 37 35
54 59 92 21 79
 7 81 86 29 41
52 99 42 57 71
 3 15 75 34 77

 7 70  5 69  4
34 60 40 73  6
74 54 67 32 38
93 62 17 51 86
57 88 99  3 16

42 74 11 34  7
82 47 71 31 58
69 23 43  4 64
32 19 98 93 41
63 97  8 85 48

63 54 34 38 86
 4 27 15 49  0
61 77 53 98 74
62 23 88 97 37
93 28 25 50 13

56 82 41 27 79
23 31 64  7 65
52 98 93 16 57
88 49 10 11 62
43 95 53 51 83

41 10 87 54 86
19 22 13 40 17
37 27 45 29 63
83 85 81 90  7
57 88 47 66 56

67 44 54 88 89
20 46 61 28 92
86 49 60 83 95
42 78 97 51 96
11 62  4 26 31

18 68 87 26 70
62 84 11 33 90
 0 45 66 83  6
20 19 27 44 55
52  8  5  7  3

54 94 88 76 92
13 98 22 33 26
95 62 53 81 24
29 69 15 87 25
61 40 84 90 93

 7 31  3 28 46
20 51 21 18 38
30 92 39 70 61
27 88 35 96 74
23  5 66 11 42

40 61 90 57 54
41 14 99 62 59
92 10 48 81 52
22 29 77 18 87
31 79 25 94 13

17 26 44 98 57
74 83 51 14 11
76 91 96 64 33
43 45 92 72 27
66  3 28 20 40

88 82 44 71 55
83 47 51 76 24
86 19 42 34 99
30 31 87 48 62
98 53 68  9 21

 3 31  6 41 61
24 77 81 96 44
78 73  1 98 11
40 80 27 65 92
62 67  2 30 10

78 46 50 65 56
84 16 32 58 86
22 12 54 99 35
 9 43 55 10 94
66 81 59 92 76

78  3 55 23 83
13 42 94 91 22
14 37 31 67 71
 8 61 57 34 43
74 50  0 39 65

78 16 13 91 34
14 74 86  3 97
12 89 58 65 51
29 57 48 44 93
95  1 42 39 92

93 96 16 85 25
59  3 70 19 17
21 84 58 38 86
57 10 35 95 79
81 44 73 63  9

22  1 96  7 93
40 49  2  4 66
87 21 17 32 48
44 28 42 99 26
69  8 85 86 75

21 31 37 87 28
89 43 74 83 57
95 29 92 88 35
94 25 97 81 50
15 19 73 45 63

92 62 67 95 57
30  8  4 39 64
99 31 70 63 96
25 53 24 93 35
34 51 82 91 28

41 30 20 56 46
16 32 98 60 35
67  9 43 42 88
78 90 71  5 29
49 31 37 63 18

80 40 88  5 62
 3  6 74 71 97
19 58 63 59 38
50 64 34 68 45
25 30 21 33 83

10 65 67 17 50
21 51 18 68 59
29 78 77 99 76
62 35 96  7 95
82 53 42 49 69

74 65 89  6  1
18 30 72 75 24
60 50 52 55 82
68 99  4 61 22
 9 37 84 57 87

96 85 56 72  2
 9 38 98 12  4
34 45 74 97 86
18 94 64 70 68
91 41 58 39 66

34 13 26 80 29
 0  4 21 60 90
39 73 12  2 19
64 44 61 88 45
59 50  8 91 49

34 85 55  2 75
10 15 89 12 63
90 29 87 73 71
38 17 84 45  9
97 98 77 23 61

47 43 22 58  1
63 44  2 94 99
33 81 51 49 13
38 86 42 91 23
 7 67 68 39 84

 4 26 12 38 41
43 16 88 71 99
50 24 19 77 98
23 73 44 10 51
56 42 30 52 59

57 16  9 62 27
26 65 56 10 82
 0 74 78 12 99
77 18 38  5 37
 7 60 40 90 23

14 69 18 51  8
21 79 60 36 12
68 44 59 45 16
90 50 85 25 70
91 31 30 54 26

24 40 51 72 63
31 60 62 25 96
 9 44 35 28 91
97  4 34 81  2
61 68 94 52 86

 0 57 95 88 94
36 38 25 35 19
13  6  8 61 98
45 85 86 69 97
41 32  7 15 59

41 82 19 29 34
44 96  6 91 76
69 21 32 94 98
 4 10 88 30  2
 8 74 56 65 99

36 91 73 15 54
62 55 40 27 44
11 60 95 61 46
31 32 21 41 35
74 86 83 89 79

 2 96 94 82 68
39 83 49 30 15
62 11 86 99 59
51 80 12 72 58
87 66 98 53 29

44 71 18 63 85
11 75 60 66 13
36  9 94 57  8
10 12 32  3 86
 4 29 54 70 21

27 33 76 83 67
77 29 65 39 44
52 34 25 93 64
35  4 57 92 84
41 51 88 96  0

21 91 82  3 26
23  8 36 20 73
54 39 60 34 57
49 99 97 69 43
41 93 95 80 63

73 77  4  9 22
17 33 15 86 79
38 16 99 98 30
64 92 76 50 68
83 85 52 87 88

57 53 13 36 76
 7 10 91  3 22
 8 84 56 73 59
62 80 85 38 33
68 97 47 14 96

36  8 98 43 70
85 95 31  1 51
33 41 78 89 56
76 16 15 34 82
12 18 39  4  3

98 49 41 30 95
68 89 81 48 84
15 19 90 66 14
32  1 88 34 64
73 65  6 20 86

22 18 13 74 34
75  4 60 88 46
25 97 54 94 91
42 67 40 11 81
 5 12 49 48 15

82 91 18 73 57
97 50 34 16 66
29 43 81 20 15
19 44 85  4 32
90 58 39 53 42

50 53 83 10  0
93 16 84 23 13
89 63 75 69 51
65 35 67 56 70
 4 37 29 47 38)";