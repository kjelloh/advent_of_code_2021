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
#include <ranges>
#include <unordered_set>
#include <unordered_map>

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
// custom specialization of std::hash for Environment injected into namespace std
template<>
struct std::hash<Environment>
{
    std::size_t operator()(Environment const& env) const noexcept
    {
      std::size_t result;
      for (auto const me : env) {
        result ^= std::hash<char>{}(me.first);
        result ^= std::hash<int>{}(me.second);
      }
      return result;
    }
};
// custom specialization of std::hash for std::tuple<int,char,Environment> injected into namespace std
template<>
struct std::hash<std::tuple<int,char,Environment>>
{
    std::size_t operator()(std::tuple<int,char,Environment> const& triad) const noexcept
    {
      std::size_t result;
      result ^= std::hash<int>{}(std::get<int>(triad));
      result ^= std::hash<int>{}(std::get<char>(triad));
      for (auto const me : std::get<Environment>(triad)) {
        result ^= std::hash<char>{}(me.first);
        result ^= std::hash<int>{}(me.second);
      }
      return result;
    }
};
// custom specialization of std::hash for std::pair<int,size_t> injected into namespace std
template<>
struct std::hash<std::pair<int,size_t>>
{
    std::size_t operator()(std::pair<int,size_t> const& pair) const noexcept
    {
      std::size_t result;
      result ^= std::hash<int>{}(pair.first);
      result ^= std::hash<int>{}(pair.second);
      return result;
    }
};

std::string to_string(Environment const& env) {
  std::ostringstream os{};
  os << "<environment>";
  for (auto const me : env) {
    os << " " << me.first << " = " << me.second;
  } 
  return os.str();
}
class ALU {
public:
  ALU(std::istream& in) : m_in{in} {}
  ALU& execute(Program const& program) {
    for (auto const& statement : program) {
      // log
      // if (true){
      //   std::cout << "\nexecute: " 
      //     << to_string(statement.op()) << "|" 
      //     << to_string(statement.operands()[0]);
      //   if (statement.operands().size()>1) std::cout << "|" << to_string(statement.operands()[1]);
      // }
      char a = std::get<char>(statement.operands()[0]);
      int b;
      if (statement.operands().size()==2) {
        std::visit(overloaded {
            [this,&b](char arg) {b = this->environment()[arg];}
          ,[this,&b](int arg) { b = arg;}
        }, statement.operands()[1]);        
      }
      
      switch (statement.op()) {
        case op_inp: {
          int b;
          // std::cout << " >"; 
          if (m_in >> b) {
            m_environment[a] = b;
          }
          else {
            std::cout << " ERROR: insufficient input";
          }
        } break;
        case op_add: {
          m_environment[a] += b;
        } break;
        case op_mul: {
          m_environment[a] *= b;
        } break;
        case op_div: {
          m_environment[a] /= b;
        } break;
        case op_mod: {
          m_environment[a] %= b;
        } break;
        case op_eql: {
          int val_a = m_environment[a];
          m_environment[a] = (val_a==b)?1:0;
        } break;
      }
      // std::cout << "\t" << a << " = " << m_environment[a];
    }
    // std::cout << "\n" << this->env_dump();
    return *this;
  }

