# advent_of_code_2021
My solutions to the Advent of Code 2021 at https://adventofcode.com

day0 is for cross compilation and preparation tests only ;)

These projects ALL builds and runs on macOS, Debian 10 and Windows 11.

Projects files includes,

  VSCode for macOS (g++-11) and Debian 10 (g++8).
  Visual Studio 2022

C++ Source code requires between C++14 and C++20 (C++2a on Visual Studio 2022)
# day 23
Pitfalls I fell into when solving this puzzle
* Vexing C++ parsing anyone?!

```

      if (move1.to.row==1) heuristic1 += 100+(move1.to.col<3)?10:0+(move1.to.col>9)?10:0; 
      if (move2.to.row==1) heuristic2 += 100+(move2.to.col<3)?10:0+(move2.to.col>9)?10:0;
```

*The 100 is never added! The following code does what I intended...*

```

      if (move1.to.row==1) heuristic1 += 100+((move1.to.col<3)?10:0)+((move1.to.col>9)?10:0); 
      if (move2.to.row==1) heuristic2 += 100+((move2.to.col<3)?10:0)+((move2.to.col>9)?10:0);
```
*It turns out the ternary operator requires enclosing parentheses to not evaluate also the 100+ before the '?'!*

* Apart from that I REALLY struggled with getting all the rules right to get the state search space correct!
* The final key, It think, was my added heuristics to shuffle the moves to try in an order that really sped up the search

```

std::vector<std::pair<Move,Cost>> apply_strategy(std::pair<State,Cost> const& state_cost, std::vector<std::pair<Move,Cost>> const& potential_steps) {
  // Rearrange the order of the potential steps to enhance our finding of a solution
  // Assume potential_steps are valid steps (no self to self or move out of a room where pod is already home etc.)
  std::vector<std::pair<Move,Cost>> result{potential_steps};
  auto& [state,cost] = state_cost;
  // Invent a heuristic to assign more or less value to certain steps
  // NOTE: I implemented a hunch about what good steps are from the example solution in the puzzle description.
  //       As long as we do not drop any moves we are at least safe, but can make solving the possible more or less time consuming

  // note: C++ issue with capturing a structured binding in the lambda below...
  // note: Structured binding introduces names, not variables (https://stackoverflow.com/questions/54842919/k-in-capture-list-does-not-name-a-variable?noredirect=1&lq=1 )
  // note: Not exactly sure about the semantics though (name vs variable?)
  // note: clang reports error while gcc accepts
  // note: the following code works in both gcc and clang (capture capture by reference to variable state from structured binding name state)
  std::sort(result.begin(),result.end(),[&state=state](auto const& step1,auto const& step2){
    int heuristic1{0},heuristic2{0};
    auto& [move1,cost1] = step1;
    auto& [move2,cost2] = step2;
    char type1 = state[move1.from.row][move1.from.col];
    char type2 = state[move1.from.row][move1.from.col];
    auto home1 = home_pos(type1,state);
    auto home2 = home_pos(type2,state);
    // Prefer to a home room (any order)
    if (home1) heuristic1 += 10000;
    if (home2) heuristic2 += 10000;
    // prefer from room that is not yet a home, D room before "lesser" rooms
    if (!home1) heuristic1 += 1000*move1.from.col; 
    if (!home2) heuristic2 += 1000*move2.from.col;
    // Prefer alcoves before blocking hallway positions
    if (move1.to.row==1) heuristic1 += 100+((move1.to.col<3)?10:0)+((move1.to.col>9)?10:0); 
    if (move2.to.row==1) heuristic2 += 100+((move2.to.col<3)?10:0)+((move2.to.col>9)?10:0);
    return heuristic1>heuristic2; // prefer steps with highest heuristic
  });
  return result;

```
# day 22
Pitfalls I fell into when solving this puzzle
* I was blinded by auto type deduction...
* I used auto for the volume type calculations from int coordinates, causing silent and devastating integer overflow for part 2!

