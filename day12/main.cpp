#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <ctype.h>
#include <numeric>

extern char const* pTest;
extern char const* pTest2;
extern char const* pTest3;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;
using Model = std::vector<std::pair<std::string,std::string>>;

std::pair<std::string,std::string> split(std::string const& line) {
  auto pos = line.find('-');
  std::string left = line.substr(0,pos);
  std::string right = line.substr(pos+1);
  return {left,right};
}

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (in >> line) {
      std::cout << "\nentry:" << line;
      auto [start, end] = split(line);
      std::cout << "\nsplit: " << start << " and " << end;
      result.push_back({start,end});
    }
    return result;
}

using Vertex = std::string;
using Path = std::vector<Vertex>; 
using Paths = std::vector<Path>; 
using Graph = std::map<Vertex,std::set<Vertex>>;
Graph create_adjacent_graph(auto const& edges) {
  Graph result;
  for (auto const& edge : edges) {
    result[edge.first].insert(edge.second);
    result[edge.second].insert(edge.first);
  }
  return result;
}
std::string to_string(Path const& path) {
  std::string result{};
  int call_count{0};
  result = std::accumulate(path.begin(),path.end(),std::string{},[&call_count](auto acc,auto vertex) {
    if (call_count++>0) {
      acc += std::string{","} + vertex;
    }
    else acc += vertex;
    return acc;
  });
  return result;
}
bool is_full_path(Path const& path) {
  bool result (path.back() == "end");
  // if (result) std::cout << "\nis_full_path " << to_string(path);
  return result;
}
bool is_all_lower(Vertex const& vertex) {
  return std::all_of(vertex.begin(),vertex.end(),[](auto ch) {return std::islower(ch);});
}
int vertex_count(Path const& path,Vertex const& vertex) {
  return std::accumulate(path.begin(),path.end(),int{0},[&vertex](auto acc, auto const& vx) {
    acc += (vx==vertex)?1:0;
    return acc;
  });
}
namespace part1 {
  bool is_valid_path(Path const& path) {
    bool result{true};
    for (auto const& vertex : path) {
      if (is_all_lower(vertex) == true and vertex_count(path,vertex)>1) {
        result = false;
      }
    }
    return result;
  }
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto edges = parse(in);
      // print
      {
        for (auto const& edge : edges) {
          std::cout << "\nedge:" << edge.first << " <=> " << edge.second;
        }
      }
      auto adjacent_graph = create_adjacent_graph(edges);
      // print
      {
        for (auto const& entry : adjacent_graph) {
          std::cout << "\nvertex\"" << entry.first << "\" -> ";
          for (auto const& vertex : entry.second) {
            std::cout << " \"" << vertex << "\"";
          }
        }
      }
      {
        // breadth first search for valid paths
        Paths candidate_paths{{"start"}};
        Paths valid_paths{};
        while (candidate_paths.size()>0) {
          // next candidate
          auto candidate_path = candidate_paths.back();
          candidate_paths.pop_back();
          if (is_full_path(candidate_path)) {
            valid_paths.push_back(candidate_path);
          }
          else if (is_valid_path(candidate_path)) {
            // new candidates = extend with connected vertecies
            for (auto const& new_candidate_vertex: adjacent_graph[candidate_path.back()]) {
              auto new_candidate_path = candidate_path;
              new_candidate_path.push_back(new_candidate_vertex);
              candidate_paths.push_back(new_candidate_path);
              // rotate candidates right to move new candidate back in line 
              std::rotate(candidate_paths.begin(),candidate_paths.end()-1,candidate_paths.end());
            };
          }
        }
        result = valid_paths.size();
      }
      return result;
  }
}

