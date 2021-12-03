#include <iostream>
#include <sstream>
#include <iomanip> // std::quoted
#include <regex>
#include <string>
#include <ranges>
#include <string_view>
#include <map> 
#include <numeric>
#include <bitset>
#include <array>
#include <utility> // std::pair

extern char const* pTest1;
extern char const* pData;

using Result = size_t;

namespace part2 {

    struct Meta {
        bool active_oxygen_generator_rating{true};
        bool active_CO2_scrubber_rating{ true };
    };
    using Entry = std::pair<Meta, size_t>;
    using Report = std::vector<Entry>;

    template <int BIT_COUNT>
    struct Rates {
        size_t oxygen_generator_rating_count{};
        size_t CO2_scrubber_rating_count{};
        std::array<size_t, BIT_COUNT> bit_counts;
    };

    template <int BIT_COUNT>
    Rates<BIT_COUNT> rates_of_report(Report const& v,auto predicate) {
        Rates<BIT_COUNT> result{};
        result = std::accumulate(std::begin(v), std::end(v), Rates<BIT_COUNT>{}, [&predicate](auto acc, auto entry) {
            if (entry.first.active_oxygen_generator_rating) ++acc.oxygen_generator_rating_count;
            if (entry.first.active_CO2_scrubber_rating) ++acc.CO2_scrubber_rating_count;
            if (predicate(entry)) {
                std::cout << "\n\tprocess " << std::bitset<BIT_COUNT>{entry.second}.to_string();
                std::bitset<BIT_COUNT> bit_set{ entry.second };
                for (int i = 0; i < BIT_COUNT; i++) {
                    if (bit_set[BIT_COUNT - i - 1]) ++acc.bit_counts[i];
                }
            }
            return acc;
            });
        std::cout << "\nrates : oxygen_generator_rating_count=" << result.oxygen_generator_rating_count;
        std::cout << "\nrates : CO2_scrubber_rating_count=" << result.CO2_scrubber_rating_count;
        std::cout << "\nrates : ";
        for (auto n : result.bit_counts) std::cout << " " << n;
        return result;
    };

    auto active_oxygen_generator_rating = [](auto entry) {
        return entry.first.active_oxygen_generator_rating;
    };

    auto active_CO2_scrubber_rating = [](auto entry) {
        return entry.first.active_CO2_scrubber_rating;
    };

