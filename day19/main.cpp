#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <array>
#include <numeric>
#include <map>
#include <random>
#include <cmath>
#include <optional>
#include <deque>
#include <set>

std::pair<std::string,std::string> split(std::string const& line,std::string delim) {
  if (auto pos = line.find(delim); pos != std::string::npos) {
    auto left = line.substr(0,pos);
    auto right = line.substr(pos + delim.size()); 
    return {left,right};
  }
  else {
    return {line,""};
  }
}

std::tuple<int,int,int> split_ints(std::string const& line) {
  auto [xs,right] = split(line,",");
  auto [ys,zs] = split(right,",");
  std::cout << "\n{" << xs << "," << ys << "," << zs << "}";
  return {std::stoi(xs),std::stoi(ys),std::stoi((zs==""?"0":zs))};
}

bool contains(std::string const& token,std::string const& key) {
  return (token.find(key) != std::string::npos);
}

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;


/*
For a cartesian coordinate system the rotation matrices are

Rx90 =  1  0  0   Rotate 90 degrees counter clockwise around x-axis
        0  0 -1
        0  1  0

Ry90 =  0  0  1
        0  1  0
       -1  0  0

Rz90 =  0 -1  0
        1  0  0
        0  0  1
*/


namespace prototype {
  /*
  // 1-dimension thinking
  // If scanner 1 and 2 lives on the same map at different locations they may see the same beacons B as,

        <scanner 1>               <scanner 2 view 0>    <scanner 2 view 1>
    S1 . . B1 . . B2 . B3   . . . B1 . . B2 . B3 S2     S2 B3 . B2 . . B1 . . .
    0      3      6    8 +       -6     -3   -1  0      0  1    3      6

  Meaning scanner 1 is to the left of the beacons and scanner 2 is to the right.
  S1 sees B1 at 3, B2 at 6 and B3 at 8
  S2 sees B1 at -6, B2 at -3 and B3 at -1
  
  Assuming we know there is full overlap (scanner 1 and 2 sees the all and same beacons) we can identify the same beacons in two ways (I think?)

  1) Identify pairwise beacons [B1..B2[ = [3..6[ == [-6..-3[, [B1..B3[ = [3..8[ == [-6..-1[, [B2..B3[ = [6..8[ == [-3..-1[
     I.e., follows the same "edge" direction and length indicates the same beacon pair.

     We can count the ranges of the same size by mapping the range to a count and do if (r1==r2) map[r1]++

  2) Identify the same pairwise transaction between S1 and S2 view of the same beacons
     S1[B1] - S2[B1] == S1[B2] - S2[B2] == S1[B3] - S2[B3] == S1-S2

    S1 . . B1 . . B2 . B3   - . . . B1 . . B2 . B3 S2 = S1 . . D . . D . D S2 where D = translation between S1 and S2
                                                        0      3     6   8 9 

    We can find this translation by translating S2 until the distance between *all* seen beacons by S1 and S2 becomes 0.

  *) If scanner 1 and 2 can have left and right "mixed upp" we need to alco check for matches in a mirrored view

    <scanner 2 mirrored>
    . . . B1 . . B2 . B3 S2 ==> S2 B3 . B2 . . B1 . . . 

  // 2-dimensional thinking

  We add a second dimension to the 1-dimensional thinking

  <scanner 1>            <scanner 2>
    .  .  .  B2 .     .  .  .  B2 .  .
    B1 .  .  .  .     B1 .  .  .  .  S2
    .  .  .  .  B3    .  .  .  .  B3 .
    S1 .  .  .  .     .  .  .  .  .  .

  1) Identify "edges" bnetween seen beacons
  <scanner 1>               <scanner 2 view 0>
  .  .  .  .  .   .  .    .  .  .  .  .   .  .
  .  .  .  .  e1  .  .    .  .  .  .  e1  .  .
  .  +  .  .  .  .  .     .  +  .  .  .  .   eS
  .  .  .  .  .  e2  .    .  .  .  .  .  e2  .
  .  eS e3 .  .  .   .    .  .  e3 .  .  .   .

  Where + is origo, DS is the edge between S->B1 and Dn are the pairwise edges B1->B2, B1->B3, B2->B3 (some enumeration of beacons seen by S1 and S2)

  NOTE: We need to enumerate ALL edges in each map (each perumation of Ba->Bb and Bx->S on each map)

  2) Identify "edges" between beacons seens by scanner 1 and scanner 2

  <scanner 1>         <scanner 2 view 0>      <scanner1> - <scanner 2 view 0>          
    .  .  .  B2 .     .  .  .  B2 .  .        .  .  .  .  .  .  .  e
    B1 .  .  .  .     B1 .  .  .  .  S2       .  .  .  .  .  .  .  .
    .  .  .  .  B3    .  .  .  .  B3 .        .  .  +  .  .  .  .  .
    S1 .  .  .  .     .  .  .  .  .  .        .  .  .  .  .  .  .  .

  Where + is origo and e is the common S1-S2 == S1[B1]-S2[B1] == ...

  *) AND we need to try all "flipping" of scanner 2 map left/right and up/down to ensure we find the orientation of scanner 2 vs scanner 1 so they "see" the same reality

  OBSERVATION ==> Scheme 2 requires only one edges-calculation while scheme 1 requires we make an edges calculation for each scanner separatly before comparing!
                  In scheme 2 we basically searches for the orientation where all beasons seens by scanner 2 is just translated.
                  And we do this by computing the beacopn translations for each orientation of scanner 2 vs scanner 1.

  PROPOSED ALGORITHM

    *) Compute the translation between beacons on scanner 1 map and all possible orienations (4) of beacons as seen by scanner 2.
    *) find the orientation where *all* beacons are translated the same amount (same translation vector e)

  // 3-dimensional thinking

  We add a third dimension to the 2-dimensional thinking

  It seems in addition to the 2-dimensional flipping left/right and up/down we can now also "look" in the direction of any of the 6 sides of a cube?

  If we imagine a die we can look from inside the die out through the face 1,2,3,4,5,6.
  And for each of these "looking" orientation we can rotate around the looking axis in four ways (0,1/4,1/2,3/4 turns).
  Gives us a total of 6*4 = 24 possible unique orientations.

PROPOSED ALGORITHM

    *) Compute the translation between beacons on scanner 1 map and all possible orienations (24) of beacons as seen by scanner 2.
    *) find the orientation where *all* beacons are translated the same amount (same translation vector e)

  */

