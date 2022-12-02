#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <sstream>
#include <algorithm>

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
      auto [start, end] = split(line);
      result.push_back({start,end});
    }
    return result;
}

namespace part1 {

  class Graph {
  private:
      using Bag = std::vector<int>;
      int m_V;
      int m_E;
      std::vector<Bag> m_adj;
      // throw an IllegalArgumentException unless {@code 0 <= v < V}
      void validateVertex(int v) {
          if (v < 0 || v >= m_V)
              throw std::runtime_error(std::string{"vertex "} + std::to_string(v) + " is not between 0 and " + std::to_string(m_V-1));
      }
  public:
      // Construct Graph with V vertices and zero edges
      Graph(int V=0)
      : m_V{V},m_E{},m_adj{(m_V>=0)?static_cast<size_t>(m_V):0,Bag{}} {
          if (m_V < 0) throw std::runtime_error("Number of vertices must be non-negative");
          std::cout << "\nGraph(" << m_V << ")";
      }
      Graph(Graph const& G) = default;
      int V() {return m_V;}
      int E() {return m_E;}
      void addEdge(int v, int w) {
          validateVertex(v);
          validateVertex(w);
          ++m_E;
          m_adj[v].push_back(w);
          m_adj[w].push_back(v);
      }
      auto adj(int v) {
          validateVertex(v);
          return m_adj[v];
      }
      int degree(int v) {
          validateVertex(v);
          return static_cast<int>(m_adj[v].size());
      }
      std::string to_string() const {
          std::ostringstream os{};
          os << m_V << " vertices, " << m_E << " edges " << "\n";
          for (int v = 0; v < m_V; v++) {
              os << to_string(v) << ": ";
              for (auto const& w : m_adj[v]) {
                  os << to_string(w) << " ";
              }
              os << "\n";
          }
          return os.str();
      }
      virtual std::string to_string(int v) const {
        return std::to_string(v);
      }
      // Cast to std::string
      operator std::string() const {return this->to_string();}
      friend std::ostream& operator<<(std::ostream& os,Graph const& g);
  };
  std::ostream& operator<<(std::ostream& os,Graph const& G) { os << static_cast<std::string>(G);return os;}

  class StringGraph : public Graph {
  public:
    using String = std::string;
    StringGraph(int V=0) : Graph{V} {};
    void addEdge(String v, String w) {
      Graph::addEdge(to_index(v),to_index(w));
      std::cout << "\nafter addEdge {" << v << ":" << w << "}";
      std::cout << "\n" << *this;
    }

  private:
    virtual String to_string(int v) const {
      String result{};
      if (auto iter = std::find_if(
         m_vertex.begin()
        ,m_vertex.end()
        ,[v](auto entry){
          return (entry.second==v);
        }); iter != m_vertex.end()) {
        result = iter->first;
      }
      else {
        result = String{"??"} + Graph::to_string(v) + "??"; 
      }
      return result + ":" + Graph::to_string(v);
    }
    int to_index(String name) {      
      int result = -1;
      auto iter = m_vertex.find(name);
      if (iter == m_vertex.end()) {
        auto [inserted,success] = m_vertex.insert({name,m_vertex.size()}); // index is 0,1,2,...
        if (success) result = inserted->second;
      }
      else {
        result = iter->second;
      }
      return result;
    }
    std::map<std::string,int> m_vertex{};
  };

  std::set<std::string> to_symbol_table(Model const& data_model) {
    std::set<std::string> result{};
    for (auto const& edge : data_model) {
      result.insert(edge.first);
      result.insert(edge.second);
    }
    return result;
  }

  Result solve_for(char const* pData) {
      Result result{};
      std::cout << "\nin=" << std::quoted(pData);
      std::stringstream in{ pData };
      auto data_model = parse(in);
      auto symbol_table = to_symbol_table(data_model);
      StringGraph G{static_cast<int>(symbol_table.size())};
      for (auto const& edge : data_model) {
        G.addEdge(edge.first,edge.second);
      }
      std::cout << "\nGraph";
      std::cout << "\n" << G;
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
