#include <iostream>
#include <vector>
#include <string>
#include <utility>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

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
    Answers answers{};
    answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
    answers.push_back({"Part 1     ",part1::solve_for(pTest)});
    answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
    answers.push_back({"Part 2     ",part2::solve_for(pTest)});
    std::cout << "\n";
    return 0;
}

char const* pTest = R"()";
char const* pData = R"()";