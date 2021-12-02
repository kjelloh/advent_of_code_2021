#include <iostream>
#include <sstream>
#include <iomanip> // std::quoted
#include <regex>
#include <string>
#include <ranges>

namespace tokenize {
    void t1(char const* pData) {
        std::istringstream in{pData};
        std::string s{};
        while (std::getline(in,s)) {
            std::cout << "\nt1 : " << s;
        }
    }
    template <class I>
    struct IContainer {
        I iter;
        I begin() { return iter; }
        I end() { return I{}; }
    };    
    void t2(char const* pData) {
        std::string sdata{ pData };
        const std::regex ws_re("\\s+");
        auto sTokens = IContainer<std::sregex_token_iterator>{ std::sregex_token_iterator(sdata.begin(), sdata.end(), ws_re, -1) };
        auto state{ 0 };
        for (auto const& s : sTokens) {
            std::cout << "\nt2 : \"" << s << "\""; // s is not std::string
        }
    }
    namespace detail {
        template <typename Out>
        void split(const std::string &s, char delim, Out result) {
            std::istringstream in(s);
            std::string item;
            while (std::getline(in, item, delim)) {
                *result++ = item;
            }
        }

        std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }    
    }
    void t3(char const* pData) {
        auto tokens = detail::split(pData,'\n');
        for (auto const& s : tokens) {
            std::cout << "\nt3 : " << std::quoted(s);            
        }
    }
    void t4(char const* pData) {
        std::istringstream in {pData};
        std::string s;
        while (in >> s) {
            std::cout << "\nt4 : " << std::quoted(s);
        }        
    }
    void t5(char const* pData) {
        std::istringstream in {pData};
        struct Entry {
            std::string s1;
            std::string s2;
            std::string s3;
            std::string s4;
        } entry;
        while (in >> entry.s1 >> entry.s2 >> entry.s3 >> entry.s4) {
            std::cout << "\nt5 : " 
                << std::quoted(entry.s1) 
                << "," << std::quoted(entry.s2)
                << "," << std::quoted(entry.s3)
                << "," << std::quoted(entry.s4); 
        }        
    }
    void t6(char const* pData) {
        std::istringstream in {pData};
        std::string sEntry;
        while (std::getline(in,sEntry)) {
            if (sEntry.size() > 0) {
                // Same group
                std::cout << "\nt6 : " << std::quoted(sEntry); 
            }
            else {
                // New group (empty line)
                std::cout << "\nt6 : NEW GROUP";
            }
        }        
    }

    void t7(char const* pData) {
        std::istringstream in {pData};
        for (const auto& s: std::ranges::istream_view<std::string>(in)) {
            std::cout << "\nt7 : " << std::quoted(s, '|') << " ";
        }
    }
    void t8(char const* pData) {
        std::istringstream in {pData};
        std::cout << "\nt8 floats: ";
        std::ranges::copy(
            std::ranges::istream_view<float>(in),
            std::ostream_iterator<float>{std::cout, ", "});        
    }


    void apply_all(char const* pdata) {
        t1(pdata);
        t2(pdata);
        t3(pdata);
        t4(pdata);
        t5(pdata);
        t6(pdata);
        t7(pdata);
        t8(pdata);
    }     
}

extern char const* pTest1;
extern char const* pData;

using Result = size_t;

namespace part1 {
    Result answer(char const* pData) {
        Result result{};
        tokenize::apply_all(pData);
        return result;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "\nWelcome :)";
    std::cout << "\nanswer[part 1 test] : " << part1::answer(pTest1);
    std::cout << "Press <enter>...";
    std::cin.get();
    std::cout << "Bye!";
    return 0;
}

char const* pTest1 = R"(199
200
208
210
200
207
240
269
260
263

..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#
abc

a
b
c

ab
ac

a
a
a
a

b

forward 5
down 5
forward 8
up 3
down 8
forward 2)";;
char const* pData = R"()";

