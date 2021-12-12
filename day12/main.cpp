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
using Graph = std::map<std::string,std::set<std::string>>;
Graph create_adjacent_graph(auto const& edges) {
  Graph result;
  for (auto const& edge : edges) {
    result[edge.first].insert(edge.second);
    result[edge.second].insert(edge.first);
  }
  return result;
}
bool all_lower(std::string const& s) {
  return std::accumulate(s.begin(),s.end(),true,[](auto acc,auto ch) {return acc &= std::islower(ch);});
}
using Path = std::vector<std::string>;
using Paths = std::vector<Path>;
using VisitCount = std::map<std::string,int>;
Paths all_paths(std::string const& v1,std::string const& v2,Graph const& graph,VisitCount visit_count) {
  Paths result{};
  visit_count[v1] += 1;
  if (v1 != v2) {
    for (auto const v : graph.at(v1)) {
      // v is connected to v1
      if (visit_count[v]==0 or all_lower(v)==false) {
        Paths paths = all_paths(v,v2,graph,visit_count); // 
        for (auto const& path : paths) {
          Path new_path{};
          new_path.push_back(v1);
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
      Path new_path{};
      new_path.push_back(v1);
    result.push_back(new_path);
  }
  return result;
}

Paths all_paths(std::string const& v1,std::string const& v2,Graph const& graph) {
  VisitCount visit_count{};
  return all_paths(v1,v2,graph,visit_count);
}

namespace part1 {
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
      Paths paths = all_paths("start","end",adjacent_graph);
      // print
      {
        for (auto const path : paths) {
          std::cout << "\nstart-end : ";
          for (auto const& v : path) std::cout << " -> " << v;
        }
      }
      result = paths.size();
      return result;
  }
}

namespace part2 {
  Paths all_paths(std::string const& v1,std::string const& v2,Graph const& graph,VisitCount visit_count,bool small_cave_twice) {
    Paths result{};
    if (v1 != v2) {
      auto cashed_visit_count = visit_count; 
      for (auto const v : graph.at(v1)) {
        visit_count = cashed_visit_count; // reset to incoming vsiit counts for each sub-path
        // v is connected to v1
        if (visit_count[v]==0 or all_lower(v)==false or (visit_count[v]==1 and all_lower(v)==true and !small_cave_twice and v!="start" and v!="end")) {
          visit_count[v] += 1;
          small_cave_twice = (visit_count[v]>0 and all_lower(v)==true);
          Paths paths = all_paths(v,v2,graph,visit_count,small_cave_twice);
          for (auto const& path : paths) {
            Path new_path{};
            new_path.push_back(v1);
            new_path.insert(new_path.end(),path.begin(),path.end());
            result.push_back(new_path);
            // print
            {
              std::cout << "\ncandidate path: ";
              for (auto const& v : new_path) {
                std::cout << " -> " << v << "[" << visit_count[v] << "]";
              }
              std::cout << " small_cave_twice " << small_cave_twice;
            }
          }          
        }
      }
    }
    else {
        Path new_path{};
        new_path.push_back(v1);
      result.push_back(new_path);
    }
    return result;
  }
  Paths all_paths(std::string const& v1,std::string const& v2,Graph const& graph) {
    VisitCount visit_count{};
    bool small_cave_twice = false;
    return all_paths(v1,v2,graph,visit_count,small_cave_twice);
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
      Paths paths = all_paths("start","end",adjacent_graph);
      // print
      {
        for (auto const path : paths) {
          std::cout << "\nstart-end : ";
          for (auto const& v : path) std::cout << " -> " << v;
        }
      }
      result = paths.size();
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

  answers.push_back({"Part 1 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 1 Test 2",part2::solve_for(pTest2)});
  // answers.push_back({"Part 1 Test 3",part2::solve_for(pTest3)});
  // answers.push_back({"Part 1     ",part2::solve_for(pData)});
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