  void test() {

    /*
        --- scanner 0 ---
    0,2
    4,1
    3,3

    --- scanner 1 ---
    -1,-1
    -5,0
    -2,1
    */

    const std::vector<std::pair<int,int>> scanner_0 = {{0,2},{4,1},{3,3}};
    const std::vector<std::pair<int,int>> scanner_1 = {{-1,-1},{-5,0},{-2,1}};

    // 1) Explore edge matching
    std::vector<std::pair<int,int>> edges_0{};
    for (auto const& b0 : scanner_0) {
      for (auto const& b1 : scanner_0) {
        edges_0.push_back({b0.first-b1.first,b0.second-b1.second});
      }
    }
    std::vector<std::pair<int,int>> edges_1{};
    for (auto const& b0 : scanner_1) {
      for (auto const& b1 : scanner_1) {
        edges_1.push_back({b0.first-b1.first,b0.second-b1.second});
      }
    }
    std::map<std::pair<int,int>,int> edge_map{};
    std::cout << "\nedges_0:";
    for (auto const& edge : edges_0) {
      edge_map[edge]++;
      std::cout << "\n{" << edge.first << "," << edge.second << "}";
    }
    std::cout << "\nedges_1:";
    for (auto const& edge : edges_1) {
      edge_map[edge]++;
      std::cout << "\n{" << edge.first << "," << edge.second << "}";
    }
    for (auto const& [edge,count] : edge_map) {
      std::cout << "\nedge {" << edge.first << "," << edge.second << "} count: " << count;
    }
    // Output:
    // edge {-4,1} count: 2
    // edge {-3,-1} count: 2
    // edge {-1,2} count: 2
    // edge {0,0} count: 6
    // edge {1,-2} count: 2
    // edge {3,1} count: 2
    // edge {4,-1} count: 2    
    // ==> Conclusion, we need to filter out self-self {0,0} and we do extra work by generating each edge twice.
    //     We also lose he translation information!
    //     In theory we can detect a macth when *all* edges match but approach 2 below is less work and gives us the translation "for free"

    // Explore translation matching
    std::vector<std::pair<int,int>> translations{};
    for (auto const& b0 : scanner_0) {
      for (auto const& b1 : scanner_1) {
        translations.push_back({b0.first-b1.first,b0.second-b1.second});
      }
    }
    std::map<std::pair<int,int>,int> translation_map{};
    std::cout << "\ntranslations:";
    for (auto const& edge : translations) {
      translation_map[edge]++;
      std::cout << "\n{" << edge.first << "," << edge.second << "}";
    }
    for (auto const& [edge,count] : translation_map) {
      std::cout << "\nedge {" << edge.first << "," << edge.second << "} count: " << count;
    }
    auto iter = std::max_element(translation_map.begin(),translation_map.end(),[](auto const& e1,auto const& e2){
      return e1.second < e2.second;
    });
    std::cout << "\nmatched translation {" << iter->first.first << "," << iter->first.second << "} count:" << iter->second;
    // Example:
    // edge {1,3} count: 1
    // edge {2,1} count: 1
    // edge {4,4} count: 1
    // edge {5,2} count: 3
    // edge {6,0} count: 1
    // edge {8,3} count: 1
    // edge {9,1} count: 1
    // matched translation {5,2} count:3    
    // CONCLUSION: This is the way to go! We can identify the translation shared by *all* beacons seen by scanner 1 and 2 and this is
    //             also the translation between the scanners :)

    // What is now missing is a way to find the correct orientation of scanner 2
  } // test()

  using Vector2D = std::array<int,2>;
  using Matrix2D = std::array<std::array<int,2>,2>;
  Vector2D operator*(Matrix2D const& m, Vector2D const& v){
    Vector2D result{};
    for (int r=0;r<m.size();r++) {
      for (int c=0;c<m[0].size();c++) {
        result[r] += m[r][c]*v[c];
      }
    }
    return result;
  }
  Matrix2D operator*(Matrix2D const& m1, Matrix2D const& m2){
    Matrix2D result{};
    for (int r=0;r<m1.size();r++) {
      for (int c=0;c<m2[0].size();c++) {
        for (int k=0;k<m1[0].size();k++) {
          result[r][c] += m1[r][k]*m2[k][c];
        }
      }
    }
    return result;
  }