```
              auto dx = (x_boundaries[on_off_x+1] - x_boundaries[on_off_x]);
              auto dy = (y_boundaries[on_off_y+1] - y_boundaries[on_off_y]);
              auto dz = (z_boundaries[on_off_z+1] - z_boundaries[on_off_z]);
              auto volume = dx*dy*dz;

```
* When I got the right answer for part 1 but the wrong one for the large numbers in part 2 I suspected integer overflow. And I did check that I had type Result of sufficient size.
* BUT - for some reason it escaped me that the volume calculation input was coordinates and they where ints!
* The fix was then easy.

```
              Result dx = (x_boundaries[on_off_x+1] - x_boundaries[on_off_x]);
              Result dy = (y_boundaries[on_off_y+1] - y_boundaries[on_off_y]);
              Result dz = (z_boundaries[on_off_z+1] - z_boundaries[on_off_z]);
              auto volume = dx*dy*dz;

```


# day 21
Pitfalls I fell into when solving this puzzle.
* At first, for part 2, I got the impression that the quantum dice produced the same outcome as in part 1. The only difference was that it split the universe on each roll.
    * Thankfully it turned out this was not the case ;)  
* for part 2 I need to count all permutations of three rolls of the quantum dice.
    * My first attempt was to loop over 3..9 (possible moves from three rolls of dice 1,2 or 3)
    * BUT - Each of these permutations counts as separate "universes" even if the actual move is the same.
* I first made a solution for part 2 with an argument defining what players turn it was.
    * This caused the recursion to contain "mirrored code" which caused some problems to get it right in all its details.
    * Actually - I failed to get it right (it gave the wrong answer)
    * Only after I had refactored the code so that each recursion switched between player 1 and player 2 state to mimic "taking turns" - did my code produce the right answer.
    * I may never know where I screwed up in my first attempt...
# day 20
Pitfalls I fell into when solving this puzzle.


            std::string bs{};
            std::transform(line.begin(),line.end(),bs.begin(),[](char ch) {return (ch=='#')?'1':'0';});


* using std::transform into an empty string without using std::back_inserter.
    * Runtime error "EXC_BAD_ACCESS (code=EXC_I386_GPFLT)" in another place on the code.
    * The transform will access memory (indexing into bs) that is not there to access and corrupt the state of the application!
 
# day 19
Pitfalls I fell into when solving this puzzle in C++17

* Multidimensional std::array member in a Matrix class could not be brace initialised!
    * It has something to do with what this article "Nested Initialiser Lists for Multidimensional Arrays" solves (https://christophercrouzet.com/blog/dev/nested-initializers)?
    * I have to come back to this Issue. But the bottom line is that I lost valuable time trying to get my code to compile...
    * The following code does NOT compile (however many curly braces I try to add in trial and error...)

```
class Matrix {
  public:
  std::array<std::array<int,3>,3> m;
};
Matrix Rx90 = {{
   {1,0,0}
  ,{0,0,-1}
  ,{0,1,0}}};
```

    * Compiler error "error: too many initializers for ‘std::array<std::array<int, 3>, 3>’"
    * The following code DOES compile :)
    * But WHY do I need so many curly braces?


```
using Matrix = std::array<std::array<int,3>,3>;
Matrix Rx90 = {{
   {1,0,0}
  ,{0,0,-1}
  ,{0,1,0}}};
```
* Getting the signs correct on offsets between scanners.
  * In the end we are looking for the offset so that scanner + offset = scanner 0
  * Which gives us offset = "scanner 0" - "scanner".
  * For some reason I am "dyslectic" in getting correct "mirroring" and had to perform testing until I had the sign correct through the whole chain.

Lessons learned