  std::string env_dump() {
    return to_string(m_environment);
  }
  Environment& environment() {return m_environment;}
private:
  std::istream& m_in;
  Environment m_environment;
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

// Memoize on visitied state {digit index 13..0 int, z so far size_t} mapped to best digit string from here to z=0
// Use std::optional for the result to handle that there may be NO passable digits from current state
using Visited = std::unordered_map<std::pair<int,size_t>,std::optional<std::string>>;
std::optional<std::string> best_digits(int ix,std::vector<Program> const& snippets,size_t z,Visited& visited) {
  // Get the best digits i..0 for z so far
  // Init with ix=13, z=0
  // Expands to ix=12, z= one for each digit 13 '9','8',...
  // Victory if ix==0 and next z for a digit == 0 ==> return the victory digit 
  static int call_count{0};
  call_count++;
  // Log
  {
    if (ix>10) std::cout << "\n" << call_count << " " << ix << " " << visited.size() << " " << z;
  }
  if (ix<0) return std::nullopt; // Give up
  else if (visited.find({ix,z}) != visited.end()) return visited[{ix,z}];
  else {
    // Not memoized - run program for step 13-ix (step 13 for digit index 0 = last one)
    // for (char digit : {'9','8','7','6','5','4','3','2','1'}) {
    auto digits = {'9','8','7','6','5','4','3','2','1'};
    for (char digit : digits | std::views::reverse) { // part2
      std::string input{digit};
      std::istringstream d_in{input};
      ALU alu{d_in};
      alu.environment()['z'] = z; // Run with provided in z
      alu.execute(snippets[13-ix]);
      auto next_z = alu.environment()['z']; // Get next z
      if (ix==0 and next_z==0) return std::string{digit}; // VICTORY
      else {
        // pass the new z along down the chain unless we are done
        auto result = best_digits(ix-1,snippets,next_z,visited); // Recurse down
        visited[{ix-1,next_z}] = result; // Memoize best digit down from this state
        if (result) return std::string{digit} + result.value();
        else continue; // Try next digit
      }
    }
  }
  return std::nullopt;
} 

namespace part1 {
  void investigate(std::string const& sData) {
    std::stringstream in{ sData };
    auto program = parse(in);
    // Observation: The program reads one digit input at a time and performs
    // a set of computations on that input before reading and processing the next digit.

    // Split the program into snippets for each digit processing
    std::vector<Program> snippets{};
    for (auto const& statement : program) {
      if (statement.op() == op_inp) snippets.push_back({});
      snippets.back().push_back(statement);
    }
    std::cout << "\nsnippets count " << snippets.size();

    // 1. What are the possible program states after the first digit processing?
    if (false) {
      std::vector<Environment> envs{};
      for (char digit : {'9','8','7','6','5','4','3','2','1'}) {
        // Run the program from the start state with the digit and store the resulting state
        std::string input{digit};
        std::istringstream d_in{input};
        // Run snippet[0]
        ALU alu{d_in};
        alu.execute(snippets[0]);
        envs.push_back(alu.environment());
      }
      // Log
      {
        std::cout << "\n<possible states after first digit>"; 
        for (auto const& env : envs) std::cout << "\n" << to_string(env); 
      }
    }
    // 2. What possible states after each step (if we run each step/snippet in isolation)
    if (false) {
      std::vector<std::tuple<int,char,Environment>> envs{};
      for (int i=0;i<14;i++) {
        for (char digit : {'9','8','7','6','5','4','3','2','1'}) {
          // Run the program from the start state with the digit and store the resulting state
          std::string input{digit};
          std::istringstream d_in{input};
          // Run snippet[0]
          ALU alu{d_in};
          alu.execute(snippets[i]);
          envs.push_back({i,digit,alu.environment()});
        }
      }
      // Log
      {
        std::cout << "\n<possible states after each snippet in isolation per digit>"; 
        for (auto const& tripple : envs) std::cout 
          << "\n" 
          << std::get<int>(tripple) 
          << " " << std::get<char>(tripple)
          << " " << to_string(std::get<Environment>(tripple));
      }
    }
    // 3. How does each step/snippet process a large input z?
    if (false) {
      std::vector<std::tuple<int,char,Environment>> envs{};
      for (int i=0;i<14;i++) {
        for (char digit : {'9','8','7','6','5','4','3','2','1'}) {
          // Run the program from the start state with the digit and store the resulting state
          std::string input{digit};
          std::istringstream d_in{input};
          // Run snippet[0]
          ALU alu{d_in};
          // alu.environment()['z'] = 1000000; // large z 
          alu.environment()['z'] = 10000000; // even largee z 
          alu.execute(snippets[i]);
          envs.push_back({i,digit,alu.environment()});
        }
      }
      // Log
      {
        std::cout << "\n<possible states after each snippet in isolation per digit for LARGE in-z>"; 
        for (auto const& tripple : envs) std::cout 
          << "\n" 
          << std::get<int>(tripple) 
          << " " << std::get<char>(tripple)
          << " " << to_string(std::get<Environment>(tripple));
          /*
          These steps drastically reduced z

          4 7 <environment> w = 7 x = 0 y = 0 z = 384615
          5 6 <environment> w = 6 x = 0 y = 0 z = 384615
          7 2 <environment> w = 2 x = 0 y = 0 z = 384615
          8 7 <environment> w = 7 x = 0 y = 0 z = 384615
          11 4 <environment> w = 4 x = 0 y = 0 z = 384615

          ==> Seems we MUST choose 
            digit 4 = '7'
            digit 5 = '6'
            digit 7 = '2'
            digit 8 = '7'
            digit 11 = '4'

            to have any chance of reducing z down to 0?

          These steps also reduced z, although just a little bit
          9 3 <environment> w = 3 x = 1 y = 9 z = 9999999
          9 2 <environment> w = 2 x = 1 y = 8 z = 9999998
          9 1 <environment> w = 1 x = 1 y = 7 z = 9999997

         This step increased z very little

          13 1 <environment> w = 1 x = 1 y = 13 z = 10000003

          Lowest increase for the really bad steps
          0 1 <environment> w = 1 x = 1 y = 15 z = 260000015
          1 1 <environment> w = 1 x = 1 y = 9 z = 260000009
          2 1 <environment> w = 1 x = 1 y = 5 z = 260000005
          3 1 <environment> w = 1 x = 1 y = 11 z = 260000011
          6 1 <environment> w = 1 x = 1 y = 5 z = 260000005
          10 1 <environment> w = 1 x = 1 y = 1 z = 260000001
          12 1 <environment> w = 1 x = 1 y = 14 z = 260000014

          Speculation: The digits affects z very bad to very kindly in the following way

          digit     z affect
          0         BAD
          1         BAD
          2         BAD
          3         BAD
          4         '7' good
          5         '6' good
          6         BAD
          7         '2' good
          8         '7' good
          9         kind. '3','2','1' reduces z a little
          10        BAD
          11        '4' good
          12        BAD
          13        kind. all increase z but a small amount

          We have 7 very BAD digits.
          We have 2 kind digits
          We have 5 fixed digits!

          */
      }
    }
    // 4. Lets try the whole program for all possible BAD digits
    //    with 4,5,7,8,11 fixated
    //    9 set to the kind '1' and 13 to the kind '1'
    // NOMAD = NNNN76N271N4N1 where N is '9' for this test
    if (false) {
      //NOMAD =          xNNN76N271N4N1 where N is '9' for this test
      std::string nomad="9 9 9 9 7 6 9 2 7 1 9 4 9 1";
      std::vector<std::tuple<int,char,Environment>> envs{};
      for (int i : {0,1,2,3,6,10,12}) {
        for (char digit : {'9','8','7','6','5','4','3','2','1'}) {
          // Run the program from the start state with the digit and store the resulting state
          nomad[2*i] = digit;
          std::string input{nomad};
          std::istringstream d_in{input};
          ALU alu{d_in};
          alu.execute(program);
          envs.push_back({i,digit,alu.environment()});
        }
      }
      // Log
      {
        std::cout << "\n<possible states after each nomad xNNN76N271N4N1 (x='1'..'9'"; 
        for (auto const& tripple : envs) std::cout 
          << "\n" 
          << std::get<int>(tripple) 
          << " " << std::get<char>(tripple)
          << " " << to_string(std::get<Environment>(tripple));

          /*
          Ok, quite unconclosive but for digit 0 where we can control z to both negative and positive!

          0 9 <environment> w = 1 x = 1 y = 13 z = -1276661321
          0 8 <environment> w = 1 x = 1 y = 13 z = -1585577097
          0 7 <environment> w = 1 x = 1 y = 13 z = -1894492873
          0 6 <environment> w = 1 x = 1 y = 13 z =  2091558625
          0 5 <environment> w = 1 x = 1 y = 13 z =  1782642849
          0 4 <environment> w = 1 x = 1 y = 13 z =  1473727073
          0 3 <environment> w = 1 x = 1 y = 13 z =  1164811297
          0 2 <environment> w = 1 x = 1 y = 13 z =  855895521
          0 1 <environment> w = 1 x = 1 y = 13 z =  546979745
          */
      }
    }
    // What can we learn by brute force the 7 BAD digits?
    // If we store the environmet after each step we can get a grasp of the search space?
    if (false) {
      // Brute force digit with index {0,1,2,3,6,10,12}
      std::unordered_set<Environment> visited{};
      size_t call_count{0};
      std::string nomad="9 9 9 9 7 6 9 2 7 1 9 4 9 1";
      for (auto i0 : {1,2,3,4,5,6,7,8,9}) {
        for (auto i1 : {1,2,3,4,5,6,7,8,9}) {
          for (auto i2 : {1,2,3,4,5,6,7,8,9}) {
            for (auto i3 : {1,2,3,4,5,6,7,8,9}) {
              for (auto i6 : {1,2,3,4,5,6,7,8,9}) {
                for (auto i10 : {1,2,3,4,5,6,7,8,9}) {
                  for (auto i12 : {1,2,3,4,5,6,7,8,9}) {
                    ++call_count;
                    nomad[0] = '0'+i0;
                    nomad[2] = '0'+i1;
                    nomad[4] = '0'+i2;
                    nomad[6] = '0'+i3;
                    nomad[12] = '0'+i6;
                    nomad[20] = '0'+i10;
                    nomad[24] = '0'+i12;
                    std::string input{nomad};
                    std::istringstream d_in{input};
                    ALU alu{d_in};
                    alu.execute(program);
                    visited.insert(alu.environment());
                    if (call_count%10000==0) std::cout << "\n" << call_count << std::flush;
                  }
                }
              }
            }
          }
        }
      }
      // Log
      {
        std::cout << "\n" << call_count << std::flush;
        std::cout << "\n" << visited.size();
        // 2187 unique states only! (Should be correct even with a sloppy hash function)?
        // Note: I implemented a quick-and-dirty hash function for Envronment.
        //       But the unordered set should still keep track if unique elements
        //       even if the hash but them in buckets defined by the same hash? 
      }
    }
    // What can we learn by brute force the 7 BAD digits and map the environment to the tested nomad?
    if (false) {
      // Brute force BAD digits,i.e., those with index {0,1,2,3,6,10,12}      
      std::unordered_map<Environment,std::string> visited{};
      size_t call_count{0};
      std::string nomad="9 9 9 9 7 6 9 2 7 1 9 4 9 1";
      for (auto i0 : {1,2,3,4,5,6,7,8,9}) {
        for (auto i1 : {1,2,3,4,5,6,7,8,9}) {
          for (auto i2 : {1,2,3,4,5,6,7,8,9}) {
            for (auto i3 : {1,2,3,4,5,6,7,8,9}) {
              for (auto i6 : {1,2,3,4,5,6,7,8,9}) {
                for (auto i10 : {1,2,3,4,5,6,7,8,9}) {
                  for (auto i12 : {1,2,3,4,5,6,7,8,9}) {
                    ++call_count;
                    nomad[0] = '0'+i0;
                    nomad[2] = '0'+i1;
                    nomad[4] = '0'+i2;
                    nomad[6] = '0'+i3;
                    nomad[12] = '0'+i6;
                    nomad[20] = '0'+i10;
                    nomad[24] = '0'+i12;
                    std::string input{nomad};
                    std::istringstream d_in{input};
                    ALU alu{d_in};
                    alu.execute(program);
                    visited[alu.environment()] = nomad;
                    if (call_count%10000==0) std::cout << "\n" << call_count << " " << nomad << std::flush;
                  }
                }
              }
            }
          }
        }
      }
      // Log
      {
        std::cout << "\n" << call_count << std::flush;
        std::cout << "\n" << visited.size();
        // 2187
        for (auto const& em : visited) {
          if (em.first.at('z') == 0) std::cout << "\nvalid:" << em.second;
        }
        // But none of them are z=0.
      }
      // How low z have we found (Note that we have two kind digits to find also)
      std::vector<Environment> qv{};
      std::transform(visited.begin(),visited.end(),std::back_inserter(qv),[](auto em) {
        return em.first;
      });
      std::sort(qv.begin(),qv.end(),[](auto em1,auto em2){
        return (em1['z'] < em2['z']);
      });
      for (auto const& env : qv) {
        std::cout << "\n" << to_string(env);
      }
      /*
        Observation: For some reason we get the same w,x and y for the input we generated.
        Probably because the last digit processed is the same and w,x and y does NOT
        propagate between "steps" (digit processing)
        And we basically get two chunks of z, negative and one positive (large in any case)
        ==> So much for getting close to a low z this way...
        ...
        <environment> w = 1 x = 1 y = 13 z = -1277575273
        <environment> w = 1 x = 1 y = 13 z = -1277118323
        <environment> w = 1 x = 1 y = 13 z = -1277118297
        <environment> w = 1 x = 1 y = 13 z = -1276661347
        <environment> w = 1 x = 1 y = 13 z = -1276661321
        <environment> w = 1 x = 1 y = 13 z = 182426387
        <environment> w = 1 x = 1 y = 13 z = 182443963
        <environment> w = 1 x = 1 y = 13 z = 182461539
        <environment> w = 1 x = 1 y = 13 z = 182479115
        <environment> w = 1 x = 1 y = 13 z = 182496691
        <environment> w = 1 x = 1 y = 13 z = 182514267
        ...

        Still, for out input 9pow7 = 4782969
        We get only 2187 possible environments.
        We should be able to leverage that?

        ==> Even if we save all intermediate states we only get 14*2187 = 30618 states.

        Does this men we can brute force all 9 unknown digits
        by memoize encountered states to short-cut the evaluation?
      */
    }
    // *) Lets asume w,x and y does not affekt the "cost" to z for each "step" (digit in the input)?
    //    We are thyen interested in the possible space of z-changes for each step
    // given some z-input.
    if (false) {
      size_t const Z = 10000;
      std::unordered_map<std::tuple<int,char,Environment>,size_t> visited{};
      for (int i=0;i<14;i++) {
        // Run program snippet i for digit '9'..'1' for some range of input z 0..Z
        // count the number each state is reached
        // Note: Requires a specialisation to hash std::tuple<int,char,Environment> injected into
        // the std namespace (see code below Environment type above)
        for (auto digit : {'9','8','7','6','5','4','3','2','1'}) {
          for (size_t z = 0;z<Z;z++) {
            std::string input{digit};
            std::istringstream d_in{input};
            ALU alu{d_in};
            alu.environment()['z'] = z;
            alu.execute(snippets[i]);
            visited[{i,digit,alu.environment()}] += 1; // count reach of this state
          }
        }
      }
      // Log
      {
        std::cout << "\nstate count: " << visited.size();
        auto max_e_z = std::max_element(visited.begin(),visited.end(),[](auto const& entry1, auto const& entry2){
          auto z1 = std::get<Environment>(entry1.first).at('z');
          auto z2 = std::get<Environment>(entry2.first).at('z');
          return z1<z2;
        });
        auto max_z = std::get<Environment>(max_e_z->first).at('z');
        auto min_e_z = std::min_element(visited.begin(),visited.end(),[](auto const& entry1, auto const& entry2){
          auto z1 = std::get<Environment>(entry1.first).at('z');
          auto z2 = std::get<Environment>(entry2.first).at('z');
          return z1<z2;
        });
        auto min_z = std::get<Environment>(min_e_z->first).at('z');
        auto max_e_c = std::max_element(visited.begin(),visited.end(),[](auto const& entry1, auto const& entry2){
          auto c1 = entry1.second;
          auto c2 = entry2.second;
          return c1<c2;
        });
        auto max_c = max_e_c->second;
        auto min_e_c = std::min_element(visited.begin(),visited.end(),[](auto const& entry1, auto const& entry2){
          auto c1 = entry1.second;
          auto c2 = entry2.second;
          return c1<c2;
        });
        auto min_c = max_e_c->second;

        std::cout << "\nmin z    :" << std::get<int>(min_e_z->first) << " " << std::get<char>(min_e_z->first) << " " << to_string(std::get<Environment>(min_e_z->first)) << " " << min_e_z->second;
        std::cout << "\nmax z    :" << std::get<int>(max_e_z->first) << " " << std::get<char>(max_e_z->first) << " " << to_string(std::get<Environment>(max_e_z->first)) << " " << max_e_z->second;
        std::cout << "\nmin count:" << std::get<int>(min_e_c->first) << " " << std::get<char>(min_e_c->first) << " " << to_string(std::get<Environment>(min_e_c->first)) << " " << min_e_c->second;
        std::cout << "\nmax count:" << std::get<int>(max_e_c->first) << " " << std::get<char>(max_e_c->first) << " " << to_string(std::get<Environment>(max_e_c->first)) << " " << max_e_c->second;

        /*
          state count: 678496
          min z    :11 1 <environment> w = 1 x = 0 y = 0 z = 0 1
          max z    :0 9 <environment> w = 9 x = 1 y = 23 z = 259997 1
          min count:12 1 <environment> w = 1 x = 1 y = 14 z = 259988 1
          max count:13 2 <environment> w = 2 x = 1 y = 14 z = 9920 25

          So still, we have tried 10 000 z and 9 digits for each step and only got 678496 different states
          Worst case would have been 14*9*10000 = 1 260 000 (we have half the expected state counts...)
          We actually got one snippet to produce a zero z output (digit 11 input '1')

          Digit 12 = '1' is hit only once
          Digit 13 = '2' is hit 25 times (there are 25 input z:s for digit '2' that produce the same output z 9920)

        */

      }
    }
    // So, are we ready to just tey and brute force this?
    // It seems we care only about z at each step (digit). Adn we are looking for the highest sequence of 14 digits
    // that causes z to become 0.
    // And we know the space of states {digit index, z} are limited so we can memoize on this state.
    if (true) {
      // So, what question can we iterate?
      // Lets try - Given digit index i and z so far, what are the highets remaining digits to get z down to 0?
      // This qiestion can be asked at every level of digit index.
      // And at each level we can return the digit seuqence below us with the current digit at the front.
      // What is a good name for this function?
      // What about best_digits(i,z)
      Visited visited{};
      auto result = best_digits(13,snippets,0,visited); // Get the best digits 13..0 for z=0 so far 
      if (result) std::cout << "\nbest monad: " << result.value();
      else std::cout << "\nFAILED - no monad found";
    }
  }
  Result solve_for(std::string const& sData, std::string sIn) {
    std::cout << "\nsolve_for in: " << sIn;
    Result result{};
    std::stringstream in{ sData };
    auto program = parse(in);
    if (sIn.size()==0) {
      bool done{false};
      size_t call_count{0};
      do {
        // Puzzle program
        // std::cout << "\n"
        // for (char digit : s_result) {
        //   sIn += " ";
        //   sIn += digit;
        // }
        // std::istringstream alu_in{sIn};
        // ALU alu{alu_in};
        std::cout << "\nNOMAD>";
        ALU alu{std::cin};
        alu.execute(program);
        std::cout << "\n" << alu.env_dump();
        int z = alu.environment()['z'];
        done = (z == 0);
        call_count++;
      } while (!done);
    }
    else {
      std::istringstream alu_in{sIn};
      ALU alu{alu_in};
      alu.execute(program);
    }
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
  // answers.push_back({"Part 1 Test 0",part1::solve_for(pData[0],"-17")});
  // answers.push_back({"Part 1 Test 1",part1::solve_for(pData[1],"3 9")});
  // answers.push_back({"Part 1 Test 2",part1::solve_for(pData[2],"10")});

  // I interpreted the rpogram and...
  // Each digit is processed by the program so that z(i+1) = 0 for w(i) = z(i)%26 + N(i)
  // So if we identify N(i) in the program for each step and enters them for input
  // we actually get a z=0. They are: 11 13 11 10 -3 -4 12 -8 -3 -12 14 -6 11 -12
  // BUT - This is of course invalid as the actual input must be digits 1..9
  // ==> Can we use this knowledge somehow though?
  // answers.push_back({"Part 1 Test 3",part1::solve_for(pData[3],"11 13 11 10 -3 -4 12 -8 -3 -12 14 -6 11 -12")});
  part1::investigate(pData[3]);
  // answers.push_back({"Part 1 Test 3",part1::solve_for(pData[3],"")});
  // answers.push_back({"Part 1       ",part1::solve_for(pData[3],"1 3 5 7 9 2 4 6 8 9 9 9 9 9")});
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