  const std::vector<Vector2D> scanner_0 = {{0,2},{4,1},{3,3}};
  const std::vector<Vector2D> scanner_1 = {{-1,-1},{-5,0},{-2,1}};

  void test2() {
    // 2D rotation 90 degree anticlockwise (x,y) -> (-y,x) can be defined as multiplying a matrix m90 to column vector v
     Matrix2D m0{{
      {1,0}
      ,{0,1}
    }};
     Matrix2D m90{{
      {0,-1}
      ,{1,0}
    }};

    Vector2D v{-5,0};
    std::cout << "\n{" << v[0] << "," << v[1] << "}";

    std::cout << "\n<rotate>";
    auto v1 = v;
    for (int i=0;i<4;i++) {
      v1 = m90*v1;
      std::cout << "\n{" << v1[0] << "," << v1[1] << "}";
    }

    std::vector<Matrix2D> rotations{m0};
    for (auto i : {1,2,3}) {
      rotations.push_back(m90*rotations.back());
    }

    if (m90*rotations.back() != rotations.front()) std::cout << "\nERROR: Four rotations should return to unit";
    else std::cout << "\nSUCCESS - Four rotations gets us back to unit";

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0,3);

    for (int i=0;i<20;i++) std::cout << "\nrandom: " << distrib(gen);

    auto rand_rot = rotations[distrib(gen)];
    std::vector<Vector2D> scanner_x{};
    for (auto const& v : scanner_1) {
      scanner_x.push_back(rand_rot*v);
    }

    std::cout << "\nscanner_x";
    for (auto const& b : scanner_x) std::cout << "\n{" << b.at(0) << "," << b.at(1) << "}";

