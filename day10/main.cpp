#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <ranges>
#include <stack>
#include <numeric>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using Stack = std::stack<char>;

using Line = std::string;
using Lines = std::vector<Line>;

Lines parse(std::istream& in) {
  Lines result{};
  std::string entry{};
  while (in >> entry) {
    result.push_back(entry);
    // std::cout << "\nentry:" << entry;
  }
  return result;
}

bool is_open_char(char ch) {
  std::string const open_chars{"([{<"};
  return (open_chars.find(ch) != std::string::npos);
}

bool is_close_char(char ch) {
  std::string const close_chars{")]}>"};
  return (close_chars.find(ch) != std::string::npos);
}

bool are_matching_chars(char open_char,char close_char) {
  bool result{false};
  switch (open_char) {
    case '(': result = (close_char==')'); break;
    case '[': result = (close_char==']'); break;
    case '{': result = (close_char=='}'); break;
    case '<': result = (close_char=='>'); break;
  }
  return result; 
}

auto corrupted_char = [](Line const& line)->char { 
  char result{' '};
  Stack stack{};
  for (auto const& ch : line) {
    if (is_open_char(ch)) {
      // std::cout << "\nopen: " << ch;
      stack.push(ch);
    }
    else if (is_close_char((ch))) {
      // std::cout << "\nclose: " << ch;
      if (stack.size()==0) {
        std::cout << " ZERO ";
        result = ch;
        break;
      }
      else {
        auto open_char = stack.top();
        // std::cout << " top: " << open_char;
        if (are_matching_chars(open_char,ch)) {
          stack.pop();
        }
        else {
          result = ch;
          break;
        }
      }
    }
    else {
      std::cout << "\nERROR";
    } 
  }
  std::cout << "\nillegal:" << result;
  return result;
};

auto corrupted_line = [](Line const& line ) { 
  bool result{corrupted_char(line) != ' '};
  // std::cout << "\nline: " << line;
  // if (result) std::cout << " CORRUPTED";
  // else std::cout << " not corrupted";
  return result;
};

namespace part1 {
  auto score_acc = [](Result acc, char illegal_char){
    Result result{acc};
    switch (illegal_char) {
    // ): 3 points.
    // ]: 57 points.
    // }: 1197 points.
    // >: 25137 points.      
      case ')' : result += 3; break; 
      case ']' : result += 57; break; 
      case '}' : result += 1197; break; 
      case '>' : result += 25137; break; 
      default:
        std::cout << "\nERROR";
    }
    return result;
  };
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{pData};
    auto lines = parse(in);
    auto illegals = lines | std::views::filter(corrupted_line) | std::views::transform(corrupted_char);
    result = std::accumulate(illegals.begin(),illegals.end(),Result{},score_acc);
    return result;
  }
}

namespace part2 {
  auto incomplete_line = [](Line const& line){
    return (corrupted_line(line) == false);
  };
  char closing_char(char ch) {
    char result{' '};
    switch (ch) {
      case '(' : result = ')'; break;
      case '{' : result = '}'; break;
      case '[' : result = ']'; break;
      case '<' : result = '>'; break;
      default:
        std::cout << "\nERROR";
    }
    return result;
  }
  auto autocomplete = [](Line const& line) {
    Line result;
    Stack stack{};
    for (auto const& ch : line) {
      if (is_open_char(ch)) {
        stack.push(ch);
      }
      else if (is_close_char((ch))) {
        if (stack.size()==0) {
          break;
        }
        else {
          auto open_char = stack.top();
          if (are_matching_chars(open_char,ch)) {
            stack.pop();
          }
          else {
            break;
          }
        }
      }
      else {
        std::cout << "\nERROR";
      } 
    }
    while (stack.size()>0) {
      result.push_back(closing_char(stack.top()));
      stack.pop();
    }
    return result;
  };
  auto score_acc = [](Result acc, char ch){
    Result result{acc*5};
    switch (ch) {
      // ): 1 point.
      // ]: 2 points.
      // }: 3 points.
      // >: 4 points.
      case ')' : result += 1; break; 
      case ']' : result += 2; break; 
      case '}' : result += 3; break; 
      case '>' : result += 4; break; 
      default:
        std::cout << "\nERROR";
    }
    return result;
  };
  Result score(Line const& line) {
    Result result{};
    result = std::accumulate(line.begin(),line.end(),Result{},score_acc);
    return result;
  }
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{pData};
    auto lines = parse(in);
    auto scores_range = lines 
      | std::views::filter(incomplete_line)
      | std::views::transform(autocomplete)
      | std::views::transform(score);
    for (auto score : scores_range) {
      std::cout << "\nscore:" << score;
    }
    std::vector<Result> scores{};
    std::copy(scores_range.begin(),scores_range.end(),std::back_inserter(scores));
    std::sort(scores.begin(),scores.end());
    result = scores[scores.size()/2];
    return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\n";
  return 0;
}