    template <int BIT_COUNT>
    Result answer(char const* pData) {
        Result result{};
        std::cout << "\npart 2 :)";
        // #1 Tokenize input to vector if integers (diagnostic report data)
        Report v{};
        std::istringstream in{ pData };
        std::string sNumber{};
        while (in >> sNumber) v.push_back(Entry{ Meta{true},std::bitset<BIT_COUNT>{sNumber}.to_ullong() });
        for (auto const& n : v) std::cout
            << "\nentry : meta{active_oxygen_generator_rating="
            << n.first.active_oxygen_generator_rating
            << "} rates {"
            << std::bitset<BIT_COUNT>{n.second}.to_string()
            << "}";

        // #2 std::accumulate bit-counts for each "column" (digit index)
        auto rates = rates_of_report<BIT_COUNT>(v, active_oxygen_generator_rating);
        // #3 transform the diagnostic report according to the "oxygen generator rating" criteria until one remains
        std::cout << "\n<< oxygen_generator_rating >>";
        for (int i = 0; (i < BIT_COUNT) and (rates.oxygen_generator_rating_count > 1);  i++) {
            std::cout << "\nindex:" << i+1;
            size_t zero_bit_count_i = rates.oxygen_generator_rating_count - rates.bit_counts[i];
            bool most_common_digit_i_is_one = (rates.bit_counts[i] >= zero_bit_count_i);
            if (rates.bit_counts[i] == zero_bit_count_i) {
                std::cout << " equal one and zero counts ==> Keep if '1'";
            }
            std::transform(std::begin(v), std::end(v), std::begin(v), [&i,&most_common_digit_i_is_one](auto entry) {
                if (entry.first.active_oxygen_generator_rating) {
                    std::bitset<BIT_COUNT> bit_set{ entry.second };
                    std::cout << "\ntransform " << bit_set.to_string() << "[" << i+1 << "]";
                    bool do_keep = (most_common_digit_i_is_one == bit_set[BIT_COUNT - i - 1]);
                    if (do_keep) {
                        std::cout << " Keep :)";
                    }
                    entry.first.active_oxygen_generator_rating = do_keep;
                }
                return entry;
                });
            rates = rates_of_report<BIT_COUNT>(v, active_oxygen_generator_rating);
        }
        auto oxygen_generator_rating_iter = std::find_if(std::begin(v), std::end(v), [](auto entry) {
            return entry.first.active_oxygen_generator_rating;
            });
        std::cout << "\noxygen_generator_rating" << std::bitset<BIT_COUNT>{oxygen_generator_rating_iter->second}.to_string();

        // #4 do the same according to the "CO2 scrubber rating" criteria until one remains
        rates = rates_of_report<BIT_COUNT>(v, active_CO2_scrubber_rating);
        std::cout << "\n<< CO2_scrubber_rating >>";
        for (int i = 0; (i < BIT_COUNT) and (rates.CO2_scrubber_rating_count > 1); i++) {
            std::cout << "\nindex:" << i + 1;
            size_t zero_bit_count_i = rates.CO2_scrubber_rating_count - rates.bit_counts[i];
            bool most_common_digit_i_is_one = (rates.bit_counts[i] >= zero_bit_count_i);
            if (most_common_digit_i_is_one) {
                std::cout << "\nKeep if ZERO (minority)";
            }
            else {
                std::cout << "\nKeep if ONE (minority)";
            }
            if (rates.bit_counts[i] == zero_bit_count_i) {
                std::cout << " equal one and zero counts ==> keep if '0'";
            }
            std::transform(std::begin(v), std::end(v), std::begin(v), [&i, &most_common_digit_i_is_one](auto entry) {
                if (entry.first.active_CO2_scrubber_rating) {
                    std::bitset<BIT_COUNT> bit_set{ entry.second };
                    std::cout << "\ntransform " << bit_set.to_string() << "[" << i + 1 << "]";
                    // Keep if have digit in minority
                    bool do_keep{};
                    if (most_common_digit_i_is_one) do_keep = (bit_set[BIT_COUNT - i - 1] == false); // Keep '0'
                    else do_keep = (bit_set[BIT_COUNT - i - 1] == true); // keep '1'
                    if (do_keep) {
                        std::cout << " Keep :)";
                    }
                    entry.first.active_CO2_scrubber_rating = do_keep;
                }
                return entry;
                });
            rates = rates_of_report<BIT_COUNT>(v, active_CO2_scrubber_rating);
        }
        auto CO2_scrubber_rating_iter = std::find_if(std::begin(v), std::end(v), [](auto entry) {
            return entry.first.active_CO2_scrubber_rating;
            });
        std::cout << "\aCO2_scrubber_rating" << std::bitset<BIT_COUNT>{CO2_scrubber_rating_iter->second}.to_string();

        // #finally the result life support rating = oxygen generator rating x CO2 scrubber rating
        result = oxygen_generator_rating_iter->second * CO2_scrubber_rating_iter->second;
        // Constraints
        // #1 Do NOT copy report entries around (we need only to transform meta-data about the entries)
        // #2 Process binary numbers as integers (and use math and not topology to do the matching, filtering and and counting)
        // #3 Identify and use as much C++ standard library functionality as possible (avoid NIH)

        std::cout << "\n";
        return result;
    }
}