* I now know aligned is spelled with only one 'l'...
* I now have the 3D rotation and translation algebra refreshed! It took some time to get the rotation matrices correct and also the generation of the 24 possible "dice orientations".
* Prototyping is an art in itself! I got into some extra trouble when refactoring from prototype code into live code.
* I identified a way to perform pairwise processing of symmetrical pairs (e.g., pairs of beacons where we want to process each pair only once).
    * I ended up in code doing this on deques which was suboptimal but I did not take the time to refactor into a more suitable data structure like a vector.

```

  std::deque<Scanner> unaligned{scanners.begin()+1,scanners.end()};
  std::deque<std::pair<Scanner,Alignment>> aligned{{scanners[0],{}}};
  while (unaligned.size()>0) {
    std::cout << "\nunaligned count " << unaligned.size();
    std::cout << "\naligned count " << aligned.size();
    auto unaligned_count_before = unaligned.size();
    auto const [scanner_0,alignment_0] = aligned.back();
    aligned.pop_back();
    for (int i=0;i<unaligned.size();i++) {
      auto const scanner_x = unaligned.front();
      unaligned.pop_front();
      auto alignment = find_alignment(scanner_x,scanner_0);
      if (alignment) {
        // align x to 0
        Scanner aligned_x{scanner_x.id,{}};
        std::transform(scanner_x.beacons.begin(),scanner_x.beacons.end(),std::back_inserter(aligned_x.beacons),[&alignment](Vector const& bx){
          auto& [rotation,translation] = alignment.value();
          auto aligned_v = rotation*bx + translation;
          return aligned_v; 
        });
        aligned.push_back({aligned_x,alignment.value()});
      }
      else {
        unaligned.push_back(scanner_x); // may be aligned with other scanner later
      }
    }
    aligned.push_front({scanner_0,alignment_0});
  }
  if (unaligned.size()!=0) std::cout << "\nERROR - Failed to align all scanners.";


```

* The main idea here is to keep all scanners used as left-hand-side in a visited deque. These are also the aligned ones.
    * Then we can align each in the unaligned deque to any of the ones in the aligned deque.
    * We then tray to align the back of the aligned deque with the front of the unaligned while rotating the unaligned.
    * The rotation of the unaligned is accomplished by popping from the front and pushing to the back while aligning.
    * NOTE: The for (int i=0;i<unaligned.size();i++) is just for safety reasons to prevent eternal looping should the search fail.
* I left the code when it worked and was reasonably cleaned up.
* I have not tried to refactor it to have the different phases fit more readily together (for example using a deque as the result to process for beacon count and best manhattan distance is for suer suboptimal...)

# day18
Lessons learned in day 18.

* I should learn to not be to clever initially and write "stupid" but resilient code.
    * I initially assumed (correctly) that no pair could go beyond level 5 (as they will all explode above level 4).
    * So I wrote the code to identify level==5 instead of level>4.
    * But due to an error I got pairs above level 5.
    * I incorrectly then thought my assumption was wrong.
    * So I changed my code to work even for higher levels.
    * Lesson learned: If I had written the original code less clever (not assuming 5 is the highest level) I would have saved time not chasing the wrong bug.
* I initially misunderstood that a pair that splits to level 5 must immediately trigger an explosion of this pair.
    * I found this error when performing tests against the examples in the puzzle description.
    * Lesson learned: Be sure to test everything that is possible to test from the information provided to find all corner cases.
