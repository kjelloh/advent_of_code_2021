#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <variant>
#include <optional>

extern std::vector<std::string> pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

/*
inp a - Read an input value and write it to variable a.
add a b - Add the value of a to the value of b, then store the result in variable a.
mul a b - Multiply the value of a by the value of b, then store the result in variable a.
div a b - Divide the value of a by the value of b, truncate the result to an integer, then store the result in variable a. (Here, "truncate" means to round the value toward zero.)
mod a b - Divide the value of a by the value of b, then store the remainder in variable a. (This is also called the modulo operation.)
eql a b - If the value of a and b are equal, then store the value 1 in variable a. Otherwise, store the value 0 in variable a.
*/
enum Operation {
  op_unknown
  ,op_inp
  ,op_add
  ,op_mul
  ,op_div
  ,op_mod
  ,op_eql
  ,op_undefined
};
using Operand = std::variant<char,int>;
using Operands = std::vector<Operand>;
std::optional<Operand> to_operand(std::string const& s) {
  try {
    return std::stoi(s);
  }
  catch (std::exception const& e) {}
  if (s.size()==1) return s[0];
  else return std::nullopt;
}
std::string to_string(Operation const& op) {
  switch (op) {
    case op_inp: return "inp";
    case op_add: return "add";
    case op_mul: return "mul";
    case op_div: return "div";
    case op_mod: return "mod";
    case op_eql: return "eql";
    default: return std::string{"op ?? "} + std::to_string(static_cast<int>(op));
  }
}
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
std::string to_string(Operand const& opr) {
  std::string result{};
  std::visit(overloaded {
             [&result](char arg) { result = arg;}
            ,[&result](int arg) { result = std::to_string(arg);}
        }, opr);
  return result;
}
class Statement {
public:
  Statement(std::string const& sOp,std::string const& sopr1,std::string const& sopr2) {
    this->m_op = op_undefined;
    bool done{false};
    for (int op = op_unknown;op<op_undefined;op++) {
      switch (op) {
        case op_inp: done = (sOp=="inp"); break;
        case op_add: done = (sOp=="add"); break;
        case op_mul: done = (sOp=="mul"); break;
        case op_div: done = (sOp=="div"); break;
        case op_mod: done = (sOp=="mod"); break;
        case op_eql: done = (sOp=="eql"); break;
      }
      if (done) {
        m_op = static_cast<Operation>(op);
        break;
      }
    }
    if (auto opr = to_operand(sopr1);opr) m_operands.push_back(*opr); 
    if (auto opr = to_operand(sopr2);opr) m_operands.push_back(*opr);
  }
  Operation const& op() const {return m_op;}
  Operands const& operands() const {return m_operands;}

private:
  Operation m_op;
  Operands m_operands;
};
using Program = std::vector<Statement>;
using Model = Program;

using Environment = std::map<char,int>;
class ALU {
public:
  ALU& execute(Program const& program) {
    for (auto const& statement : program) {
      // log
      {
        std::cout << "\nexecute: " 
          << to_string(statement.op()) << "|" 
          << to_string(statement.operands()[0]);
        if (statement.operands().size()>1) std::cout << "|" << to_string(statement.operands()[1]);
      }
      switch (statement.op()) {
        case op_inp: {} break;
        case op_add: {} break;
        case op_mul: {} break;
        case op_div: {} break;
        case op_mod: {} break;
        case op_eql: {} break;
      }
    }
    return *this;
  }
private:
  Environment environment;
};

std::pair<std::string,std::string> split(std::string const& token,std::string const& delim) {
  auto pos = token.find(delim);
  auto left = token.substr(0,pos);
  auto right = token.substr(pos+delim.size());
  if (pos == std::string::npos) return {left,""};
  else return {left,right};
}

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in,line)) {
      auto [sOp,sOperands] = split(line," ");
      // std::cout << "\n" << sOp << " | " << sOperands; 
      auto [sopr1,sopr2] = split(sOperands," ");
      // std::cout << "\n[" << sOp << "|" << sopr1 << "|" << sopr2 << "]";
      Statement statement{sOp,sopr1,sopr2};
      result.push_back(statement);
    }
    return result;
}

namespace part1 {
  Result solve_for(std::string const sData) {
      Result result{};
      std::stringstream in{ sData };
      auto program = parse(in);
      ALU alu{};
      alu.execute(program);
      return result;
  }
}

namespace part2 {
  Result solve_for(std::string const sData) {
      Result result{};
      std::stringstream in{ sData };
      auto data_model = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test 0",part1::solve_for(pData[0])});
  answers.push_back({"Part 1 Test 1",part1::solve_for(pData[1])});
  answers.push_back({"Part 1 Test 2",part1::solve_for(pData[2])});
  answers.push_back({"Part 1       ",part1::solve_for(pData[3])});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

std::vector<std::string> pData{
  R"(inp x
mul x -1)"
  ,R"(inp z
inp x
mul z 3
eql z x)"
  ,R"(inp w
add z w
mod z 2
div w 2
add y w
mod y 2
div w 2
add x w
mod x 2
div w 2
mod w 2)"
  ,R"(inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 14
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 13
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 8
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 4
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 10
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 10
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -3
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 14
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -4
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 10
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 4
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -8
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 14
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -3
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 1
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 6
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 14
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 0
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -6
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 9
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 13
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 12
mul y x
add z y)"
};
