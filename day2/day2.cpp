// day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <regex>
#include <ranges>
#include <utility> // std::pair

extern char const* pData;
extern char const* pTest;

template <class I>
struct IContainer {
    I iter;
    I begin() { return iter; }
    I end() { return I{}; }
};

enum eCommand {
    eCommand_Unknown
    , eCommand_Forward
    , eCommand_Down
    , eCommand_Up
    , eCommand_Undefined
};

using Command = std::pair<std::string, size_t>;

eCommand command_of_string(std::string const& s) {
    eCommand result{ eCommand_Unknown };
    bool matched{ false };
    for (int i = eCommand_Unknown; i != eCommand_Undefined; ++i) {
        switch (i) {
        case eCommand_Unknown: break;
        case eCommand_Forward: matched = (s=="forward"); break;
        case eCommand_Down: matched = (s == "down"); break;
        case eCommand_Up: matched = (s == "up"); break;
        case eCommand_Undefined: break;
        default: std::cout << "\nUNKNOWN COMMAND:" << i;
        }
        if (matched) {
            result = static_cast<eCommand>(i);
            break;
        }
    }
    return result;
}

namespace part1 {
    struct Position {
        size_t m_x; // horizontal
        size_t m_z; // vertical
        void move_to(Command const& command) {
            // std::cout << "\nmove_to " << command.first << " " << command.second;
            switch (command_of_string(std::get<std::string>(command))) {
            case eCommand_Forward: m_x += std::get<size_t>(command); break;
            case eCommand_Down: m_z += std::get<size_t>(command); break;
            case eCommand_Up: m_z -= std::get<size_t>(command); break;
            }
            // std::cout << "\n==> Moved To {x=" << m_x << ",z=" << m_z << "}";
        }
    };
}
namespace part2 {
    struct Position {
        size_t m_aim;
        size_t m_x; // horizontal
        size_t m_z; // vertical
        void move_to(Command const& command) {
            // std::cout << "\nmove_to " << command.first << " " << command.second;
            switch (command_of_string(command.first)) {
            case eCommand_Forward: m_x += command.second; m_z += m_aim * command.second; break;
            case eCommand_Down: m_aim += std::get<size_t>(command); break;
            case eCommand_Up: m_aim -= std::get<size_t>(command); break;
            }
            // std::cout << "\n==> Moved To {aim=" << m_aim << ",x = " << m_x << ", z = " << m_z << " }";
        }
    };
}

using Result = size_t;
Result part1_result(char const* pData) {
    Result result{};
    part1::Position position{ 0,0 };
    std::string sdata{ pData };
    const std::regex ws_re("\\s+");
    auto sCommands = IContainer<std::sregex_token_iterator>{ std::sregex_token_iterator(sdata.begin(), sdata.end(), ws_re, -1) };
    auto state{ 0 };
    Command command{};
    for (auto const& s : sCommands) {
        switch (state%2) {
        case 0:command.first = s; break;
        case 1: {
            command.second = std::stoi(s);
            position.move_to(command);
        } break;
        }
        ++state;
    }
    result = position.m_x * position.m_z;
    return result;
}

Result part2_result(char const* pData) {
    Result result{};
    part2::Position position{ 0,0 };
    std::string sdata{ pData };
    const std::regex ws_re("\\s+");
    auto sCommands = IContainer<std::sregex_token_iterator>{ std::sregex_token_iterator(sdata.begin(), sdata.end(), ws_re, -1) };
    auto state{ 0 };
    Command command{};
    for (auto const& s : sCommands) {
        switch (state % 2) {
        case 0:command.first = s; break;
        case 1: {
            command.second = std::stoi(s);
            position.move_to(command);
        } break;
        }
        ++state;
    }
    result = position.m_x * position.m_z;
    return result;
}

int main()
{
    std::cout << "\nHello :)";
    std::cout << "\nresult[part 1 test] : " << part1_result(pTest);
    std::cout << "\nresult[part 1] : " << part1_result(pData);
    std::cout << "\nresult[part 2 test] : " << part2_result(pTest);
    std::cout << "\nresult[part 2] : " << part2_result(pData);
    std::cout << "\nPress <Enter>...";
    std::cin.get();
    std::cout << "\nBye!";
}

char const* pTest = R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";