* I learned to go for a data representation that sutis the problem at hand. Maybe I could have solved it using an expression tree for the snailfish numbers.

  A snailfish number is a pair p
  add p1,p2 = [p1,p2]
  reduce p = explode p* | split n*
  p* is first level 4 pair in p
  n* is first number >= 10
  explode p =
            left_of_p.right += p.left
            right_of_p.left += p.right
            p is replaced by number 0
  split n = [round_down(n/2),round_up(n/2)]

  We need a data structure to represent p so that we can
  add p1,p2 (p can be a std::pair)
  reduce p  (we need to be able to find level 4 p*, find pair left_of_p and right_of_p)
  magnitude p (p can be a std::pair)

  Let's try a representation of a number paired with its nest level?
  [1,2] := {{1,1}{1,2}} i.e., level 1 value 1, level 1 value 2
  [[3,4],5] := {{2,3}{2,4}{1,5}}

  The sum [1,2] + [[3,4],5] = [[1,2],[[3,4],5]]
  becomes {{1,1},{1,2}} appended with {{2,3}{2,4}{1,5}} with all levels incremented
  = {{2,1}{2,2}{3,3}{3,4}{2,5}}

  Explode is now a flat array manipulation
  [[[[[9,8],1],2],3],4] := {{5,9}{5,8}{4,1}{3,2}{2,3}{1,4}}
  The pair nested in level is easily found as being on level 5
  So the {5,9},{5,8} "explodes" to the '9' adds to the left and the '8' adds to the right
  {{5,9}{5,8}{4,1}{3,2}{2,3}{1,4}} explodes to {{4,0}{4,9}{3,2}{2,3}{1,4}} := [[[[0,9],2],3],4]

  Split is now also a flat array manipulation
  [[[[0,7],4],[15,[0,13]]],[1,1]] := {{4,0}{4,7}{3,4}{3,15}{4,0}{4,13}{2,1}{2,1}}
  {{4,0}{4,7}{3,4}{3,15}{4,0}{4,13}{2,1}{2,1}} splits to {{4,0}{4,7}{3,4}{3,round_down(15/2)}{3,round_up(15/2)}{4,0}{4,13}{2,1}{2,1}}

  Finally the magnitude is an iterative reduction on the flat array (top level and down)
  [[9,1],[1,9]] := {{2,9}{2,1}{2,1}{2,9}}
  magnitude {{2,9}{2,1}{2,1}{2,9}} = magnitude {magnitude {2,9}{2,1} magnitude {2,1}{2,9}}
  Note that magnitude reduces a pair into a number and thus a number with decreased level
  = magnitude {{1,3*9+2*1}{1,3*1+2*9}} = magnitude {{1,29}{1,21}} = {0,3*29+2*21} = 129

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
  * Here is the "correct" and safe range definition :)
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

* Then for part 2 I thought I was to test all the listed shots to see how many of them hit the target.
    * Fortunately it did not take me so much time to parse the list of example shots and apply them to count the hits.
    * It took me a little longer to realise that this was NOT the task at all.
* I am a meta-thinker. Meaning, I do what I experience is the intention, not what is actually said...

# day 16
Lessons learned for day 16
* It is quite easy to cut the path up from recursive calls and lose the result
    * For part 2 I refactored recursive parsing to recursive evaluation.
    * I did this by replacing the part 1 parse result being the bits read counter, to a struct also carrying the evaluated values.
    * This was a good idea but it took me some time to get the re-assembly of values up the call stack again.
    * I suppose I can find a better way to express the re-assembly operations so I don't have to repeat the same code in so many places?

* For part 2 it worked well to refactor part 1 recursive parse functions and turn them into recursive evaluation functions.

From:

```
int parse_packet(Bin& bin);
int parse_packets_size(Bin& bin,int bit_count);
int parse_packets_count(Bin& bin,int package_count);
```
To:
```
struct EvalResult {
  std::vector<size_t> val;
  size_t version_acc{0};
  int bits_read{0};
};

EvalResult eval_packet(Bin& bin);
EvalResult eval_packets_size(Bin& bin,int bit_count);
EvalResult eval_packets_count(Bin& bin,int package_count);
```
* I think it is worth noting that we had only to change the return type up back the chain of recursion. The arguments pushed down the call stack remained the same for part 2.
* In hind sight, the arguments going down the recursion is the input stream and the constraint on how much to read (parse or evaluate)
    * And this constraint can be either a measure of bits to read or a measure of the package count to read. 