    // Now try to find out what the rotation and translation is?
    // Apply each possible orientation of other scanner to see if any enables us to match with the first scanner?
    std::optional<std::pair<Matrix2D,Vector2D>> found_deviation{};
    for (auto const& orientation : rotations) {
      // count the translaton vectors between beacons of scanner 1 and x until one translatition matches *all* seen beacons
      std::cout << "\torientation try:";
      std::map<Vector2D,int> translations_count{};
      for (auto const& b0 : scanner_0) {
        for (auto const& bx : scanner_x) {
          std::cout << "\tre-oriented bx try:";
          Vector2D rotated_bx = orientation*bx;
          // b0 + translation = bx 
          Vector2D translation{b0[0]-rotated_bx[0],b0[1]-rotated_bx[1]};
          std::cout << "\ncount " << translations_count[translation]+1;
          if (++translations_count[translation]>=3) {
            found_deviation={orientation,translation};
            goto done;
          }
        }
      }
    }
    done:
    if (found_deviation) {
      std::cout << "\nfound translation (b0+translation=bx): {" << found_deviation->second.at(0) << "," << found_deviation->second.at(1) << "}";
      std::cout << "\nfound rotation (rotation*bx same rot as b0): ";
      for (auto const& row : found_deviation->first) {
        std::cout << "\n(" << row.at(0) << " " << row.at(1) << ")";
      }
    }
  }

  using Vector3D = std::array<int,3>;
  using Matrix3D = std::array<Vector3D,3>;
  Vector3D operator*(Matrix3D const& m, Vector3D const& v){
    Vector3D result{};
    for (int r=0;r<m.size();r++) {
      for (int c=0;c<m[0].size();c++) {
        result[r] += m[r][c]*v[c];
      }
    }
    return result;
  }
  Matrix3D operator*(Matrix3D const& m1, Matrix3D const& m2){
    Matrix3D result{};
    for (int r=0;r<m1.size();r++) {
      for (int c=0;c<m2[0].size();c++) {
        for (int k=0;k<m1[0].size();k++) {
          result[r][c] += m1[r][k]*m2[k][c];
        }
      }
    }
    return result;
  }
  using Scanner = std::vector<Vector3D>;
  using Scanners = std::vector<Scanner>;

  class Orientations3D {
  public:
    Orientations3D() {
      // Generate all 24 rotations
      int face_index{0};    
      for (auto i : {1,2,3,4,5,6}) {
        switch (i) {
          case 1: rotations.push_back(RUNIT); break; // outward x
          case 2: rotations.push_back(RZ90*rotations[face_index]); break; // outward y
          case 3: rotations.push_back(RZ90*rotations[face_index]);break; // outward -x
          case 4: rotations.push_back(RZ90*rotations[face_index]); break; // outward -y
          case 5: rotations.push_back(RY90);break; // outward -z
          case 6: rotations.push_back(RY90*RY90*rotations[face_index]); break; // outward z
        }
        face_index=rotations.size()-1;
        for (auto j : {2,3,4}) {
          switch (i) {
            case 1: rotations.push_back(RX90*rotations.back()); break; // rotate x
            case 2: rotations.push_back(RY90*rotations.back()); break; // rotate y
            case 3: rotations.push_back(RX90*rotations.back()); break; // rotate x (should be -x but we get all rotations clockwise too)
            case 4: rotations.push_back(RY90*rotations.back()); break; // rotate y (should be -y but we get all rotations clockwise too)
            case 5: rotations.push_back(RZ90*rotations.back()); break; // rotate z (should be -z but we get all rotations clockwise too)
            case 6: rotations.push_back(RZ90*rotations.back()); break; // rotate z 
          }
        }
      }
    }

    // See 3d_rotations_matrices.png (https://github.com/kjelloh/advent_of_code_2021/tree/main/day19 )
    // We can use these to gerenate all 24 possible rotations
    // 6 for each "facing" out through a face of a die
    // 4 rotations around the face normal
    // 24 in total 
    static constexpr Matrix3D const RUNIT = {{ // No rotation
      {1,0,0}
      ,{0,1,0}
      ,{0,0,1}}};
    static constexpr Matrix3D const RX90 = {{
      {1,0,0}
      ,{0,0,-1}
      ,{0,1,0}}};
    static constexpr Matrix3D const RY90 = {{
      {0,0,1}
      ,{0,1,0}
      ,{-1,0,0}}};
    static constexpr Matrix3D const RZ90 = {{
      {0,-1,0}
      ,{1,0,0}
      ,{0,0,1}}};

    auto begin() const {return rotations.begin();}
    auto end() const {return rotations.end();}
    auto size() {return rotations.size();}
    auto operator[](auto i) {return rotations[i];}

  private:
    std::vector<Matrix3D> rotations{};
  };

  void test3() {
    // lets go 3D and build some support code
    Orientations3D rotations{};
    // Test all 24 rotations
    std::cout << "\nrotations count " << rotations.size();
    Vector3D v{5,0,1};
    std::vector<Vector3D> rotated{
       {5,0,1}
      ,{5,-1,0}
      ,{5,0,-1}
      ,{5,1,0}
      ,{0,5,1} // outward y
      ,{1,5,0}
      ,{0,5,-1}
      ,{-1,5,0}
      ,{-5,0,1} // outward -x
      ,{-5,-1,0}
      ,{-5,0,-1}
      ,{-5,1,0}
      ,{0,-5,1} // outward -y
      ,{1,-5,0}
      ,{0,-5,-1}
      ,{-1,-5,0}
      ,{1,0,-5} // outward -z
      ,{0,1,-5}
      ,{-1,0,-5}
      ,{0,-1,-5}
      ,{-1,0,5} // outward z
      ,{0,-1,5}
      ,{1,0,5}
      ,{0,1,5}
    };
    for (int i=1;i<= rotated.size();i++) {
      auto r = rotations[i-1]*v;
      auto e = rotated[i-1]; 
      std::cout << "\nrotated: {" << r[0] << "," << r[1] << "," << r[2] << "}";
      std::cout << "\nexpected: {" << e[0] << "," << e[1] << "," << e[2] << "}";
      if (r==e) std::cout << " ok";
      else std::cout << " FAILED";
    }
  } // test3()

  void test4() {
    std::cout << "\ntest4";
    // Part 1 example of one scanner in different orientations 
    const Scanners scanners = {
        { {-1,-1,1}
        ,{-2,-2,2}
        ,{-3,-3,3}
        ,{-2,-3,1}
        ,{5,6,-4}
        ,{8,0,7}}
      ,{ {1,-1,1}
        ,{2,-2,2}
        ,{3,-3,3}
        ,{2,-1,3}
        ,{-5,4,-6}
        ,{-8,-7,0}}
      ,{ {-1,-1,-1}
        ,{-2,-2,-2}
        ,{-3,-3,-3}
        ,{-1,-3,-2}
        ,{4,6,5}
        ,{-7,0,8}}
      ,{ {1,1,-1}
        ,{2,2,-2}
        ,{3,3,-3}
        ,{1,3,-2}
        ,{-4,-6,5}
        ,{7,0,8}}
      ,{ {1,1,1}
        ,{2,2,2}
        ,{3,3,3}
        ,{3,1,2}
        ,{-6,-4,-5}
        ,{0,7,-8}}
    };
    std::cout << "\nscanners size " << scanners.size();

    struct Deviation {
      Matrix3D rotation;
      Vector3D translation;
    };

    Orientations3D orientations{};
    std::cout << "\norientations size " << orientations.size();
    const int THRESHOLD = 6;
    auto scanner_0 = scanners[0];
    std::optional<Deviation> deviation{};
    for (int i=1;i<scanners.size();i++) {
      auto scanner_x = scanners[i];
      std::cout << "\nscanner x";
      // Now try to find out what the rotation and translation is?
      // Apply each possible orientation of other scanner to see if any enables us to match with the first scanner?
      for (auto const& orientation : orientations) {
        // count the translaton vectors between beacons of scanner 1 and x until one translatition matches *all* seen beacons
        std::map<Vector3D,int> translations_count{};
        for (auto const& b0 : scanner_0) {
          for (auto const& bx : scanner_x) {
            auto rotated_bx = orientation*bx;
            // b0 + translation = bx 
            Vector3D translation{b0[0]-rotated_bx[0],b0[1]-rotated_bx[1],b0[2]-rotated_bx[2]};
            if (++translations_count[translation]>=THRESHOLD) {
              deviation={orientation,translation};
              goto done;
            }
          }
        }
      }
    }
    done:
    if (deviation) {
      std::cout << "\nfound translation (b0+translation=bx): {" << deviation->translation.at(0) << "," << deviation->translation.at(1) << "," << deviation->translation.at(2) << "}";
      std::cout << "\nfound rotation (rotation*bx same rot as b0): ";
      for (auto const& row : deviation->rotation) {
        std::cout << "\n(" << row.at(0) << " " << row.at(1) << " " << row.at(2) << ")";
      }
    }    
  }

} // namespace prototype

namespace part1 {
  using Vector = prototype::Vector3D;
  using Matrix = prototype::Matrix3D;
  using Orientations = prototype::Orientations3D;
  using prototype::operator*;