char const* pData = R"(forward 9
forward 7
down 7
down 3
forward 2
forward 3
forward 7
down 6
forward 7
down 7
forward 9
down 9
up 2
down 5
up 1
forward 5
forward 6
up 4
down 3
down 4
down 5
up 6
down 3
forward 6
forward 4
down 4
forward 5
down 2
up 1
up 8
down 1
down 6
forward 8
down 4
forward 8
forward 6
forward 2
forward 2
forward 3
forward 8
up 9
up 2
down 3
up 3
forward 5
forward 2
up 5
forward 9
down 7
down 2
up 7
down 4
down 6
up 2
down 9
forward 7
down 8
forward 6
up 1
forward 6
forward 4
down 5
forward 6
down 8
down 3
forward 7
down 8
up 7
down 1
up 1
forward 9
down 7
up 3
down 6
down 6
down 6
down 7
down 9
down 6
down 9
down 8
down 3
down 7
down 3
up 8
down 5
down 9
up 4
forward 5
forward 5
forward 2
up 1
forward 6
down 6
down 2
forward 1
forward 8
down 4
down 1
down 8
down 7
forward 6
forward 8
down 8
up 1
up 1
forward 1
forward 3
up 8
down 1
forward 4
down 3
forward 3
forward 4
forward 3
down 3
down 9
down 3
up 6
forward 6
forward 8
forward 2
up 1
up 4
up 4
down 2
down 2
down 2
forward 7
up 9
down 9
up 1
down 5
forward 4
down 2
down 4
forward 3
down 7
down 9
forward 1
up 5
down 5
down 5
forward 2
down 6
forward 8
up 4
forward 6
up 1
down 4
forward 2
down 9
forward 5
down 7
down 8
down 9
forward 6
up 6
forward 9
forward 9
down 7
forward 7
forward 5
up 9
forward 3
down 9
down 1
down 8
down 4
forward 5
forward 6
forward 8
forward 8
down 4
down 3
down 8
forward 3
down 6
down 8
down 2
up 8
up 9
down 6
forward 3
down 4
down 6
forward 9
forward 6
up 2
down 8
forward 2
down 7
forward 9
up 9
down 9
down 2
forward 9
down 4
down 9
up 4
forward 6
down 2
down 9
forward 8
forward 2
up 8
forward 9
forward 2
forward 3
down 2
up 3
forward 9
down 6
down 3
down 1
forward 9
forward 8
down 9
up 7
down 8
up 7
forward 1
forward 1
forward 7
down 2
down 1
up 1
up 6
down 5
up 9
up 7
forward 1
forward 6
forward 1
up 4
down 6
forward 2
up 7
down 2
up 8
forward 9
forward 6
forward 3
forward 8
down 1
forward 8
up 3
forward 1
forward 1
up 9
down 1
down 8
down 2
forward 8
down 8
forward 7
down 5
forward 8
forward 3
forward 6
forward 7
up 5
down 5
forward 8
down 2
forward 3
down 4
down 9
forward 6
forward 5
up 4
forward 7
down 3
forward 9
forward 5
down 3
up 5
forward 4
forward 8
down 7
up 2
forward 7
down 5
up 2
down 9
forward 4
down 3
forward 5
forward 4
down 3
forward 6
up 1
forward 8
down 1
up 7
forward 8
up 1
up 1
forward 2
down 8
forward 4
forward 8
up 6
forward 5
forward 7
up 6
up 4
up 6
down 1
forward 3
down 1
down 1
down 8
forward 8
down 5
down 5
forward 5
forward 9
down 9
forward 7
down 3
down 5
forward 6
down 1
down 5
up 8
down 9
forward 3
down 6
up 2
down 2
forward 2
up 2
forward 8
down 2
forward 9
forward 2
down 7
down 5
forward 1
forward 7
up 6
up 8
forward 8
forward 8
up 3
forward 8
down 6
down 6
forward 4
down 8
down 5
down 7
forward 1
forward 9
forward 9
up 5
down 9
down 1
forward 4
forward 1
up 9
forward 6
down 6
forward 2
up 6
forward 9
up 1
down 2
up 3
forward 2
forward 1
forward 6
down 9
up 1
forward 7
up 3
forward 6
forward 6
up 2
down 8
forward 4
down 4
forward 2
forward 2
down 4
down 7
down 4
down 5
forward 3
down 1
forward 1
forward 8
down 7
up 1
forward 7
forward 2
down 9
down 2
up 2
forward 3
down 4
down 7
down 8
forward 4
forward 5
forward 3
up 3
down 6
forward 4
forward 4
forward 8
forward 1
up 2
up 3
down 4
up 9
forward 1
forward 1
forward 9
down 2
down 5
up 9
down 7
down 9
down 2
down 4
forward 1
forward 1
forward 8
down 9
down 6
forward 2
up 3
down 8
forward 1
forward 8
forward 4
up 7
forward 5
forward 2
forward 2
up 8
down 5
forward 6
down 3
up 5
forward 8
forward 3
forward 9
down 1
down 3
forward 8
down 2
forward 6
forward 2
down 3
down 3
forward 6
forward 4
forward 7
forward 2
up 7
up 4
up 6
forward 9
down 3
down 3
up 7
down 4
up 3
up 3
down 5
forward 1
up 3
down 1
forward 2
up 9
forward 7
down 6
forward 4
forward 8
up 1
forward 6
down 7
down 4
up 9
forward 4
down 7
up 1
forward 9
down 4
down 7
forward 1
down 6
down 6
forward 3
up 8
forward 3
down 1
down 5
down 8
forward 2
up 5
forward 2
up 7
forward 5
forward 1
forward 3
forward 4
forward 5
up 1
forward 9
down 5
down 7
up 9
down 9
forward 7
up 6
up 7
forward 2
forward 1
up 4
forward 6
forward 9
down 1
forward 4
down 5
forward 4
down 3
down 5
forward 6
forward 3
down 3
down 8
down 2
down 4
down 6
down 4
forward 2
up 9
down 3
forward 1
forward 9
forward 5
forward 5
forward 9
up 1
down 4
down 4
up 7
down 3
up 3
up 4
forward 3
forward 1
forward 8
up 6
down 8
down 4
forward 7
forward 9
forward 2
forward 8
up 2
down 4
down 5
forward 9
down 6
down 7
down 8
up 8
forward 3
forward 7
forward 8
up 2
down 9
down 6
forward 3
forward 4
down 4
forward 2
up 6
forward 1
forward 7
down 2
down 1
forward 2
forward 2
down 2
forward 2
forward 7
up 4
down 3
forward 9
down 7
down 7
down 6
forward 3
forward 9
down 9
forward 2
down 5
down 4
down 9
up 9
down 6
down 8
down 1
forward 8
up 4
up 4
down 8
forward 6
down 2
forward 4
forward 3
forward 2
forward 4
down 4
forward 6
down 9
up 7
up 5
down 7
down 4
up 3
forward 4
down 9
forward 6
forward 4
forward 4
down 9
forward 3
forward 2
up 7
forward 3
down 1
down 3
up 5
down 8
down 3
down 4
forward 7
forward 9
up 2
forward 3
up 4
down 5
up 3
up 9
down 6
down 2
down 5
up 4
up 6
forward 4
forward 6
up 5
up 5
forward 8
down 6
forward 6
down 7
down 5
down 3
down 8
forward 6
forward 9
forward 9
up 9
down 3
up 5
forward 4
down 7
forward 5
down 7
down 4
forward 2
forward 9
down 8
up 3
up 7
down 7
up 7
forward 3
down 2
forward 7
down 4
forward 1
down 6
forward 1
up 4
down 7
up 3
forward 7
forward 5
forward 7
forward 6
up 2
down 4
down 8
down 4
up 3
forward 3
up 3
up 3
down 7
down 2
down 3
forward 7
down 6
down 9
up 1
down 8
down 6
down 3
up 2
up 6
forward 9
forward 5
forward 4
forward 9
down 9
forward 2
up 7
down 4
down 8
up 2
forward 6
up 6
up 4
down 2
forward 6
forward 4
up 3
down 6
forward 5
forward 3
up 4
down 7
down 2
down 6
up 7
forward 2
forward 1
forward 3
down 2
forward 1
forward 2
forward 4
down 2
down 5
down 7
down 8
down 1
up 1
up 1
forward 9
down 3
down 1
forward 4
up 6
up 8
forward 7
forward 9
down 3
forward 9
down 9
forward 6
down 1
forward 7
down 9
forward 1
down 8
forward 8
up 7
forward 4
up 5
up 9
forward 1
forward 4
forward 3
down 3
down 8
up 3
forward 1
up 5
forward 5
up 6
forward 8
forward 1
down 7
forward 2
down 9
forward 3
forward 7
forward 2
down 4
forward 2
up 6
down 7
up 3
forward 7
down 8
down 3
forward 2
up 7
down 2
down 8
up 6
forward 7
forward 1
down 3
forward 2
forward 8
down 8
forward 1
down 7
down 1
up 5
up 3
forward 5
down 5
up 9
up 9
down 3
up 3
down 4
down 6
up 7
forward 3
up 5
down 3
forward 4
down 1
up 1
up 6
down 8
forward 5
up 2
down 5
forward 6
forward 4
forward 9
down 9
down 5
forward 5
down 7
down 7
down 8
forward 3
down 6
forward 5
forward 5
down 6
forward 3
down 7
up 4
up 3
down 5
forward 9
forward 9
up 9
down 1
up 2
up 3
down 7
forward 3
down 7
down 4
down 5
down 1
down 4
up 9
forward 1
up 8
forward 7
up 6
down 1
up 2
forward 2
up 9
down 6
forward 4
down 2
up 5
forward 1
forward 4
down 6
down 2
up 8
forward 2
forward 8
forward 4
down 9
up 3
forward 5
forward 9
forward 4
down 2
up 4
up 9
down 5
up 2
forward 6
up 2
down 6
up 5
up 3
up 9
forward 8
down 2
forward 7
up 8
down 9
forward 2
forward 2
down 6
forward 9
forward 2
forward 8
up 3
forward 5
down 4
forward 2
down 7
up 6
forward 7
down 6
down 8
down 3
up 4
up 5
down 2
down 9
forward 2
down 7
forward 2
forward 3
forward 9
down 6
down 1
forward 6
down 5
forward 2
down 5
down 1
forward 5
down 4
down 6
down 5
forward 9
up 6
up 5
up 2
down 1
down 8
forward 4
down 2
forward 5
down 1
forward 7
down 8
down 9
down 7
up 1
forward 2
up 8
down 9
down 2
down 1
down 9
down 2
down 5
forward 9
forward 1
down 1
forward 9
forward 7
down 6
down 1
down 7
forward 4
forward 1
forward 4
forward 5
forward 5
down 2
forward 7
forward 6
forward 3
forward 9
up 1
down 5
down 4
down 2
forward 1
up 7
forward 2)";