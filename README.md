# advent_of_code_2021
My solutions to the Advent of Code 2021 at https://adventofcode.com

day0 is for cross compilation and preparation tests only ;)

These projects ALL builds and runs on macOS, Debian 10 and Windows 11.

Projects files includes,

  VSCode for macOS (g++-11) and Debian 10 (g++8).
  Visual Studio 2022

C++ Source code requires between C++14 and C++20 (C++2a on Visual Studio 2022)

# day1
Pitfalls for me in C++ for this puzzle.
* I wanted to try C++20 std::ranges which was overkill for this puzzle and only created more work than called for.
* To make the puzzle input become a range of integers I used std::sregex_token_iterator (I think std::istream_iterator would have sufficed for the simple input in this case)

```
Result part1(auto const pData) {
    std::string sdata{ pData };
    const std::regex ws_re("\\s+"); // whitespace
    auto sdepths = IContainer<std::sregex_token_iterator>{ std::sregex_token_iterator(sdata.begin(), sdata.end(), ws_re, -1) };
    auto prev{ 0 };
    auto depths = sdepths
        | std::views::transform([](auto const s) {return std::stoi(s); })
        | std::views::transform([&prev](auto const d) {auto result = d > prev; prev = d; return result; });
    auto result = std::accumulate(std::begin(depths), std::end(depths), 0, [](auto const& acc, auto const& b) {return b ? acc + 1 : acc; });
    return result-1;
}
```
* I hand-rolled a helper struct to turn the regex iterator into an actual range (begin,end) (I now think std::ranges::subrange does the very same thing?)

```
template <class I>
struct IContainer {
    I iter;
    I begin() { return iter; }
    I end() { return I{};}
};
```
* For part 2 I had to add internal states to the range adaptors to handle the 3-value window processing.
```
    std::array<int, 3> w = { 0,0,0 }; // Initial "window"
    auto prev{ 0 }; // previous value for adjacent compare
    const std::regex ws_re("\\s+"); // whitespace
    auto sdepths = IContainer<std::sregex_token_iterator>{ std::sregex_token_iterator(sdata.begin(), sdata.end(), ws_re, -1) };
    auto depths = sdepths
        | std::views::transform([](auto const s) {return std::stoi(s); })
        | std::views::transform([&w](auto const d) {
            std::rotate(std::rbegin(w), std::rbegin(w) + 1, std::rend(w));
            w[0] = d;
            return w; })
        | std::views::transform([](auto const a) {return std::accumulate(std::begin(a),std::end(a),0); })
        | std::views::transform([&prev](auto const d) {auto result = d > prev; prev = d; return result; });
```
* At first I used static members for the windows w and prev variables but then these values would of course fail to initiate for all but the first call ;)
# day2
For this puzzle I struggled mostly with parsing the input.
* I stuck with the std::sregex_token_iterator IContainer from day 1 as I thought maybe I could use this approach to parse all puzzle inout to come. But the result was that parsing become overly complicated.
# day3
The day 3 puzzle was one of those I easilly stumble on due to all samll +/- one and true/false problems.
* I failed to design the code so that I did not shoot myself in the foot over and over again.
* For me part of the challange of this puzzle was that I falied to name "things" properly so that my code could "talk" about the problem in a clear way.
* After I finished I was left with the feeling that this problem was in fact just a transform + accumulate problem? If I have time I want to come back to is and solve it properly.
# day4
# day5
# day6
# day7
# day8
# day9
Pitfalls for me in C++ for this puzzle.

* C++ standard library operator<< to std::cout treats size8_t as a char! So when I made a map of size8_t values and tried to print it it got gibberish and lost some hair and time trying to figure out what went wrong.
* Using std::transform to expand the frontier of a basin I had to remember to use std::back_inserter to get the new candidates into the candidate list. I also had to flatten the candidate list as the transform created a vector of vectors. NOTE that using the begin() interator to the result will compile but fail to expand the output.

```
        std::vector<Points> points_vector{};
        std::transform(frontier.begin(), frontier.end(), std::back_inserter(points_vector), [](auto const p) {
            Points candidates{ {p + LEFT},{p + UP},{p + RIGHT},{p+DOWN} };
            return candidates;
            });
```

* At first I imagined for part 2 that I needed to expand also to diagonal points on the map. But that will leak as the input data does NOT define basin boundaries in this way.
* To make my own Point struct work in std::set I needed to define an operator< for it. And I defind it by calculating an integer from the x and y coordinates by offsetting the x by 10000. This is a quick-and-dirty way and I would not recommend it (unless you are sure y will never be greater that 10000) ;).

```
struct Point {
    int x, y; // x horizintal (+ = right), y vertical (+ = down)
    bool operator<(Point const& other) const {
        return (x*10000+y < other.x*10000+other.y);
    }
};
```