  Vector operator+(Vector const& v1,Vector const& v2) {
    return {v1.at(0)+v2.at(0),v1.at(1)+v2.at(1),v1.at(2)+v2.at(2)};
  }

  Vector operator-(Vector const& v1,Vector const& v2) {
    return {v1.at(0)-v2.at(0),v1.at(1)-v2.at(1),v1.at(2)-v2.at(2)};
  }

  struct Scanner {
    int id;
    std::vector<Vector> beacons;
  };
  using Model = std::vector<Scanner>;
  
  Model parse(auto& in) {
      Model result{};
      std::string line{};
      int state{0};
      Scanner scanner{};
      while (std::getline(in,line)) {
        if (line.size()>0) {
          if (contains(line,"scanner")) {
            auto [left,right] = split(line,"scanner ");
            auto [sId,tail] = split(right," ");
            try {
              auto id = std::stoi(sId);
              scanner = Scanner{id,{}};
            }
            catch (std::exception const& e) {
              std::cout << "\nERROR - parse scanner id failed. Excpetion=" << e.what();
            }
          }
          else {
            try {
              auto [x,y,z] = split_ints(line);
              scanner.beacons.push_back({x,y,z});
            }
            catch (std::exception const& e) {
              std::cout << "\nERROR - parse ints falied. Excpetion=:" << e.what();
            }
          }
        }
        else if (scanner.beacons.size()>0) {
          result.push_back(scanner);
          scanner = {};    
        }
      }
      if (scanner.beacons.size()>0) result.push_back(scanner);
      return result;
  }

  struct Allignment {
    Matrix rotation{Orientations::RUNIT};
    Vector translation{{0,0,0}};
  };

  std::optional<Allignment> find_allignment(Scanner const& scanner_x, Scanner const& scanner_0) {
    // finds allignment so that allignment(scanner_x) = scanner_0 
    // where allignment(s) = rotation of s + translation of s
    std::optional<Allignment> result{};
    static Orientations const orientations{};
    const int THRESHOLD = 12;
    std::cout << "\nfind_allignment scanner " << scanner_x.id << " --> scanner " << scanner_0.id;
    // try to find out what the rotation and translation of scanner_x is in relation to scanner_0?
    // Apply each possible orientation of other scanner to see if any enables us to match with the first scanner?
    for (auto const& orientation : orientations) {
      // count the translaton vectors between beacons of scanner 1 and x until one translatition matches *all* seen beacons
      std::map<Vector,int> translations_count{};
      for (auto const& b0 : scanner_0.beacons) {
        for (auto const& bx : scanner_x.beacons) {
          // rotation*bx = b0
          auto rotated_bx = orientation*bx;
          // bx + translation = b0
          // translation = b0-bx
          Vector translation{b0-rotated_bx};
          if (++translations_count[translation]>=THRESHOLD) {
            result={orientation,translation};
            goto done; // break out of all loops
          }
        }
      }
    }
    done:
    if (result) {
      std::cout << "\nfound scanner_x " << scanner_x.id << " allignment to scanner_0 " << scanner_0.id;
      std::cout << "\n\ttranslation (scanner_x+translation=scanner_0): {" << result->translation.at(0) << "," << result->translation.at(1) << "," << result->translation.at(2) << "}";
      std::cout << "\n\trotation (rotation*scanner_x alligned with scanner_0): ";
      for (auto const& row : result->rotation) {
        std::cout << "\n(" << row.at(0) << " " << row.at(1) << " " << row.at(2) << ")";
      }
    }    

    return result;
  }

  Result solve_for(char const* pData) {
    std::string caption{"\nsolve_for:"};
    Result result{};
    std::stringstream in{ pData };
    auto scanners = parse(in);
    std::cout << caption << " scanner count " << scanners.size();
    std::cout << "\nscanners size " << scanners.size();

    std::deque<Scanner> unalligned{scanners.begin()+1,scanners.end()};
    std::deque<Scanner> alligned{scanners[0]};
    while (unalligned.size()>0) {
      std::cout << "\nunalligned count " << unalligned.size();
      std::cout << "\nalligned count " << alligned.size();
      auto unalligned_count_before = unalligned.size();
      auto const scanner_0 = alligned.back();
      alligned.pop_back();
      for (int i=0;i<unalligned.size();i++) {
        auto const scanner_x = unalligned.front();
        unalligned.pop_front();
        auto allignment = find_allignment(scanner_x,scanner_0);
        if (allignment) {
          // allign x to 0
          Scanner alligned_x{scanner_x.id,{}};
          std::transform(scanner_x.beacons.begin(),scanner_x.beacons.end(),std::back_inserter(alligned_x.beacons),[&allignment](Vector const& bx){
            auto& [rotation,translation] = allignment.value();
            auto alligned_v = rotation*bx + translation;
            return alligned_v; 
          });
          alligned.push_back(alligned_x);
        }
        else {
          unalligned.push_back(scanner_x); // may be alligned with other scanner later
        }
      }
      alligned.push_front(scanner_0);
    }
    if (unalligned.size()!=0) std::cout << "\nERROR - Failed to allign all scanners.";
    else {
      // count scanners
      std::set<Vector> init{};
      auto unique_beacons = std::accumulate(alligned.begin(),alligned.end(),init,[](auto acc,Scanner const& scanner) {
        std::copy(scanner.beacons.begin(),scanner.beacons.end(),std::inserter(acc,acc.begin()));
        return acc;
      });
      result = unique_beacons.size();
    }
    return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      return result;
  }
}