* The EvalResult for part 2 accumulates (assembles) the data we are searching for plus the data required to implement the read constraint (bit count for the bit count constraint).  

# day 15
I really struggled with this puzzle!
* For one I implemented the shortest-path algorithm over and over and continuously got different answers.
    * In hindsight I think the "not move diagonally" was one of the hurdles?
    * I mean, I did make sure NOT to expand the search diagonally.
    * Still, I think I managed to still get an algorithm that chose a diagonal step although I did not e x p a n d diagonally.
    * But I am still not sure what small errors I kept doing that prevented me from getting the right path answer?
* I struggled debugging this problem.
    * The layers of indirection made it hard to produce debug outputs that pointed me to problems.
    * The algorithm will search several paths at once and Dijkstra will switch path when one with a lower risk is found.
    * Finding log outputs to follow this behavior was not easy to my experience.
* I can still not grasp why my algorithm does NOT work if I expand only right and down?
    * I have to expand left/right and up/down to get the right answer for part 2.
    * But my intuition is that left and up should always be an already visited node?

* Here is the expand part of my search where I generate -1,0,1 row step as well as column step.
```
    for (auto delta_row : {-1,0,1}) {
      for (auto delta_col : {-1,0,1}) {
        if (std::abs(delta_row) == std::abs(delta_col)) continue; // skip self and diagonal
        Position adj{.row=pos.row+delta_row,.col=pos.col+delta_col};
        if (adj.row<0 or adj.row>max_row or adj.col<0 or adj.col>max_col) continue; // skip out of bounds 
        if (state_map.at(adj).visited) continue; // skip visited
        auto& adj_state = state_map.at(adj);
        auto path_to_adj_risk = pos_state.risk_acc+adj_state.enter_risk;
        if (path_to_adj_risk<adj_state.risk_acc) {
          adj_state.risk_acc = path_to_adj_risk;
          adj_state.from = pos;
          unvisited.push(adj); // expand graph with this candidate
          // std::cout << " +{" << adj.row << "," << adj.col << "}";
        }
      }
```
* If I try and other combination I get different answers for part 2!

```
    // This fails to find the shortest path...
    for (auto delta_row : {0,1}) {
      for (auto delta_col : {-1,0,1}) {
```
```
    // This also fails to find the shortest path...
    for (auto delta_row : {-1,0,1}) {
      for (auto delta_col : {0,1}) {
```
```
    // ...so does this...
    for (auto delta_row : {0,1}) {
      for (auto delta_col : {0,1}) {
        if (std::abs(delta_row) == std::abs(delta_col)) continue; // skip self and diagonal

```
* Maybe the answer to this puzzle will come to me after a nights sleep?
* **WAIT!!** Aha - I need to reach a new node from **EVERY DIRECTION**!
    * Why did it take me so long to realize this?
    * I AM allowed to go down, left and then up again (even up and then right...) and so on!
    * And Dijkstra-like searching along the currently least risky path may take the algorithm so far into the map that there IS unvisited nodes up and left :)
* Thanks Advent of Code - I learned something today :)