namespace part2 {
//   std::set<Path> small_test_valid_paths() {
//     // create set with paths for small test
//     std::set<Path> result{};
//     char const* pValidPaths = R"(start,A,b,A,b,A,c,A,end
// start,A,b,A,b,A,end
// start,A,b,A,b,end
// start,A,b,A,c,A,b,A,end
// start,A,b,A,c,A,b,end
// start,A,b,A,c,A,c,A,end
// start,A,b,A,c,A,end
// start,A,b,A,end
// start,A,b,d,b,A,c,A,end
// start,A,b,d,b,A,end
// start,A,b,d,b,end
// start,A,b,end
// start,A,c,A,b,A,b,A,end
// start,A,c,A,b,A,b,end
// start,A,c,A,b,A,c,A,end
// start,A,c,A,b,A,end
// start,A,c,A,b,d,b,A,end
// start,A,c,A,b,d,b,end
// start,A,c,A,b,end
// start,A,c,A,c,A,b,A,end
// start,A,c,A,c,A,b,end
// start,A,c,A,c,A,end
// start,A,c,A,end
// start,A,end
// start,b,A,b,A,c,A,end
// start,b,A,b,A,end
// start,b,A,b,end
// start,b,A,c,A,b,A,end
// start,b,A,c,A,b,end
// start,b,A,c,A,c,A,end
// start,b,A,c,A,end
// start,b,A,end
// start,b,d,b,A,c,A,end
// start,b,d,b,A,end
// start,b,d,b,end
// start,b,end)";
//     std::istringstream lines{pValidPaths};
//     std::string line{};
//     while (std::getline(lines,line)) {
//       std::istringstream vertecies{line};
//       std::string vertex{};
//       Path path{};
//       while (std::getline(vertecies,vertex,',')) {
//         path.push_back(vertex);
//       }
//       result.insert(path);
//     }
//     std::cout << "\ntest paths count : " << result.size();
//     return result;
//   }
  bool single_small_cave_max_twice(Path const& path,Vertex const& vertex_to_add) {
    bool result{true};
    std::set<Vertex> small_caves_in_path{};
    int small_cave_twin_count = std::accumulate(path.begin(),path.end(),int{0},[&small_caves_in_path](auto acc,Vertex const& vertex){
      if (is_all_lower(vertex)) {
        // small cave
        if (small_caves_in_path.contains(vertex)) {
          // small cave vertex already in preceeding path
          acc += 1; // count as twin
        }
        small_caves_in_path.insert(vertex);
      }
      return acc;
    });
    result = small_cave_twin_count==0; // No twins before adding new_vertex
    // std::cout << "\n\t single_small_cave_max_twice for " << to_string(path) << " is " << result;
    return result;
  }
  bool is_valid_candidate(Path const& path,Vertex const& vertex_to_add) {
    bool result{false};
    for (auto const& vertex : path) {
      if (vertex_to_add == "start") result = false;
      else if (is_all_lower(vertex_to_add) == false or vertex_count(path,vertex_to_add)==0) result = true;
      else if (single_small_cave_max_twice(path,vertex_to_add)) result = true;
      else result = false;
    }
    // std::cout << "\nis_valid_candidate " << to_string(path) << " = " << result;
    return result;
  }
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto edges = parse(in);
      // print
      {
        for (auto const& edge : edges) {
          std::cout << "\nedge:" << edge.first << " <=> " << edge.second;
        }
      }
      auto adjacent_graph = create_adjacent_graph(edges);
      // print
      {
        for (auto const& entry : adjacent_graph) {
          std::cout << "\nvertex\"" << entry.first << "\" -> ";
          for (auto const& vertex : entry.second) {
            std::cout << " \"" << vertex << "\"";
          }
        }
      }
      {
        // breadth first search for valid paths
        Paths candidate_paths{{"start"}};
        std::vector<Path> valid_paths{};
        const size_t LOOP_LOG_THRESHOLD = 10000;
        size_t loop_count{LOOP_LOG_THRESHOLD};
        std::cout << "\n{candidates\t\tmatches} ";
        while (candidate_paths.size()>0) {
          if (loop_count++%10000==0) {
            std::cout << "\n" << candidate_paths.size();
            std::cout << "\t\t" << valid_paths.size();
          }
          // next candidate
          auto candidate_path = candidate_paths.back();
          candidate_paths.pop_back();
          // std::cout << "\ncandidate_path " << to_string(candidate_path);          
          if (is_full_path(candidate_path)) {
            valid_paths.push_back(candidate_path);
            // std::cout << "\nfound: " << to_string(candidate_path);
          }
          else {
            // produce new valid candidates paths                        
            for (auto const& new_candidate_vertex: adjacent_graph[candidate_path.back()]) {
              auto new_candidate_path = candidate_path;
              if (is_valid_candidate(new_candidate_path,new_candidate_vertex)) {
                new_candidate_path.push_back(new_candidate_vertex);
                candidate_paths.push_back(new_candidate_path);
                // rotate candidates right to create breadth first (otherwise we will do depth first)
                // std::rotate(candidate_paths.begin(),candidate_paths.end()-1,candidate_paths.end());
              }
            }
          }
        }
        result = valid_paths.size();
        {
          // check against small test result
          // auto valid_test_paths = small_test_valid_paths();
          // for (auto const& path : valid_test_paths) {
          //   if (valid_paths.contains(path)) {
          //     std::cout << "\nfound : " << to_string(path);
          //   }
          //   else {
          //     std::cout << "\nmissed : " << to_string(path);
          //   }
          // }
        }
      }
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1 Test 2",part1::solve_for(pTest2)});
  // answers.push_back({"Part 1 Test 3",part1::solve_for(pTest3)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});

  // answers.push_back({"Part 1 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 1 Test 2",part2::solve_for(pTest2)});
  // answers.push_back({"Part 1 Test 3",part2::solve_for(pTest3)});
  answers.push_back({"Part 1     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)";
char const* pTest2 = R"(dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc)";
char const* pTest3 = R"(fs-end
he-DX
fs-he
start-DX
pj-DX
end-zg
zg-sl
zg-pj
pj-he
RW-he
fs-DX
pj-RW
zg-RW
start-pj
he-WI
zg-he
pj-fs
start-RW)";
char const* pData = R"(yw-MN
wn-XB
DG-dc
MN-wn
yw-DG
start-dc
start-ah
MN-start
fi-yw
XB-fi
wn-ah
MN-ah
MN-dc
end-yw
fi-end
th-fi
end-XB
dc-XB
yw-XN
wn-yw
dc-ah
MN-fi
wn-DG)";
