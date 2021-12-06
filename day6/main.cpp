#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <array>
#include <numeric>
#include <utility>

extern char const* pTest;
extern char const* pData;

using Answer = size_t;
using Age = int;

namespace part1 {
    using Fish = std::vector<int>;

    void print_fish(auto const& fish) {
        auto size = fish.size();
        std::cout << "[" << size << "]";
        auto cropped_index = std::min(size,size_t{35});
        for (int i=0;i<cropped_index;i++) {
            std::cout << " " << fish[i];
        }
        if (cropped_index<size) std::cout << "...";
    }

    Fish parse(auto& in) {
        Fish result{};
        std::string sAge{};
        while (std::getline(in,sAge,',')) {
            // std::cout << " " << sAge;
            result.push_back(std::stoi(sAge));
        }
        return result;
    }

    Answer answer(char const* pData) {
        Answer result{};
        std::istringstream in{pData};
        std::vector<Fish> fish;
        fish.push_back(parse(in));
        fish.push_back(Fish(fish[0].size(),0));
        std::cout << "\nInitial state    ";
        print_fish(fish[0]);
        for (int day=0;day<80;day++) {
            int today = day%2;
            int tomorrow = (day+1)%2;
            int new_borns = 0;
            std::transform(
                std::begin(fish[today])
                ,std::end(fish[today])
                ,std::begin(fish[tomorrow])
                ,[&new_borns](auto count) {
                    auto result = (count==0)?6:count-1;
                    if (count<6) new_borns += (result==6)?1:0; 
                    return result;
            });
            Fish new_fish(new_borns,8);
            fish[tomorrow].insert(fish[tomorrow].end(),std::begin(new_fish),std::end(new_fish));
            fish[today].insert(fish[today].end(),std::begin(new_fish),std::end(new_fish));
            std::cout << "\nAfter " << day+1 << " day(s) : ";
            print_fish(fish[tomorrow]);
        }
        result = fish[0].size();
        return result;
    }
}

namespace part2 {
    using Counts = std::array<size_t,9>;
    auto parse(auto& in) {
        Counts result{};
        std::string sAge{};
        while (std::getline(in,sAge,',')) {
            // std::cout << " " << sAge;
            ++result[std::stoi(sAge)];
        }
        return result;
    }
    void print_counts(auto counts) {
        for (auto const& count : counts) {
            std::cout << " " << count;
        }
    }
    template<int N>
    Answer answer(char const* pData) {
        Answer result{};
        std::istringstream in{pData};
        auto counts = parse(in);
        std::cout << "\nInitial state    ";
        print_counts(counts);
        for (int day=0;day<N;day++) {
            int today = day%2;
            int tomorrow = (day+1)%2;
            std::rotate(std::begin(counts),std::begin(counts)+1,std::end(counts));
            counts[6] += counts[8];
            std::cout << "\nAfter " << day+1 << " day(s) : ";
            print_counts(counts);
        }
        result = std::accumulate(std::begin(counts),std::end(counts),size_t{0});
        return result;
    }
}


int main(int argc, char *argv[])
{
    std::vector<std::pair<std::string, Answer>> answers{};
    answers.push_back({ "answer[part 1 test]",part1::answer(pTest) });
    answers.push_back({ "answer[part 1]",part1::answer(pData) });
    answers.push_back({ "answer[part 1 test with part 2]",part2::answer<80>(pTest) });
    answers.push_back({ "answer[part 1 with part 2]",part2::answer<80>(pData) });
    answers.push_back({ "answer[part 2]", part2::answer<256>(pData) });
    for (auto const& answer : answers) {
        std::cout << "\n" << answer.first << " " << answer.second;
    }
    std::cout << "\nPress <enter>...";
    std::cin.get();
    std::cout << "\n";
}

char const* pTest = R"(3,4,3,1,2)";
char const* pData = R"(4,1,4,1,3,3,1,4,3,3,2,1,1,3,5,1,3,5,2,5,1,5,5,1,3,2,5,3,1,3,4,2,3,2,3,3,2,1,5,4,1,1,1,2,1,4,4,4,2,1,2,1,5,1,5,1,2,1,4,4,5,3,3,4,1,4,4,2,1,4,4,3,5,2,5,4,1,5,1,1,1,4,5,3,4,3,4,2,2,2,2,4,5,3,5,2,4,2,3,4,1,4,4,1,4,5,3,4,2,2,2,4,3,3,3,3,4,2,1,2,5,5,3,2,3,5,5,5,4,4,5,5,4,3,4,1,5,1,3,4,4,1,3,1,3,1,1,2,4,5,3,1,2,4,3,3,5,4,4,5,4,1,3,1,1,4,4,4,4,3,4,3,1,4,5,1,2,4,3,5,1,1,2,1,1,5,4,2,1,5,4,5,2,4,4,1,5,2,2,5,3,3,2,3,1,5,5,5,4,3,1,1,5,1,4,5,2,1,3,1,2,4,4,1,1,2,5,3,1,5,2,4,5,1,2,3,1,2,2,1,2,2,1,4,1,3,4,2,1,1,5,4,1,5,4,4,3,1,3,3,1,1,3,3,4,2,3,4,2,3,1,4,1,5,3,1,1,5,3,2,3,5,1,3,1,1,3,5,1,5,1,1,3,1,1,1,1,3,3,1)";