Final note: To my delight I was not alone in falling into the trap of "down and right is enough"! I found out Jonathan Paulson made the same asumption and also had ti struggle before getting it (https://youtu.be/hig93Etxims?t=1085 ).

# day 12
* At first I came this far. But this does not work. Why?

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
* I finally got some code that worked but this is s u p e r   s l o w!
* I imagine I managed to create a brute force search (and I want to come back to this algorithm and implement it as it should be)

```
    while (candidate_paths.size()>0) {
      if (loop_count++%LOOP_LOG_THRESHOLD==0) {
        std::cout << "\n" << loop_count-LOOP_LOG_THRESHOLD;
        std::cout << "\t" << candidate_paths.size();
        std::cout << "\t\t" << valid_paths.size();
      }
      auto candidate_path = candidate_paths.back();
      candidate_paths.pop_back();
      if (is_full_path(candidate_path)) {
        valid_paths.push_back(candidate_path);
      }
      else if (is_valid_path(candidate_path)) {
        for (auto const& new_candidate_vertex: adjacent_graph[candidate_path.back()]) {
          auto new_candidate_path = candidate_path;
          new_candidate_path.push_back(new_candidate_vertex);
          candidate_paths.push_back(new_candidate_path);
          // rotate candidates right to move new candidate back in line 
          std::rotate(candidate_paths.begin(),candidate_paths.end()-1,candidate_paths.end());
        };
      }
    }
```


# day11
The lesson after this days puzzle is to remember to KISS (Keep it simple stupid) ;)
* I tried to be clever form the start and squeeze the processing through standard C++ library algorithms. I failed. I believe it may still be possible but for this small puzzle I am not sure it is worth the overhead?
* I failed to see through the weeds of details, that I can mutate the same grid of crabs to get the answer (I do not need a temp grid to mutate and them switch back for each flash mutations of the grid).
* I want to learn to solve these puzzles in small iterations with a lot of checks and logging of each progress. So I know I am on the right track. I can use the approach applied by the fastest Python programmers when they solve these puzzles :).
* I Do have a tendency to go for the cleanest solution right away. But so far this mostly means a get stuck in compiler errors in my "smart" wrapper classes 8-)
* I am still not satisfied with the final code as it re-uses an automata class for part 1&2 that relies on mutable references to local variables. I suppose we will see more cellular automata in the 2021 puzzles to come so I try to design my automata in a more elegant way then ;)
# day9
Pitfalls for me in C++ for this puzzle.
* C++ standard library operator<< to std::cout treats size8_t as a char! So when I made a map of size8_t values and tried to print it it got gibberish and lost some hair and time trying to figure out what went wrong.
* Using std::transform to expand the frontier of a basin I had to remember to use std::back_inserter to get the new candidates into the candidate list. I also had to flatten the candidate list as the transform created a vector of vectors. NOTE that using the begin() iterator to the result will compile but fail to expand the output.

```
        std::vector<Points> points_vector{};
        std::transform(frontier.begin(), frontier.end(), std::back_inserter(points_vector), [](auto const p) {
            Points candidates{ {p + LEFT},{p + UP},{p + RIGHT},{p+DOWN} };
            return candidates;
            });
```

* At first I imagined for part 2 that I needed to expand also to diagonal points on the map. But that will leak as the input data does NOT define basin boundaries in this way.
* To make my own Point struct work in std::set I needed to define an operator< for it. And I defined it by calculating an integer from the x and y coordinates by offsetting the x by 10000. This is a quick-and-dirty way and I would not recommend it (unless you are sure y will never be greater that 10000) ;).

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
* Again I found myself being dragged into rabbit holes of ever and ever more details and entangled data and logic.
* In fact - I have a hunch day 4 is day 3 in disguise? I mean, both these puzzles has a grid of data as input. We need to accumulate over rows or columns to extract information.
* If I have time I want to come back to this puzzle and see if I can solve day 3 and day 4 with some common framework? 
# day3
Note: 211210 - Solved the puzzle from scratch and got a somewhat cleaner code ;)

Originally - The day 3 puzzle was one of those I easily stumble on due to all small +/- one and true/false problems.
* I failed to design the code so that I did not shoot myself in the foot over and over again (detail overflow). Just look at this example...
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
* For me part of the challenge of this puzzle was that I failed to name "things" properly so that my code could "talk" about the problem in a clear way.
* After I finished I was left with the feeling that this problem was in fact just a transform + accumulate problem? If I have time I want to come back to is and solve it properly.

# day2
For this puzzle I struggled mostly with parsing the input.
* I stuck with the std::sregex_token_iterator IContainer from day 1 as I thought maybe I could use this approach to parse all puzzle input to come. But the result was that parsing become overly complicated.

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
