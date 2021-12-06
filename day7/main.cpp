#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

namespace tokenize {
  // Python> start,end = line.split('->')
  struct Splittable {
    using Splittables = std::vector<Splittable>;
    using T = std::string;
    T token{};
    operator std::string() const {return token;}
    operator char const*() const {return token.c_str();}
    size_t size() {return token.size();}

    Splittable strip() {
      auto first = std::find_if(std::begin(token), std::end(token), [](auto ch) {
        return (std::isspace(ch) == 0);
      });
      if (first!=std::end(token)) {
        auto last = std::find_if(std::rbegin(token), std::rend(token), [](auto ch) {
          return (std::isspace(ch) == 0);
        });
        return {T{first,last.base()}};
      }
      else return {T{}};
    }
    std::pair<Splittable,Splittable> split(T const& delim) const {
      if (auto pos = token.find(delim);pos!=T::npos) {
        std::cout << "\nsplitting " << token << " on " << delim;
        return {{token.substr(0,pos)},{token.substr(pos+delim.size(),token.size()-pos)}};
      }
      else return {{std::string{"#split error# on delim \""} + delim + "\" of " + token},{}};
    }
    std::pair<Splittable,Splittable> split(char delim) const {return split(T{delim});}
    Splittables splits(T const& delim) const {
      Splittables result{};
      auto temp = *this;
      while (temp.size()>0) {
        auto [left,right] = temp.split(delim);
        result.push_back(left);
        temp = right;
      }
      return result;
    }
    Splittables splits(char delim) const {return splits(T{delim});}
  };

  struct Lines {
    using T = Splittable::T;
    using Splittables = Splittable::Splittables;

    static Splittables open(std::istream& in) {
      Splittables result;
      T line{};
      while (std::getline(in, line)) {
        result.push_back(Splittable{line});
      }
      return result;
    }
    static Splittables open(char const* pData) {
      std::stringstream in{pData};
      return open(in);
    }
  };
}

namespace part1 {
  Result solve_for(char const* pData) {
      return {};
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      return {};
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  answers.push_back({"Part 1     ",part1::solve_for(pTest)});
  answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pTest)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\n";
  return 0;
}

char const* pTest = R"()";
char const* pData = R"()";