char const* pTest = R"([({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]])";
char const* pData = R"(({<{({([<<{<[<<><>><<>>](<()()>)><{{{}<>}}<<<>{}>[<>[]]>>}{[<<[]{}>[<>()]>[(()[])]]<{{<><>}[[]<>]}{<
[(<[[([[(([<{{[]{}}([][])}<<{}[]>([]{})>><{<()()><[][]>}{[()]<()[]>}>]<{({()()}){[{}[]][<>()]}}[{[{}<>]
{{(<[[(<<<(<<[{}{}]{<>()}><{[]{}}([]{})>>([([][]){{}<>}][<()<>>[(){}]]))<{<(<>())[{}<>]>{{[]{}}<<>{}>}}({<
<([({[<[(([[<(()<>){[]<>}><<(){}>{()()}>]](<(({}[]){[][]})<{<>())[<>[]]>>{{{()<>}{(){}}}{[<>()]<{}[]>}
<(((<{<(<{{[[[()<>]{[]()}]][{<()[]><<>{}>}]}}[<{[{[][]}[<><>]]}{{{(){}}[<>()]}{[[]()]{<>[]}}}>{[({{}
{{({[({(({{[<[{}()]([]<>)><<{}()>[<>[]]>]<<{[]{}}([]<>)>{{<>}{<>{}}}}}{{<<[]<>>{<>()}>}<({[]<>}
(([{[(<(<[[(([()[]][{}()])(([][])<{}<>>))]]>)[<(<{{<<><>>(<>[])}<([]<>)[[]{}]>}((<{}[]>[<><>]){{[
{{[<<{<<(<([<<[]()>[[][]]>{[()[]]<()[]>}]){(<(<>{})([])>{<(){}>{<>()]}){[<[]()><<>()>](([][
((<{[{{(<<[(<{<>{}}[<>[]]>{<{}<>>{{}[]}})]>><{([<(<>())[{}()]>{<<>{}>{[]{}}}><([[]()]<[]<>>)(<[]>{()()})>)}>)
{<({{{(<[([<{(<>[])<()[]>}([[]]([]{}))>])[{{[([]){[]{}}]}}{[[{(){}}{<>{}}][[(){}]]]<([<>{}]{{}<>})(
([([{{{[[<<[[(()[])<<><>>]{({}[])}]((([]{})<<>>)({[]<>}[<>[]]))>{(({<>()})([{}[]]<()<>>)){[
{[<(({[<<{{{[({}[]){<>()}]{[()()]{[]()}}}[{<<><>>{<>[]}}<{()()}<()>>]}({<(<><>)[()[]]>([{}()])}{[(<>())(()
<{{[[<{{(<<{<[{}{}]<{}[]>>[{{}[]}[{}()]]}((<()()>{(){}})<((){})>)>[{<({})[[]{}]>([{}<>])}{{[{}()]}{<[
[{[<{<[[({([(<{}<>>[<>()])]<<<{}{}>({}<>)>>)<{(({}[])({}<>))}<{<[]>({}<>)}{{()()}([]())}>>}){<[<[[{}<>]<
{(<{{<{<<(<<<[{}{}]((){})>>{<<{}{}>{()[]}>}>)([({(<>())<<>()>>)(<<()[]>(<>())><<()()>({}())>)]{(<[<><
[{<{[<{([{[({<(){}>([]())}[<<>[]>([]())]){{<(){}>{{}[]}}<[()[]]({})]}](((<{}()>{{}()})))}[[(
<<[{{[<<(<{({{[]{}}{()()}}){<<{}()>({}<>)>(({}<>)[()<>])}}<[[([]{})[<><>]]]((<()()>{<>[]}){{[]{
[[({[({[([({{<[]{}>{(){}}}{[[][]][()()]}}[{<{}><{}()>}({()[]})]){{[[()<>]<[][]>][({}<>)]}}](<({<{}(
[[[((([((<{{[(<>())[{}[]]]{{<>{}}}}}>))<[[[({{{}{}}(()())})({([]{})[<><>]}[([]{}){()[]}])]<{<[{
[{[<[{{([[{(({()()}<{}[]>){{()<>}{{}()}})<<<{}[]>>{<<>[]><{}{}>}>}]{{{[{()()}[<>()}]{{{}}[(){}]}}}
[[{[(({[[[[{(<<>()>(<>()))[<{}[]>{[]<>}]}{{{()<>}<()()>}}]<[<([]<>}{<>[]}>]>](((<{()()}<()()>><<<>[]><<>
({[{({(<{((<<[()[]]<()<>>]<([][])>>[([{}()]<{}{}>){<{}{}>(()[])}])[{([<>()]{()[]}){[()[]]}}([{()<>}
[(({(([[{(<{{<[][]>(()()}}[({}[])]}[(<<>[]><()[]>)]>)<{({{[]{}}[()[]]}[<<>()>(()())])}{(((()[])<[]>)
<(<([[({[((((<{}[]>[{}{}]){[{}<>](<><>)})(([()<>]({})){[{}{}]{[]<>}}))[({(()[])<<>[]>}[[()<>]<()
{[(<<({([{[(<<[][]>(<>())>){<[<>()][[]{}]>(({}[])[(){}])}][(({{}<>}))(<[()()]<()()>>}]}{[{<(<>
[{{({<({<(<[[<{}<>>{()[]}][{()()}]]>){[<{{()[]}[{}()]}>([<()[]><{}<>>](<()[]>))]{[<[<><>]{<>[]}>[<()
{[<<[{<<[[(<(<[][]>[()()])>){<(([]()){{}[]})><<[{}()]<<>>>>}}[[([<{}[]><(){}>][({}())[()<>]])[[{<>[]
{((<[{{<{[<[((()<>)<()()>)]>]}>}((([{[[<<>[]>[[][]]]<[[]()]({}[])]]{[([]{}){()()}]}}<((<[]<
{[[({<{<<{<{{{[]{}}}}>}[[{(([][]){<>[]}){{()<>}}}<<<()[]>[{}[]]>>]<[{[()[]]({}{})}{[{}()]}]>]>>}>}<
[{[(<({{(<(([{()<>}{()<>}]{{[]}(()())]))>(<<{[<><>]<()<>>}[(<>{})<<>{}>]>[{{<>{}}{{}[]}}<[<
{{((<[(([<{{([()][(){}])({<>()}[()<>])}[{[{}<>]<()()>}<{{}}([]<>)>>}(<<[[]{}]<[]<>>>[([]()){[]{}}]>((<<>(
(<<<(<([<<({<<<><>>[{}<>]>{<()>([]<>)}}{{(<><>)}<{<>()}{{}()}>})<<<({}{})<[]<>>>>>]{({({[][]}[{}()])([<>()](
([(<<<(<(([([{<>{}}<(){}>](<{}<>>[{}{}]})][([<()>{{}[]}](<<>()><()[]>))[[{<>()}[<>[]]]{<<>[]>
(<{[[<<<({[<<([]())[()()]>([()[]]{<>{}})>]})[[{<[{<>()}({}{})><(()<>)([]{})>><<<<>()><<>()
[[[{(<[[((<[{<<>[]>({}[])}<[{}{}]{[]<>}>]<{({}{})}{[{}<>]{<><>}}>>([{[[]<>](()<>)}{{<>{}}(<>[]))]{[{{}{}}]})
[(<([[[{<[[<((()()){[][]})>((<{}[]>[()<>])<([]()){<>[]}>)]<<{<<><>>}{{(){}}}>>]<{[<<(){}>({}<>)><<<>()>([]<>
[<((<<[<{[{[{[()[]]{()<>}}(<<>()>[<>()])]}{<[[(){}]{(){}}]{[()]{<>[]}}>{{<[][]>[()()]}[(<>{
[({{([([{[[([<<>{}>[[]()]]({{}()}))][<{{()[]}}<<{}{}>>><[[{}[]][<>()]][{{}[]}(<>{}))>]][{((((){
((<<{(([[[{{<[()[]]({}())>}<[{(){}}]<({}{})<()[]>>>}{((<()<>>([]<>)){[()][()[]]})([{()}<[]{}>]<(<><>
<<<<[([<<(({<[[]()]<<><>>>({[][]})}{([<>{}]<<>()>){{[]{}}}})){[[[[[]{}][(){})]{(()[])(()<>)}]{{[[]<>][{}(
[({{([<{({{[{(()[])<<><>>}(((){})[<>()])>}{{{{{}<>}<{}[]>}{[(){}](()[])}}}}([(([<>{}](<>[]))
([(<({{(<<[([[<>][<>()]])<[{()[]}([]{})]{[()<>][[]{}]}>]><<{[{<><>}<()()>]{{<>[]}>}>>><[<{{
({{(([({{<[<[{{}{}}{{}<>}]((<>{})[[]<>])>(((<><>)<{}()>){{{}{}}{{}{}}})]({<{{}}([][])><<[]{}>
[(<{({{(<<<(<[{}[]]{(){}}>(([]()){<>[])))[{{[]{}}{()()}}{[<>{}][<>]}]>[((<{}<>>[<>[]])[({}[])<[]{}>])[<[{
[(<<<[<<({{{[[(){}]]{<<>{}>{()[]}}}{({<>{}}{[][]})}}{<([()[]]<{}<>>)([{}()]({}[]))>[(<<>()>{[]()})]}}{([{{(
{[{{(({[{<{{[[()<>]<[]<>>]({{}{}}[{}{}])}{(<<>[]>{[]})[[[]{}]{()<>}>}}{<<{{}{}}(()[])>>{{[[]()]
[<[[{(([<<[<[([][])<[]{}}]{<(){}>}>(<<()<>><[][]>><<{}[]>[{}()]>)]><([([[]<>](()[]))[(()<>)<(){}>]][({
<{{<([<([{{{(<[]{}>[[]{}])[{<>{}}([])]}}[<[<<><>>({}<>)]{((){})[[]<>]}><[{()[]}<<>{}>]{{()()
{[<<<([{([{<<(()[]){{}{}}>[(<>{}){{}()}]><[[{}<>][[]()]](<{}()>([]()))>}]){{<{<[()<>][<>()]><
<{[<<[([[{[({[[]<>](<>[])})({([]<>)<[]()>}<([]<>)[<>[]]>)]}[[{[{()<>}{<>{}}][<[]{}>(<>[])]}<({[]<>
[[[[([({(((<<[()<>]>[(()())]>{[{{}()}[[]()]]{[<>()]([][])}})))}(<({(({()[]}[<>()]))<[[()<>]]<{{}()}[()<>]
{[([([<[(<<[<[[]()]<[]<>>>][{<[][]>[<>()]}{({}<>)<()[]>}]><{({<>()}{<><>})<{(){}}({}{})>}<{(
<<({[<<{(<[<({{}[]}{<>[]})[<()()>]>(([<>()][<><>])(<<>{}><()[]>))](<[<{}<>>[{}[]]]({{}{}}(
[<<{<(([<[{(([()<>]({}{})){<<>[]>[()()]})({([])(<>())}[{[][]}[{}[]]])}{({(()())[()[]]}({[]<>}([][])))
((({<[{([(<[(<[]()>)](<[{}()]<[]{}>>(({}())<()[]>))>)[(<[([]<>)(<>{})]>)]][({({[()[]]{<>}}[[[]{}]{()[]}])([<<
[[[(({[<[((({([]){<>{}}})[[{()[]}[{}[]]][{[]<>}]])<<(<[]{}>[<>[]])[<[][]>[{}[]]]>>)[([{{<><>}[[][]]
({<(({{[{{<[((<>())(<>[]))<<<><>>((){})>]([<[][]><{}{}>][[{}()]<<>()>])>((<[[]()]([][])>[<{}[]>{
(({<<([[<[[{<[()()]<[][]}>[{(){}}[{}<>]]}(<{{}<>}{[]<>}>{<(){}>{<><>}})]<<{<<>[]>[<>[]]}{{
<[{([<[({<((<[{}()]([]<>))[(()[])])((({}{})<()<>>){(()<>)}))>{[<<[<>{}][(){}]>{<<><>>(<>[])}>{[
{[([[[{{<<[<<<()()>(()<>)>[({}{})[<>{}]]>([[[]<>]]{<<>>[[][]]})]><<[{(()[])][<()>[{}[]]]]<([{}{}][()<>])(
{(({{<<((<{<<<[]{}>{{}()}><<<>()><()[]>>><<[<>[]]](({}[])[[]{}])>}[[({<>()}{()}){[[]{}]({}())}
<[[{<[({<([<<<()<>>>>([[[]()]{<><>}](<<>[]>[<>]))])(({({[]{}}){{{}()}{(){}}}}[{(()<>)[[]()]}[{<>()}{[][]}]]))
{<[[([{<<<{{[<[]{}>[[]<>]]{({}())<[][]>}}<<<{}{}>>({(){}})>}([{({}())}<([]())[()()]>])>({{[[<>()]{{}{
<{(<[{[[{[<<<{(){}}[<>{}]>{{{}[]}<<>()>}>{[{[]()}<<>>]{<[]{}>[()()]}}>][[[{<<>())[[]<>]}<(<>
[{{<<{{([(<<{{{}}[()]}<(()())(<>())>>(<{[]()}[[]<>]>)>[((<()()><[]{}>}({[]<>}))(({[][]}{[]<>})<(()[])(()[]
([(<<[<{({[[[(()<>)<{}()>]{[{}()]{<>{}}}]{(<(){}>{{}[]}){<<><>><{}[]>}}]}<{[[[()()]{{}{}}]]
{<(((<({<((([<(){}>]){{{<><>}{{}()}}})({<[[]{}]>{({}())}}))>})([[<<<{[{}[]]{[]{}}}<{()<>}({}[])>>>{[
{([[{[[[<<<({{()<>}}(<[]<>>[<><>]))[[[[]<>][{}()]]]>{<<[<>()][[][]]](<{}{}>{[]{}})>[<(<>{}){<><>}>([{}[]]
[[{(<{{{<(<{[[{}()](()[])]}<<{(){}}[()[]]>{[()()]([]())}>>)>[(<<<{[][]}>>[{([][])[(){}]}[(<
<{[(<{<{(<<{<{()[]}(()[])>(([]<>)([]()))}>{((({}[]))<{[]()}{{}()>>)<{<(){}>}<{<>()}<()()>>
<(<[{{[[{<{[{[<>][<>[]]}]{[(<>[])][<{}{}>{<>}]}}(({[()<>]{[]<>}}<{(){}}[[]{}]>)([[<><>]<()()>]<({}[])([]())
{(<{[{[{({{(<<<>{}>><({}())(<>())>)}}{(<<<(){}>{(){}}>(<{}()>(<>()))><[{<>()}{{}()}][{[]<>}(()())]>){<[{{}[]
{[((((({<(([(<[]<>><[]<>>)<<{}()>>](<{()[]}(()<>)>{{{}{}}([]{})})))[<[{([])[<>]}(([]())<{}{}>]]{
(<{[((([{(<<{(()<>){{}[]}}>><<<({}<>)[[]()]>{(<>[])[()()]}>{{{<><>}([]())}}>)[{(([[]()][<>(
<[<<<[(<[[(<<([]{})>([[][]][{}{}])><[<<>[]>[{}[]]]<[<>][()()]>>)][[<<{()[]}(<><>)><<()()>[()<>]>>][{[{[]()
<<{([[<[[<[({[<>[]](()<>)}{({}[]){()[]}}){({{}[]}(<>()))}]<[[<[][]>[[]<>]]<[{}<>]<[]{}>>)>>]<
[((([[{<<[[<{{{}[]}[()<>]}>(<<{}{}>[(){}]>({<>[]}<{}<>>))][<{[{}<>]{[]}}>({[{}{}}[[][]]})]](({[{[][]}]{[{}
[([{<[[{<((<<{[]<>}(()<>)>>{(<<>{}>(()<>))}))[(<[[[][]][<>{}]]{[()[]](()())}><({{}{}}}[({}())]>
{{[({<(({{[({{[]()}<()<>>}{([]()){[]{}}}){[<<>>[<><>]][<{}<>><()[]>]}][{<({}{}><[][]>>{{{}()}<(){}>}}]}}))>}<
{[{[<{{({[({[[{}()][(){}]]{{[][]}<<>()>}}[[(<>[])<[]{}>]((<>[])[{}()])])]})}}>{({{<([{<([]())[{}{}]>{
<{{[{<{<<{{({<{}[]>[(){}]}[[<>()]{(){}}])}}[{{[(()<>)(<><>)]<{()<>}{<>()}>}{({[]()}{{}()})<{{
((((([{{<<[<({()<>}<<>{}))<{{}<>}(<>[])>>{<{(){}}[{}[]]>}]{<<[{}{}]>{{()()}[{}<>]}>}>>([[({(()())<()
(({{[[<[[<<<<<[]()>[{}<>]]>>({[<<><>><{}()>]{{(){}}[<>{}]}}[[([]<>)({}<>)]])>{<[[<{}[]><{}<>>][<<>[]>]](<<[]
<[<([<<[([[[[<{}[]><()>][(<>{}){[]}]]{({()[]}[<>{}])}]([((<>[])(<>[]))[{<>[]}{[]<>}]][(([]{})({}<>
<((<{{({{([<([<>{}](()()))<<(){}>[<>[]>>>({<<>>([]{})})]<[[([]())<<>>]{{<>{}}(()<>)}]>)<[(((<>)<{}<>>)({<>(
{({<({<<[[{[((<>())<()<>>)<(<>[])((){})>]({{[]()}[{}()]}[[{}{}]<{}[])])}({<[(){}](<><>)><{()<>}[{}[]
<[(<<[((([(([[[]<>][<><>]]{{{}{}}[{}[]]}){(({}())<[]{}>)})<(<(()[]){()()}>{<()>[[]{}]}){{[[]{}]([]<
{(<(([{[[<[[[{<>{}}{[]{}}]({(){}})]<[[()()]][[{}[]]]>]{<<{{}{}}[{}{}]>>[(<{}()>([]{}))(<(){}>{<><>}
((<<({({<{<<[<[]{}><()[]>]<(<>()){()[]}>>>}{(((({}<>)({}()))<{[]{}}<<>[]>>)<({[][]}<(){}>){
[[<{((({{{(<{[[]{}}{{}}}<{[]{}}<()>>>{<{{}[]}<[]()>>{[()[]]{[][]}}})([{[[][]]}(([]{})<<><>>)][(<[][]>{{}
(<({[{[(<<(((([]())[<><>]}([<><>]))<[[()<>]<<><>>]{<[]()>{()<>}}>)[<<{[]()}(())>><[<(){}><<>{}
(<<[<(<{<({<[{(){}}{<><>}][<<>()>]>}<[<[()[]]<{}[]>>]<<({}[]]><[[][]]<[]()>>>>)>}><[((<[<<[]<>>>[[<>()][{}(
[((<[{[[([{[[<<>>{<>()}]<[[]{}]<[]<>>>]}<({(()<>))<<<>()><<>[]>>)<{<[]{}>((){})}[({}[]){()()}]>
<<{{({{([{{<<{{}{}}(<>{})><{()()}[{}<>]>><{<[]<>>[[]()]}[<<><>>({})]>}(((<<>{}><{}{}>)([<>{}][{}]))[{)";