namespace part1 {
    Result answer(char const* pData) {
        Result result{};
        std::istringstream in{ pData };
        std::string sBits{};
        std::vector<std::string> v{};
        std::map<int, std::vector<char>> m{};
        while (in >> sBits) {
            std::cout << "\nbits : " << sBits;
            for (int i = 0;  i < sBits.size();i++) {
                m[i].push_back(sBits[i]);
            }
        }
        std::string gamma_rate_s{ sBits };
        std::string epsilon_rate_s{ sBits };
        for (auto [i,v] : m) {
            int count_zeroes = std::accumulate(std::begin(v), std::end(v), 0, [](auto acc, auto ch) {
                return acc += (ch == '0')?1:0; });
            int count_ones = std::accumulate(std::begin(v), std::end(v), 0, [](auto acc, auto ch) {
                return acc += (ch == '1')?1:0; });
            gamma_rate_s[i] = (count_zeroes > count_ones) ? '0' : '1';
            epsilon_rate_s[i] = (count_zeroes < count_ones) ? '0' : '1';
        }
        std::cout << "\ngamma rate : " << gamma_rate_s;
        Result gamma_rate{};
        for (char ch : gamma_rate_s) {
            gamma_rate *= 2;
            gamma_rate += (ch - '0');
        }
        std::cout << "\ngamma_rate : " << gamma_rate;
        std::cout << "\nepsilon rate : " << epsilon_rate_s;
        Result epsilon_rate{};
        for (char ch : epsilon_rate_s) {
            epsilon_rate *= 2;
            epsilon_rate += (ch - '0');
        }
        std::cout << "\nepsilon_rate : " << epsilon_rate;
        result = gamma_rate * epsilon_rate;
        std::cout << "\n";
        return result;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "\nWelcome :)";
    std::map<std::string, Result> answer;
    // answer["part 1 test"] = part1::answer(pTest1);
    // answer["part 1"] = part1::answer(pData);
    answer["part 2 test"] = part2::answer<5>(pTest1);
    answer["part 2"] = part2::answer<12>(pData);

    for (auto [caption, result] : answer) {
        std::cout << "\nanswer[" << caption << "] : " << result;
    }
    std::cout << "\nPress <enter>...";
    std::cin.get();
    std::cout << "Bye!";
    return 0;
}

char const* pTest1 = R"(00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010)";;
char const* pData = R"(000010000011
001010000111
011010000010
000011111110
101101000101
000100010100
111101111110
111111000010
000011010011
100011100000
101110110001
111011111101
111011011001
010101000010
000010001000
101010011111
010000111101
011010111100
101111010110
000110110101
001010000001
110101100110
111110010100
001000001011
100111101001
000010010001
100100101010
011010110111
000001100100
101111000111
010101101100
001111010010
010001011100
110101011011
000010101000
011011101101
101001000011
101111000000
001111101111
001101110111
110110110101
100011001011
010110110011
001010001110
110110100100
001001111111
100111011100
101101001010
011110000011
010101111000
001101000101
100011001100
110000110001
011101010001
000110001101
110001101101
011001000110
011101010101
110110011010
001000011001
100101100101
010110001100
110100011001
001011000010
101011110101
011101001010
011011000111
000101000001
100001010110
111101000011
111010111011
000011101001
011111110000
110011100101
110011100110
101011001011
100110110111
010001111100
101010011001
111001111011
111001000011
010010111010
011011011110
101000010000
000010011111
000101110000
101010000110
101100000100
110000110000
000010100010
010001111001
001000001111
010110000011
010000111110
001011001010
011010100001
001001000110
011010001101
100111011101
011101010000
100111010100
100101001101
111010101001
011001011101
010011000000
000001100110
110101011100
000110101000
011110001000
010011000001
100011011111
100100001111
010011101010
001011000001
111101011000
011101000110
111011001101
110110010110
111011000110
111111010101
101110100111
011110010001
010100011111
001111000101
101100010011
111101000010
100001110100
110001110011
011011110101
100110111101
110010011010
111001000110
100010111111
100010101110
011110111001
011100101100
010101111101
101010111011
111111110100
010111100110
111010101111
011001110000
101011100011
101000000110
111100011011
001001010010
001010110001
000111101101
100010000101
010001101110
000100100001
010100011000
001100011011
111111110011
111010010000
001100101001
000101001110
111100111010
111110010000
010111001000
011100001100
000010011010
101010101011
110100011011
010110101101
000001000101
111111010011
110000000110
110100111111
111000000010
000101010111
001110110110
111111111011
010010100100
110000010011
010111110111
111000010010
111011100000
010100000101
010011011001
000110101111
110111101111
101001001011
101111110111
111010101011
100100010101
100100011100
000011010111
011010101011
101000111010
010101011101
101101000001
010111110010
111101011010
000001010100
011010101000
010000110010
101000011100
010001010000
000001001101
100101110001
001101010010
001100111110
011101010111
111110011100
100011000101
101111100100
111011001000
010111000110
001010111100
011011110001
111010011110
101011100110
001100010111
111110110111
101011010001
011110100011
110000001110
101011101011
101101000111
110111111111
010000011000
001110001000
111000000101
010111101100
000100000101
001111100101
101001111101
010111110101
100110010101
111101101100
111100010010
110011111110
011100010101
111001001100
101010110000
010100010010
111100110000
111100001000
111100110110
101101110010
010001101100
010110010000
100000100101
000000001110
011001100110
100101000010
011111100010
010010100010
100101011001
011100100100
010111110000
100111000110
111110011011
011111000011
101001100101
000110101110
111001111110
011110011101
101011111111
010110101010
100001100111
000101001111
110100011101
111110100111
101000111110
010010011110
100110100111
000111010011
110101110110
101101000000
110110111110
001100111111
000010101010
001001001001
101000110110
010011111000
000101110011
011100001111
000001101110
100001010010
001100000011
011100101110
111000111000
110010110111
100100000010
010101100100
100111001101
011101100100
110010001010
100111000000
111011001100
010010010011
110110100001
001011110001
010000100110
001111100100
100110001000
101000010001
001000001000
110001001011
001010010101
101001111011
011101110010
101000100110
010000101110
000110010110
001111010001
011010100111
101110011001
001010001100
010001011010
110010100010
100001111001
001001010001
010111101110
100100111111
100100101111
011010100011
101100011101
000001011110
111010111111
101011010011
111000110001
111010100100
000100001010
010011011010
110100100011
010001011101
011100110100
100111111100
100110010111
111101010010
011110101111
001101010001
001111000001
101010101101
100001111110
100111010001
001111110111
011110111101
000101010010
111110001111
100110011001
101000110101
100110000110
000100100010
000101110010
010011110110
011000111011
011111111111
000001100111
101001010100
010011101111
101000101100
111011100011
001101011011
001110101001
001001001010
001110111011
110100111011
110110010101
010001111101
001101111101
110001100110
110001000101
010001001001
110001101010
100110011000
001001010011
100000111010
111000000110
101010101110
011011011111
100011111111
110000011101
000110100001
100000110000
010100100110
100111111011
101110100110
011111111110
111101011101
010011100111
111110101001
111111011111
000100001000
101111110010
001010011011
010001001111
010111110100
100111110001
011000000011
000010110110
011110100001
100001011110
110111001011
100100010110
010101110001
011011111011
101011001110
110110110000
001101000011
101011100101
000001111101
001000011101
010110000010
000100011000
111011001010
100111111001
100101000100
100100001010
110010111101
110011100000
110001110101
001110011100
000001011100
101110101010
110000111100
100001011100
101010001001
100100111000
011110111110
101110111101
110001011101
000111110110
011000110010
100001101110
100111011111
010000000000
110011111010
000111000000
111000100010
110111100010
110100001110
110101101110
110111011110
101010000011
000010000101
011111111011
100110100000
111100101011
000010001101
001001100001
111010111001
101000001011
000000010010
100010000111
111111111110
000001011010
010110101000
010101000011
110011101010
100101101110
111100100000
100000000010
000110101101
011011110000
111011101101
011011101111
010100000001
101110000000
110001001111
001001011011
010101000100
011000111110
010110111001
110111001000
000101000101
101011000011
111000001110
111011110011
100111000101
110000111001
100011010000
111110010001
001100010101
110100010010
100100101100
101010101100
001000111100
110010101010
100011001101
000001110111
010101010111
100000001111
100010011001
010010101000
111100001110
101010001101
110011000001
011101011101
001100001111
101111111111
111010101000
110001111010
111111001010
011111001010
010001010100
101010100011
100111110110
111101100101
101001100110
010001011111
010011001111
101110011010
111011000111
011011111101
000001001110
101000010110
010110010100
000110100101
011000000010
011001001000
101100000011
111001100011
010101001001
011100110111
101111100110
111001111010
000000110110
001110011101
101110001011
011000010001
111111000011
101111000110
001010101010
011110111011
101000101010
011111000110
101011010110
001111001011
001011111111
111011010011
001101011010
000111010001
110011111001
100010101100
010001100111
011110110100
100111100101
101110001000
000100110001
100110110110
000111101110
110111101100
001001101001
100111100100
110100000100
110000100100
111110011010
100111010000
000111011100
101011110001
101110111110
001111111000
001010100100
000001111011
100001001110
010000110000
110011001110
001101111110
101100101100
010101011111
011000001101
111111101000
100011010110
101000011000
100011110101
111001110011
111001011001
110011100010
111110110011
101110011100
001111100010
000101010011
100011101101
011110110110
010011000101
010100101110
001100010010
010111001011
111000011000
010101001000
100011100101
111101001110
100111110101
110001000111
110100011100
010101010000
101001010101
001110000001
010100111010
100111011000
110100000011
000011100110
001011100111
111010110101
001100101000
111001101101
011101100101
110110111000
000010010011
010111101001
111110111010
100011010001
111011111100
101001110000
111100101100
011001010000
101111001101
100111001111
011011000011
101001001100
001101001100
110001001010
111100011010
101000010101
110001011111
011010001111
011101000100
010100100001
010110100001
101110100001
010000100010
010000111111
110111010111
010100000011
111111111000
101111101100
011110110001
111001010000
011101101000
001111110100
000101111011
101110111010
001111001001
111111110101
011011100100
101101101100
110111101001
000000111001
010100010000
100001001010
111010100111
101100001111
110001001001
011001001001
001110101100
001001110101
101011111011
010011011000
001010010000
001010001101
011000001110
100111111110
011011010011
011100011101
101100110100
101111000101
100111100110
110110000100
101111100001
000100011101
010100101111
000000001000
010110010010
000000000010
111010010001
111010111101
001001000011
101001010010
100000001101
101111101000
011100001011
110111100111
111111000110
110101001010
010001011110
000101100001
000011100100
101101110101
011110100100
110110101101
000010010111
100011010111
101000110001
011111010110
111001001010
000011000111
001111001111
101011001000
100000111111
110100011110
000000100110
101110010001
010011011110
100100011111
010101111100
001011010100
110011101111
001000101010
001101010100
001111111010
000101100000
110101100101
110010111010
101110001100
110100111000
001100000110
001000101100
001010101111
100010100001
011001101100
110000101100
000111001001
100001011011
000110010000
110100100100
111101101111
111001100110
101010001110
101001011110
101001111100
000100101010
111100011000
100100001100
101100001010
100000100001
001110010011
011100011110
000000101010
011000010111
010011110111
001111101010
000011110000
110001111011
011001110010
001110010010
110011110011
111111100111
101101100101
110101111100
100011110011
001111011010
111110111100
101110001111
101001111000
100011100111
110010001011
000010000110
110001010011
101011011110
001111101001
000010001011
110000101101
111111100011
100111001100
101110111000
111010000001
101100100100
110111000111
000111111110
111111000100
100000001001
101101100010
000011001101
111000001101
110111010110
010110100011
101000101000
010010101101
110011011110
111000000000
011110111010
000000010011
100110110101
101100011100
111111100010
100110001001
100011000010
011010000111
111101000111
111000011101
101111101010
100110100100
010101011110
001000011010
110101000100
001100100101
010100011110
001100110101
101101000010
010100111100
110000000000
101011101001
111100110010
100111010010
010000001110
011011010000
011011100111
110101000011
100100101000
000110001001
110001010110
100100001101
010001110001
101110101111
100110001010
011100110101
001001100100
001101100001
100010101111
101111101101
111110000010
001100001100
111111001000
001111001010
000000100111
001011010011
011110000111
010111011110
111110000011
100000111000
010010111101
010000000011
010111110110
011000100101
110011101011
001100101010
101101001000
000100000000
111010110110
100101001110
001000001110
011010111000
110110011000
110011001001
101000000101
010110011101
110110010111
010011110000
111110011000
100010101001
110111000001
001000110001
101011001100
111111100101
011110010111
011001111100
010010011010
001111011110
110001100010
001000110011
110101101101
110010011100
010111101010
001110101010
010101101000
001011000110
010101111011
101100001100
110100010100
001101000010
101110110110
001100100111
101010001010
110000100110
010100110111
101111110101
110000101001
000001010001
001000000011
101100100101
111001001111
010100001001
100001010111
000010110000
001110001110
111010000111
100100111011
101101010011
100010110100
110001001100
110001010010
101100100010
011100100011
010110000110
000011010000
000100111100
111011110111
001101101111
001101011101
010110101011
000100100100
100001100001
100001110000
010101111001
001011011001
111100000100
011000001010
111110001011
000001101111
000101011011
011010011100
001001110011
011010011001
100010011100
010000110001
111101100001
011011010100
010011111011
101000010011
100101111101
000100111010
101110111100
111100111101
000111000110
111100000110
001010100110
000000100011
110010001001
101111001001
100101100010
111101001011
001110111000
100001000011
100101100000
100010010100
010001000110
101100111100
101011101110
001001101111
010110000000
010011100011
001001110110
101001011000
000011001000
010010011001
101110010010
000101010110
111100101000
111000100101
101110000111
000110011011
101101001101
001000111101
110111000100
000001001010
111100001010
001011101110
111001000000
001111111011
101100000101
001001011111
001001000100
000001010000
101010000100
010010111111
101010101000
111101111011
001111010110
001010011010
001101001011
100001000100
001111110101
011000100010
101111001100
101110110011
101101110111
001100100011
110010000111
011110110000
101100011010
100110010010
000100101111
011000011010
110000010000
001100101100
111110100100
101010001000
101111110000
101011110010
100101101000
100010100011
011011011000
000001010110
100100100101
111100101010
100010110000
100100111010
001010110100
110011100111
010001010001
110100011010
100011111110
101001010000
011100100110
111000101010
001111001000)";