int main(int argc, char *argv[])
{
  // prototype::test();
  // prototype::test2();
  // prototype::test4();
  // return 0;
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(--- scanner 0 ---
404,-588,-901
528,-643,409
-838,591,734
390,-675,-793
-537,-823,-458
-485,-357,347
-345,-311,381
-661,-816,-575
-876,649,763
-618,-824,-621
553,345,-567
474,580,667
-447,-329,318
-584,868,-557
544,-627,-890
564,392,-477
455,729,728
-892,524,684
-689,845,-530
423,-701,434
7,-33,-71
630,319,-379
443,580,662
-789,900,-551
459,-707,401

--- scanner 1 ---
686,422,578
605,423,415
515,917,-361
-336,658,858
95,138,22
-476,619,847
-340,-569,-846
567,-361,727
-460,603,-452
669,-402,600
729,430,532
-500,-761,534
-322,571,750
-466,-666,-811
-429,-592,574
-355,545,-477
703,-491,-529
-328,-685,520
413,935,-424
-391,539,-444
586,-435,557
-364,-763,-893
807,-499,-711
755,-354,-619
553,889,-390

--- scanner 2 ---
649,640,665
682,-795,504
-784,533,-524
-644,584,-595
-588,-843,648
-30,6,44
-674,560,763
500,723,-460
609,671,-379
-555,-800,653
-675,-892,-343
697,-426,-610
578,704,681
493,664,-388
-671,-858,530
-667,343,800
571,-461,-707
-138,-166,112
-889,563,-600
646,-828,498
640,759,510
-630,509,768
-681,-892,-333
673,-379,-804
-742,-814,-386
577,-820,562

--- scanner 3 ---
-589,542,597
605,-692,669
-500,565,-823
-660,373,557
-458,-679,-417
-488,449,543
-626,468,-788
338,-750,-386
528,-832,-391
562,-778,733
-938,-730,414
543,643,-506
-524,371,-870
407,773,750
-104,29,83
378,-903,-323
-778,-728,485
426,699,580
-438,-605,-362
-469,-447,-387
509,732,623
647,635,-688
-868,-804,481
614,-800,639
595,780,-596

--- scanner 4 ---
727,592,562
-293,-554,779
441,611,-461
-714,465,-776
-743,427,-804
-660,-479,-426
832,-632,460
927,-485,-438
408,393,-506
466,436,-512
110,16,151
-258,-428,682
-393,719,612
-211,-452,876
808,-476,-593
-575,615,604
-485,667,467
-680,325,-822
-627,-443,-432
872,-547,-609
833,512,582
807,604,487
839,-516,451
891,-625,532
-652,-548,-490
30,-46,-14)";
char const* pData = R"(--- scanner 0 ---
579,799,416
48,82,-27
474,-502,534
652,-594,511
-609,574,-469
352,-544,-666
-780,740,353
-33,-102,-83
384,-640,-624
470,-559,544
468,864,533
670,759,-677
-700,873,335
-747,-486,-412
349,-584,-782
-382,-890,249
-567,658,-522
-689,659,-426
531,820,-584
-738,-569,-457
-369,-772,414
-816,800,310
601,763,-473
355,790,421
-629,-538,-431
-387,-903,381

--- scanner 1 ---
961,715,-541
-390,653,-337
475,-505,-519
-398,393,554
-543,-651,892
917,-691,745
-333,512,638
-447,-609,924
949,600,-504
-487,-493,927
959,652,536
860,-802,675
425,-583,-486
31,89,-38
856,-598,606
-729,-629,-505
-381,635,-466
-728,-658,-697
940,447,580
423,-556,-430
921,566,529
-385,763,-451
82,-46,86
863,628,-559
-792,-638,-728
-441,535,654

--- scanner 2 ---
-619,-740,-444
284,732,-285
349,769,-397
-800,371,856
-154,-142,35
-588,-753,-593
433,-771,646
426,648,604
-469,-701,964
-436,-703,873
-831,445,878
-68,-35,148
433,-910,547
321,677,-474
658,-831,-735
-708,471,-612
532,743,654
-599,-758,926
543,-753,-648
445,-815,-781
-682,-685,-597
491,-773,545
-814,389,-621
-712,458,817
-726,309,-547
422,838,592

--- scanner 3 ---
-742,640,493
468,610,-347
-656,680,388
533,814,631
-501,744,-596
472,-555,-383
-55,12,40
603,-399,668
-422,891,-618
-505,-699,597
-520,646,478
-523,-607,-421
-487,-491,600
479,-501,677
475,661,-559
428,770,510
-526,-596,-325
443,-605,-422
374,-553,-286
548,-425,783
-570,841,-566
-401,-666,575
605,756,519
469,566,-488
-494,-573,-565

--- scanner 4 ---
-24,-9,-13
693,825,-612
-659,674,730
726,696,493
-602,-470,716
-548,804,-616
-273,-440,-429
755,920,-722
591,-810,463
104,132,31
-798,656,824
701,-689,-711
-566,-637,674
800,-828,-711
901,636,510
-548,-422,677
592,-714,585
-775,678,794
-336,-418,-360
906,716,416
-738,825,-594
-360,-472,-355
-647,851,-605
802,-691,-653
743,730,-718
676,-767,543

--- scanner 5 ---
295,-829,-370
-646,-497,-795
785,-566,682
331,-823,-362
396,624,-653
-345,390,-837
-854,-521,625
-776,-569,671
-719,302,694
775,-608,710
618,434,807
613,-547,645
-691,-468,-758
-857,329,748
366,768,-662
-46,-127,-7
-368,546,-822
-855,-613,723
477,-938,-370
727,347,755
803,431,840
-390,421,-837
-915,322,743
-713,-449,-683
322,664,-548

--- scanner 6 ---
-478,-795,-399
636,601,800
475,790,-480
399,-509,961
48,51,76
-372,687,518
429,-773,-421
-854,749,-409
-751,809,-318
379,-786,-305
-764,-417,427
-712,-535,446
552,-546,960
-809,832,-531
653,710,892
483,781,-545
-104,95,187
378,-509,931
-505,589,580
-681,-426,472
499,783,-725
-347,-777,-366
452,-886,-397
-360,-782,-234
720,662,828
-418,474,502

--- scanner 7 ---
-747,-527,-295
-383,523,528
722,-666,942
622,-640,-526
-722,-449,-374
822,-614,982
531,486,601
-473,-615,807
658,-592,-519
546,512,-555
801,-857,967
-487,406,-646
-20,-36,160
-491,411,-601
596,468,439
550,606,-623
-705,-445,-317
636,-516,-380
699,575,-610
-472,-687,821
522,380,519
-563,527,518
-336,449,-620
-74,-172,0
-502,-611,738
-473,453,641

--- scanner 8 ---
-325,277,621
-346,432,631
648,-738,463
-284,-886,-509
610,-583,-878
429,537,390
-366,-908,-467
661,-654,541
567,-557,-727
605,-763,488
-379,-707,832
583,609,339
823,713,-542
888,705,-461
-262,393,-504
458,674,373
-329,-650,646
572,-602,-723
-237,503,-467
-288,-829,-492
-5,6,-132
-386,307,509
-383,-599,733
99,-170,-55
-249,384,-440
830,694,-667

--- scanner 9 ---
597,-475,-339
821,675,845
-364,-312,543
414,576,-313
743,721,797
-328,-320,537
-749,800,-693
386,653,-405
-661,726,-773
-95,72,62
-643,791,-611
-647,-638,-624
822,773,753
-735,-677,-711
613,-745,869
658,-708,898
487,-413,-244
491,-425,-434
-646,695,819
349,464,-350
-510,-320,428
-818,732,800
680,-633,917
-766,-699,-637
74,149,-8
-808,760,758

--- scanner 10 ---
671,-555,-655
672,548,433
-684,363,-477
628,600,354
720,502,355
792,-806,496
397,393,-396
-438,403,762
-573,-435,-449
-475,291,827
397,400,-473
434,477,-433
-559,-598,685
-464,-648,579
-625,390,-456
657,-513,-539
773,-881,518
-653,-440,-543
744,-918,563
-672,386,-603
-147,-172,17
647,-507,-645
-369,-586,689
-530,480,790
-593,-615,-526
-35,-50,-86

--- scanner 11 ---
-708,-978,-663
715,-781,-742
680,759,-622
-262,303,742
819,750,-718
-406,333,733
881,-642,685
-701,-920,-668
654,-831,-823
557,760,713
565,725,753
940,-709,735
-683,664,-675
933,-652,865
162,-1,80
-484,-431,902
-599,740,-760
-596,555,-727
763,-827,-746
-366,259,628
-609,-453,917
-728,-782,-614
-640,-408,917
463,792,860
30,-171,-39
742,714,-656

--- scanner 12 ---
-557,-594,-751
535,-288,-328
522,-780,851
577,462,-599
-378,644,873
506,-435,-371
562,-830,712
490,514,-708
-404,624,-594
620,-862,805
30,-32,-49
-485,-641,-711
-470,546,874
445,575,738
442,-439,-322
-423,587,-766
-722,-561,777
-794,-556,932
472,521,876
-426,598,795
-666,-473,931
465,526,934
-41,29,133
-673,-676,-778
-465,559,-756
574,429,-784

--- scanner 13 ---
-512,-780,-383
-634,-328,271
850,517,723
409,-606,298
-639,-724,-396
380,-651,335
100,-55,35
-752,362,545
735,371,731
739,589,-529
-411,665,-757
656,-371,-623
827,524,-467
708,-458,-468
833,537,-431
-488,714,-796
-610,-752,-415
749,-364,-620
-509,-350,256
-478,-305,256
-856,406,431
361,-673,410
741,335,723
-436,839,-789
22,58,-107
-712,436,440

--- scanner 14 ---
-460,675,-680
431,-518,-720
410,-453,-630
546,395,-372
-809,-922,-637
-476,-447,567
-385,609,-563
-679,-919,-628
496,378,-277
481,-504,704
-668,-866,-537
401,-490,-849
-432,672,-630
606,-464,582
711,311,823
-589,487,725
848,340,840
2,45,75
-564,530,750
559,334,-368
783,465,874
-533,-497,381
-480,543,672
-425,-501,480
451,-443,592
95,-118,129

--- scanner 15 ---
901,-715,774
-447,800,-713
-499,-598,656
-530,755,-704
613,743,-575
-57,-132,9
863,-629,781
859,628,565
383,-740,-702
563,-720,-625
698,786,-655
388,-660,-608
802,739,635
-499,781,-595
-391,-575,672
-415,-629,-674
-438,-641,-548
697,690,-533
-545,430,383
-442,-688,-695
882,-510,730
-505,546,386
-456,-456,756
798,535,704
100,-5,81
-601,588,418

--- scanner 16 ---
645,-660,-815
686,-400,877
-788,541,-551
-582,-777,-582
549,-417,906
-726,-490,687
0,107,10
-169,-72,58
621,-366,752
-751,-515,730
800,-659,-703
-545,700,768
725,515,512
-482,-755,-724
-571,851,725
-816,457,-490
737,678,-369
557,694,-371
-648,-739,-637
717,434,447
-642,-568,674
-880,524,-442
585,-617,-730
706,640,505
492,702,-362
-590,696,763

--- scanner 17 ---
-786,419,-780
580,542,522
-753,-461,678
-671,506,-702
660,-476,-725
-723,-515,665
491,710,-689
-659,-635,-482
460,561,-702
4,11,-37
-133,118,35
-477,757,820
-550,-652,-390
-828,-586,678
-449,680,728
554,557,-634
598,607,360
539,-552,439
686,533,440
-412,869,783
617,-325,-699
543,-372,449
613,-462,562
-598,-719,-454
-573,373,-764
655,-480,-680

--- scanner 18 ---
-626,741,779
-621,839,736
870,-429,299
-329,-456,-530
-481,-415,-436
474,-784,-723
769,358,-552
-484,-508,-443
862,-314,445
72,37,-49
707,453,-662
-564,805,649
862,-423,263
-328,-337,452
757,548,-556
437,-755,-678
544,738,582
-292,-331,465
483,751,701
-633,427,-669
-390,-425,517
505,825,670
-724,453,-680
-534,474,-658
549,-803,-745

--- scanner 19 ---
885,-364,-767
-758,-757,-795
-620,501,549
-808,378,-727
-652,-436,692
-654,530,693
563,-461,767
-36,-26,43
-602,-465,731
-643,533,789
754,-346,-734
81,100,-56
697,508,518
614,-468,555
541,493,-604
-818,-793,-838
484,497,-614
872,-359,-899
-882,491,-615
-891,-677,-813
519,-462,626
-817,602,-732
844,429,496
576,675,-637
-600,-367,828
794,577,548

--- scanner 20 ---
521,799,641
-814,-478,803
564,734,-498
-816,-536,666
-505,755,-478
416,854,546
-715,-478,664
626,864,-562
446,818,519
346,-834,447
322,-829,393
633,-584,-876
-483,786,-552
-488,391,596
724,-595,-850
-397,424,594
534,-573,-829
621,821,-464
-544,667,-565
-757,-471,-755
-47,63,-74
259,-760,415
-707,-577,-850
-435,554,651
-769,-467,-945

--- scanner 21 ---
693,-762,-734
616,828,628
-684,-626,636
-595,479,-482
-383,525,-446
674,-607,-772
-568,-625,-730
-392,572,557
782,844,715
682,866,573
-445,-535,-656
-363,646,506
448,-368,481
56,54,26
-671,-572,606
-72,160,-43
513,604,-739
538,579,-696
476,-452,542
-684,-501,711
384,-435,433
-455,-715,-660
-471,488,-412
622,657,-683
-315,512,408
719,-611,-777

--- scanner 22 ---
-426,-460,-817
430,-548,757
17,-21,65
645,-446,-810
-502,-376,-732
702,-493,-691
312,721,-517
477,-544,865
283,-543,859
-616,518,-622
676,-492,-734
332,738,-554
-96,88,-105
-863,-797,685
-934,800,393
-454,579,-687
498,874,776
-752,909,393
-611,-463,-784
473,807,749
535,829,715
-898,794,385
369,646,-663
-444,537,-659
-907,-693,571
-833,-653,676

--- scanner 23 ---
614,503,499
-741,-573,-430
666,496,-326
586,590,-331
-874,485,504
70,134,-24
-764,665,-640
709,-743,589
-855,603,556
-860,543,-641
-739,-631,-531
680,-638,-489
-799,-406,850
870,-642,-477
-67,-30,81
801,-715,512
-878,-631,-432
-783,-619,896
764,-780,400
567,650,495
-829,494,587
555,524,472
-817,623,-556
646,573,-506
775,-532,-539
-721,-521,763

--- scanner 24 ---
-804,350,366
456,-494,-642
363,-619,389
-411,-594,552
261,606,745
284,599,-445
341,-681,423
372,534,-507
278,586,-409
-878,381,388
-1,-117,40
-791,469,427
-127,5,-73
-670,-495,-397
435,-446,-785
543,-509,-828
-403,-518,594
-734,-428,-464
296,-644,362
-858,402,-681
376,752,753
-849,-457,-445
-892,315,-738
370,788,745
-794,272,-743
-418,-406,604

--- scanner 25 ---
86,149,-150
-572,-670,-518
748,-704,-522
569,-536,361
-723,-522,310
563,566,-778
-419,589,553
-665,-309,333
-597,-464,-470
829,-696,-457
739,901,432
806,837,480
561,-471,352
-628,-622,-575
722,809,541
-623,602,-609
585,-716,359
623,-685,-515
-470,657,-672
-474,589,-578
412,592,-735
-286,527,455
1,56,27
567,611,-773
-332,511,668
-768,-400,393)";
