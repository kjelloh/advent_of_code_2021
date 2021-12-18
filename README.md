# advent_of_code_2021
My solutions to the Advent of Code 2021 at https://adventofcode.com

day0 is for cross compilation and preparation tests only ;)

These projects ALL builds and runs on macOS, Debian 10 and Windows 11.

Projects files includes,

  VSCode for macOS (g++-11) and Debian 10 (g++8).
  Visual Studio 2022

C++ Source code requires between C++14 and C++20 (C++2a on Visual Studio 2022)
# day 17
Lessons learned in day 17.
* Don't try to be clever and define a range so that start is closer to 0 than end!

```
CoordRange coord_range(std::string const& sc1, std::string const& sc2) {
  auto c1 = std::stoi(sc1);
  auto c2 = std::stoi(sc2);
  // Range so that the coord closer to 0 is first
  if (std::abs(c1) < std::abs(c2)) {
    return { c1,c2 };
  }
  else {
    return { c2,c1 };
  }
}
```
  * This messes up all checks of wether a value is "before" or "after" the range!
  * By having the range defines so that "start" is always less than "end" the check for "below" and "above" a range is the less-than operator (but see below about "before" and "beyond" still being direction dependent!)
  * I have no idea why I felt it necessary to screw this up? Somehow my intuition was the other way around *sigh*
  * Here is the "correct" and safe range defintion :)
```
CoordRange coord_range(std::string const& sc1, std::string const& sc2) {
  auto c1 = std::stoi(sc1);
  auto c2 = std::stoi(sc2);
  // Range so that the coord closer to 0 is first
  if (c1 < c2) {
    return { c1,c2 };
  }
  else {
    return { c2,c1 };
  }
}
```
* The second trap I fell into was to try to define "before" and "beyond" a range as functions. But I failed to realise that "before" and "beyond" are concepts that depends on the direction we are traveling in!
* So I needed to define "before" and "beyond" to take a direction and have "in range" call them with zero direction.
* I tried this...
```
bool beyond_range(int coord, int dc, CoordRange const& range) {
  return (dc>=0)?(coord > range.second):(coord < range.first);
}
bool before_range(int coord, int dc ,CoordRange const& range) {
  return (dc>=0)?(coord < range.first):(coord > range.second);
}
bool in_range(int coord, CoordRange const& range) {
  return (before_range(coord,0, range) or beyond_range(coord,0,range)) == false;
}
```
* NOTE: This does NOT work either!
    * "beyond" and "before" can't be called with velocity (dc) zero.
    * In the end I scrapped the "beyond" and "before" helper functions and defined "in_range" stand-alone.

* Then for part 2 I thought I was to test all the losted shots top see how many of them hiot the target.
    * Fortunately it did not take me so much time to parse the list of example shots and apply them to count the hits.
    * It took me a little longer to realise why my answer was wrong.
* I am a meta-thinker. Meaning, I do what I experience is the intention, not what is actually said...

# day 12
I came this far but this does not work. Why?

```
Paths all_paths(std::string const& v1,std::string const& v2,Graph const& graph,VisitCount visit_count) {
  Paths result{};
  Path new_path{};
  visit_count[v1] += 1;
  new_path.push_back(v1); // A-end
  if (v1 != v2) {
    for (auto const v : graph.at(v1)) { // v: c,start, b, end
      // v is connected to v1
      if (visit_count[v]==0 or (v.size() == 1 and std::isupper(v[0]))) {
        Paths paths = all_paths(v,v2,graph,visit_count); // 
        for (auto const& path : paths) {
          new_path.insert(new_path.end(),path.begin(),path.end());
          result.push_back(new_path);
          // print
          {
            std::cout << "\ncandidate path: ";
            for (auto const& v : new_path) {
              std::cout << " -> " << v;
            }
          }
        }
      }
    }
  }
  else {
    result.push_back(new_path);
  }
  return result;
}
```

# day11
The lesson after this days puzzle is to remember to KISS (Keep it simple stupid) ;)
* I tried to be clever form the start and squeeze the processing through standard C++ library algorithms. I failed. I beleive it may still be possible but for this small puzzle I am not sure it is worth the overhead?
* I failed to see through the weeds of details, that I can mutate the same grid of crabs to get the answer (I do not need a temp grid to mutate and them switch back for each flash mutations of the grid).
* I want to learn to solve these puzzles in small iterations with a lot of checks and logging of each progress. So I know I am on the right track. I can use the approach applied by the fastest Python programmers when they solve these puzzles :).
* I Do have a tendancy to go for the cleanest solution right away. But so far this mostly means a get stuck in compiler errors in my "smart" wrapper classes 8-)
* I am still not satisfied with the final code as it re-uses an automata class for part 1&2 that relies on mutaable referbces to local variables. I suppose we will see more cellular automatas in the 2021 puzzles to come so I try to design my automata in a more elegant way then ;)
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
bool beyond_range(int coord, CoordRange const& range) {
  return (coord > range.second);
}
bool before_range(int coord, CoordRange const& range) {
  return (coord < range.first);
}
bool in_range(int coord, CoordRange const& range) {
  return (before_range(coord, range) or beyond_range(coord, range)) == false;
}
```
# day4
For me the Day 4 puzzle challenged me the same way as day 3 did!
* Again I found myself being dragged into rabbit holes of ever and ever more details and entagled data and logic.
* In fact - I have a hunch day 4 is day 3 in disguise? I mean, both these puzzles has a grid of data as input. We need to accumulate over rows or columns to extract infromation.
* If I have time I want to come back to this puzzle and see if I can solve day 3 and day 4 with some common framework? 
# day3
Note: 211210 - Solved the puzzle from scratch and got a somewhat cleaner code ;)

Originally - The day 3 puzzle was one of those I easilly stumble on due to all small +/- one and true/false problems.
* I failed to design the code so that I did not shoot myself in the foot over and over again (detail overflow). Just look at this exmple...
```
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
```
* For me part of the challange of this puzzle was that I falied to name "things" properly so that my code could "talk" about the problem in a clear way.
* After I finished I was left with the feeling that this problem was in fact just a transform + accumulate problem? If I have time I want to come back to is and solve it properly.

# day2
For this puzzle I struggled mostly with parsing the input.
* I stuck with the std::sregex_token_iterator IContainer from day 1 as I thought maybe I could use this approach to parse all puzzle inout to come. But the result was that parsing become overly complicated.

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
