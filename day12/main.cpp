#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

extern char const* pTest;
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
using Path = std::vector<std::string>;
using Paths = std::vector<Path>;

Paths all_paths(std::string const& v1,std::string const& v2,Graph const& graph) {
  Paths result{};
  if (v1 != v2) {
    for (auto const v : graph.at(v1)) {
      // v is connected to v1
      // Do NOT backtrack! How?
      Paths paths = all_paths(v,v2,graph);
      for (auto const& path : paths) result.push_back(path);
    }
  }
  return result;
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
      return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
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

char const* pTest = R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)";
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
