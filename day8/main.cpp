//
//  main.cpp
//  day8
//
//  Created by Kjell-Olov Högdal on 2021-12-08.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <numeric>
#include <map>
#include <algorithm>

char const* pNaiveTest = R"(abcefg cf acdeg acdfg bcdf abdfg abdefg acf abcdefg abcdfg |
cf acdeg acdfg bcdf)"; // results in display 1234
char const* pSingleTest = R"(acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf)";
extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;
using Segments = std::string;
using UniquePatterns = std::array<Segments,10>;
using DigitPatterns = std::array<Segments,4>;
struct Entry {
  UniquePatterns up{};
  DigitPatterns dp{};
};
using Entries = std::vector<Entry>;


Entries parse(std::istream& in) {
  Entries result{};
  std::string sToken{};
  Entry entry{};
  while (std::getline(in, sToken)) {
    // std::cout << "\nsToken:" << sToken;
    std::string left{};
    std::string right{};
    if (sToken[sToken.size()-1] == '|') {
      left = sToken.substr(0,sToken.size()-1);
      right = "";
    }
    else if (auto pos = sToken.find('|'); pos != std::string::npos) {
      left = sToken.substr(0,pos-1);
      right = sToken.substr(pos+1);
    }
    else {
      right = sToken;
    }
    
    if (left.size() > 0) {
      // std::cout << "\nleft:\"" << left << "\"";
      std::stringstream sl{left};
      std::string s{};
      int i{0};
      while (sl >> s) {
        std::sort(s.begin(), s.end());
        entry.up[i++] = s;
        // std::cout << "\ndp[" << i << "]:" << s;
      }
    }
    if (right.size() > 0) {
      // std::cout << "\nright:\"" << left << "\"";
      std::stringstream sr{right};
      for (int i=0; i<4;i++) {
        std::string s{};
        sr>>s;
        std::sort(s.begin(), s.end());
        entry.dp[i] = s;
        // std::cout << "\ndp[" << i << "]:" << s;
      }
      result.push_back(entry);
      entry = {};
    }
  }
  return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
    std::stringstream in{pData};
    auto e = parse(in);
    auto count = std::accumulate(e.begin(),e.end(),size_t{0},[](auto acc,auto const& e){
      auto count = std::accumulate(e.dp.begin(), e.dp.end(), size_t{0},[](auto acc,auto const& e){
        auto size = e.size();
        if (size==2 or size==3 or size==4 or size==7) ++acc;
        return acc;
      });
      acc += count;
      return acc;
    });
    return count;
  }
}

namespace part2 {
  using ReWiring = std::map<char,char>;

  const std::map<std::string,char> SEGMENTS_TO_DIGIT{
  /*
   0:      1:      2:      3:      4:
  aaaa    ....    aaaa    aaaa    ....
 b    c  .    c  .    c  .    c  b    c
 b    c  .    c  .    c  .    c  b    c
  ....    ....    dddd    dddd    dddd
 e    f  .    f  e    .  .    f  .    f
 e    f  .    f  e    .  .    f  .    f
  gggg    ....    gggg    gggg    ....

   5:      6:      7:      8:      9:
  aaaa    aaaa    aaaa    aaaa    aaaa
 b    .  b    .  .    c  b    c  b    c
 b    .  b    .  .    c  b    c  b    c
  dddd    dddd    ....    dddd    dddd
 .    f  e    f  .    f  e    f  .    f
 .    f  e    f  .    f  e    f  .    f
  gggg    gggg    ....    gggg    gggg
   */
   {"abcefg",'0'}
  ,{"cf",'1'}
  ,{"acdeg",'2'}
  ,{"acdfg",'3'}
  ,{"bcdf",'4'}
  ,{"abdfg",'5'}
  ,{"abdefg",'6'}
  ,{"acf",'7'}
  ,{"abcdefg",'8'}
  ,{"abcdfg",'9'}};

  ReWiring init_rewiring() {
    ReWiring result{};
    for (int i = 0; i<8;i++) {
      result['a'+i] = ('a'+i);
    }
    return result;
  }

  std::string rewired(std::string const& pattern,auto const& rewiring) {
    std::string result{pattern};
    for (int i=0;i< pattern.size();i++) {
      result[i] = rewiring.at(pattern[i]);
//      std::cout << "\nrewired " << pattern[i];
//      std::cout << " to " << result[i];
    }
    return result;
  }

  char digit_of_pattern(std::string const& pattern,auto const& rewiring) {
    char result{'?'};
    auto rewired_pattern = rewired(pattern,rewiring);
    std::sort(rewired_pattern.begin(), rewired_pattern.end());
    if (SEGMENTS_TO_DIGIT.contains(rewired_pattern)) {
      result = SEGMENTS_TO_DIGIT.at(rewired_pattern);
      // std::cout << "\n\n\n\tdigit_of_pattern " << pattern << " is " << result;
    }
    return result;
  }
  bool is_valid_digit(auto const& pattern,auto const& rewiring) {
    bool result{false};
    // is valid digit is rewiring of pattern lights up a digit
    auto digit = digit_of_pattern(pattern, rewiring);
    result = (digit>='0' and digit<='9');
    return result;
  }
    
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{pData};
    auto entries = parse(in);
    /*
     Find out how to permutate the 10 unique patterns so that they map to
     correct digits '0'...'9' on the display.
     
     Then translate the 4 digit drive signals to the correct ones and
     interpret the displayed number.
     */
    /*
    {
      // Test
      for (auto const& entry : SEGMENTS_TO_DIGIT) {
        auto digit = digit_of_pattern(entry.first,rewiring);
        std::cout << "\ndigit_of_pattern " << entry.first << " is " << digit;
      }
    }
     */
    for (auto const& entry : entries ) {
      // Brute force - test all rewiring permutations until we get a valid display output
      // std::cout << "\nentry:";
      std::string displayed_number{"????"};
      size_t loop_count{0};
      ReWiring rewiring{init_rewiring()};
      auto rewiring_pattern = rewired("abcdefg",rewiring);
      bool invalid_rewiring{true};
      while (invalid_rewiring) {
        // try a rewiring permutation
//        if (++loop_count%1 == 0) {
//          std::cout << "\n\tTry #" << loop_count << " with rewiring " << rewiring_pattern;
//        }
        // We should get a valid digit for all 10 unique patterns?
//        std::cout << "\ntransform: ";
        std::string unique_digits{"??????????"};
        std::transform(entry.up.begin(),entry.up.end(),unique_digits.begin(),[&rewiring](auto const& p){
//          std::cout << " " << p;
          return digit_of_pattern(p, rewiring);
        });
//        std::cout << " | ";
        // we must get a valdid digit for all 4 digit patterns (brute force)
        std::transform(entry.dp.begin(), entry.dp.end(), displayed_number.begin(), [&rewiring](auto const& p){
//          std::cout << " " << p;
          return digit_of_pattern(p, rewiring);
        });
//        std::cout << "\ntransformed: " << unique_digits << " | " << displayed_number;
        invalid_rewiring = (unique_digits.find('?') != std::string::npos); // found '?' = in unique digits transform
        // Try next permutation of rewiring
        auto try_next_permutation = std::next_permutation(rewiring_pattern.begin(), rewiring_pattern.end());
        if (try_next_permutation) {
          // One more permuation to try
          for (int i = 0;i<7;i++) {
            rewiring['a'+i] = rewiring_pattern[i];
            // std::cout << "\n\t" << static_cast<char>('a'+i) << " -> " << rewiring_pattern[i];
          }
        }
        else {
          std::cout << "\nfailed: Out of permutations before valid display number!";
          std::cout << "\nTry count = " << loop_count;
          break; // out of permutations
        }
      };
//      std::cout << "\nsignals ";
//      for (auto const& pattern : entry.dp) {
//        std::cout << " " << pattern;
//      }
//      std::cout << " is number " << displayed_number;

      result += std::stoi(displayed_number);
    }
    return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Naive Test",part2::solve_for(pNaiveTest)});
  answers.push_back({"Part 2 First Entry Test",part2::solve_for(pSingleTest)});
  answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb |
fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec |
fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef |
cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega |
efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga |
gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf |
gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf |
cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd |
ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg |
gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc |
fgae cfgab fg bagce)";
char const* pData = R"(ecdbfag deacfb acdgb cdg acdbf gdfb efacdg gd cagdbf beacg | cdg dcebgaf gbdf bdacg
fadecg gdbecaf agbfd fgdcb gab ebagdf feabcg deab gdefa ab | adfbg ab fcgdbae bfgecda
cgebad edfagcb fg fedg ebfca gcefb fcedgb dbagcf cgf cdbeg | cfg acfbe bcgdafe dgeafcb
bgcde cbefg gd dbeafc afbcgde bedgca gacd dbg cedba fbegda | agfcebd adgfbce dgb fgceb
gadcbe gcade debfac fdagce egdbf cfedg fbgcade gafc dcf fc | fcga eacfdg gfca fdcbea
dbgcae gdeaf cefga cfa dcbgfa cgfabe cefb cf ebfgcda acgbe | gebacdf fcgdeab bacdge cfeb
fgde efc dacgf cbdgfa fe abdcfe afdbecg gaefdc gcfae abceg | ef agdcbf bfdagec efdg
egfacd bfcdeg ac facbg acbe cfa fbgace fgecb gfdba fbdcgae | ac ca ca fgeacb
acefg dae dfbec abfcedg cfdea dgeafc ad dfag eacgbd bcagfe | dfag gfad dgfa beagfcd
gae cabfgd fbcag ecbgad gfec ge agbfe gdefbca bfeda cfbgea | geabf gecf efgc gafbc
efdgabc bda gceadb gafbec aedbf da cadf cbaef badfec gdfbe | efbda dba adcf da
dgbaec gcade feacd bcfgae fabcd edfg ef agdfcbe gdefca fea | gedf aegfcdb edagc cefdgba
cf adbfc bfagd gdaefc cgdefab gfbc gdacbf caf eacdb fdeagb | fgcb faedcg cbfg gcafbde
bg cabg cdfge efbacg cebadf dgeafb fcebg gbe cabef egdbfac | gb bge cgba ecbfa
dgfcabe geadfc gbadf cgd gbedcf acdgf dfbcea agce cg dfeac | cg agcfd aegc cdg
gcfdae bcgde bdf gfcaedb bafc abfdcg agdfc cbgdf bf beafdg | fbd bafc cafb dbfcga
fgab fbedca fgead fg bfgead efadb gefcdb defgcab fdg gadec | cbedfg abgf agfebd gabfcde
begda fgbdeac gecf cgd dgeac efadc fbecda cegadf cg fabgcd | adgfbec cgd gdc dgfcaeb
gcbfe gacdef eadbgc eabcf cbg dgfcbe bg dfeagcb dfgce bfdg | gbc bfgd bcg bafce
efcbd ecabgd cbdga cfaedbg fcabdg deg ebga eg gdceb aegfcd | acgdbe ge aecdfgb fagdec
gdeba afb cgdfab gfacebd gafe fa fbedc dfbega dbecag ebdaf | cagfdb eacdbgf bdefagc ecdfb
fcbgdae egfbc fdbac dgcaef fbcdag fgcbd aecfdb dcg bgda gd | dbag dg afcegd cgd
dgcea agbedcf ed agcef acgbd dagfbc gbed ecbgad dae bdaefc | begd ed dea ead
efdbac bedcgfa agcde cdgfea cg bgaed dcg efgc faced gdcfab | dafbec fdacgbe fgdace gadeb
bdgae acdegb adgfbe agc gbcaedf gc agdfec abecg ecfab cbgd | aebgd cg cg gdbc
abgcef ebgcf badgef cfgdbae cfagb eb gbe cfedg dbafgc baec | beac agbcf be ebg
dca cabedg acbf aedcf agcdebf daegf gefbcd efdcba ca debfc | cfba agcbefd ac cbeagd
bf gfbeca cbaef adfgbc bfc gbef aegbc gcafebd bagced acdef | fcb gefb bfeac ebgfac
gcbaefd fg cfagbd bafdc bceafg gfbcd fgb dafg dbafce ebgcd | fg gf adgf gafd
gadbf aefbg egfd efb dgfbea cbfeda cbega ef agbefcd gdcabf | gfde ef gdfe gbfae
egbdfc gecbfa dfgea egcfb bea ba bcag fabcde gacfdeb gafbe | agbc bdcagef ba dfgea
gfdce afged dcgbef adcgbef dbage agf gcfdae gbadfc cefa fa | dfcgab bdgae gaf gaf
fadgec bdaegc fedag cd bfdaeg dac agbfdce efcd cgbaf dfacg | bdgcaef gfedca cda abcgf
afebgc eac agfcb fdbce fbcdag faegbdc cafegd gabe cbeaf ea | ae eca gadefc cfdbega
gcdb db ebgacfd deb gedca efacgd cabed ebcfa decabg eabdgf | dgecfba fabdecg aecdgfb fegcbad
cgefd gd edcfbg dfbgca fdg cdabef fadcgeb dbge bdcfe caegf | dgbfce gbde degacfb edbcf
eadgb fegbd faebcg bdecga agb cbad cbdaefg aefgdc ba dgcea | egabcdf ba agb deagbcf
cedga ab fdgeba gfdeb fbga eafdgbc dbgae dcgfeb dab ebcfad | agfb dbcfage ab gfba
efdacb fabegc dbgfcae gce ceagf gcba gc gebfdc efadg afcbe | agbc dabfce fgcbde dfega
bfdcga debgf fde gfeadb de dabfce fbadg ebcgf dega bfaegdc | cefbdga dega dgbfa cgfeb
efagbcd gafbed ab gcdfab cgbde cabf cabdg gafdc bda gaefcd | cfba adb egdcb bad
fcbe fcagde bc fcbgd fdgbec fdgab degcf bcadfeg gdeacb cbg | fbagcde efdbcag gdfba abgfdce
bdfac cedfa cabdgf bdgafe cdfbega dcabge fba fgbc bf adcbg | gabcfd afcdb beadcgf eadcgb
cb edcfa aedbc dgbfea bdc cgeb adgfbec edagb cfbagd cgadbe | bdc cedba bfgceda bc
fdbca badgec gbf cgaeb gf fcebgda bfgac dafbge cafbge gcef | baegc gfb fbg gbf
dbafce daecb bgdca edbgfa abg egcdab ecga cgdbeaf dcbgf ga | bag ga cage cfbgd
eacfbgd cbagdf dbegcf edac fgceda gaefc cgfed gbefa ca fca | faebg bagfe gfabced acf
fde edbca cdgfba ef gadfebc bfagd efdba bfge efabgd edgcfa | fegb bacgefd gbef bgdafc
bcgdf gbdeca ebcgfa fcgaedb agdfb dfce cfg fc ebdfgc cedgb | cfg cf cgf cedgfb
bfega cafdbe adfcg dfabgec fdb db dagbf gaefbc afebdg bged | bfd gdeb gbed bfdagec
facgbe gbdefc egfcd ceadg efdb cef dbfcga bdgcf ef fgdaecb | bdfegc fe edfb cef
bfad dagbc cbd fgedbc dgcea degbafc dgbacf agbfc bd cgfaeb | db dcgafbe dacgbf dfba
cdbae acfe gcfadeb ead ae degafb bfcdga beadfc bcdge acbfd | fdebcag dfbac ae ea
bdgfac ga gfae cga bceadfg agdec fcead bgdce afdebc eadgfc | cga ga faeg facdbeg
af afb bcdae ecbfg feabc cdfa bgedac egdafb fcadbe eacbdfg | cabef afb gecfb cagedb
abgcde fbd bdeaf df cdeagbf dbgfae facbe dbage cefbdg gafd | dgbacef df febdcg df
egdbaf bcafg gedab fe efg ecafgdb bcgdae gacfde bgaef ebdf | bfedga ef cfgaedb bdaeg
bfe ef fgebadc deagb feadb caef cegdfb cfbad ebacdf cdfabg | cdabf fcedba dcgbaf afgbecd
dec gacfd geac ebgfd ce fdegcab edacfg dfceab bdgafc edfgc | cega geac dce ceag
cbfdge cebag efa febgda gbdef fa dfag agfbe fdaecb befdacg | febga baegcfd aefbcd agdf
fgdcba bdcaef gaf bcga dgfac ga gdcef cfdab gfdeba gfbceda | cabfd ga gfa cgfad
gdaef fabdceg fgdcba gcd acbfd cbfg cgdeab dfgca cdefab cg | begcad fgcb fegda dacbfg
efbdg gfad bfadgce deafb da fbaec ead debcfg bdafge ecdbag | acgdfbe dea eda bafgdce
gcb efcadb cgfdb bg dbag gbfdeac cagbdf bcadf egfdc abecgf | cagfbd dfacgbe bg bgda
ab fedba ceagdbf dgbef afcde bdca cgeabf afbecd aeb aegcdf | cbda egafcb abe dfcea
acgbde bcag fcbged bec dfeab ebadc bc ceagbdf gdaecf aegdc | abcde gcba ebc ecb
bacgfed ecafgb gefbc agfedc fbeag fegdba fcg dcgbe fc bcaf | gebaf facb gbefca cegbfda
dfebgca daceg beadgc bcgda gebd bag fgcade gcfeab gb cadfb | dbge gdeac bag acdgb
cefg bfcda eacgbd bdfcg cgedfb geafdb cg cdg fegbd fceagbd | fcdgb gcdbf cdbfg cg
febc cgdaebf gedbf bf fgade fdcbeg bdagcf bgcade cgbed gfb | gfdbe fb cbegd ebdfg
ed bacdge dafgbce gaedb cbgafe fabgd deb cbgae gdec efbdca | deb ed degba acbgdef
gcfdba gbadf cb dbgcae cefda bgadecf efbagd abc fcgb afcbd | cb bfcg cdeabg cbfg
bfcedg bcedaf bfecgad efbad aefc ce degbaf bce aecbd cgbda | bafdce acefdbg dbfecg cbe
bf cdbef bef gcfb ecgadbf debfcg bgadef cbdea afecgd efcgd | gfedcba gedfbc edbca efgbda
becgaf fbedcg dgbef defcb gbf begda cfdgeba dfbace dgcf fg | degab fgcd gbade gdfeb
fbecdag edafc ebdfc aecb fabcgd bfc bc ebdacf cgeafd bgfde | bc fcb cb eabc
bdegfa daefb dgbacf dcfagbe cdeab fgeab efdg gfcbae fd adf | afd eabcfdg adf df
efcabd gdfec bgdcae acegb db cegdb dbag deb bfgace fadbceg | deb bed gacebd gcedb
adb bdeagfc adgce cfdbe fbae ebacd cagfdb bdcgef ba adbcef | dab edfcagb ab aebf
caed cga dgceb edfbcag gcaeb bgaef ca gbecad gdcbaf cedfgb | cdebfg eacgb gac dcegb
cdbeag bagfc aef dbeagfc fabdec bedf gcfeda febac fe baced | efbd afe acdegb cadebf
aecfd cbedf bd cbafgd ecgfdab dbcgef gebd aebgfc bcfge dbc | dbc dbc fbgaec dbc
cfdgae fgacd ga dbcfa bgfced dag gfedc daecbg gafe ecgafbd | dgfca cfadg acefdbg ag
degba gecbad gbcfead deafgb gafedc gec gbdc ebcag cg cfeba | gce afecb gce gce
debf gcfdb ecd dgbcaf eabcg ed dgfcbe agcdfe bgedc gbcafed | cgdfaeb gcbfd dacbfg fbed
bg gbed abceg cegfa fgaebcd cbgadf bga decbaf gedcab acdeb | cadebf gba abg abcegd
cafbdg bfaeg fgedab bgafecd gab befcg adbe ecagdf edfag ba | beda afebcdg eadb dfecagb
fdebc ge edbcaf fbdagec dfebg cegd gcfdeb gbfaec dbgaf beg | ebg bge ge dceg
becad gbcdafe egcdfb eafcb degcb fbcgda agde adb badgec ad | facbgd afceb fbeca badgfc
gdfac fadgbec cef ec bdgfe agfceb fcdeg gefcbd defgab edcb | abgedf eadcbfg dbgecfa fgebdc
ebfcdg cgdbaf gdbfc bdec debfg febga edfgacb def facgde de | gfcbad cebd ebcd fdgcb
feg faedc afdebg ge bgcafe gdcabf edgb gdfea dfbag efdbacg | gedb eagdf egbd gfbad
eagdc gabedc cebdf gfc gadbfc edfcbga fcegd gdefca efag gf | fg fcg adegc edgfc
fde fgedc cbdafe agefcb fagdce gfda efacg gbcde fgdecab fd | eafgcb cgfdeab cgfae efd
afdegc dec dc ebcfad bceagf ecfag becdgfa gbdea gaedc gdfc | ced acbgedf gabde faebcd
efdacgb cdagf cdegb egcdfb bf dcfbg cbfe gebcda bfadeg fbg | cbgdae gbf bf ebcf
bdcfe cfdbga dec eabgdc caebfd ce fagcbde efac bdacf ebgdf | fadbgec cde aecf cde
acgfdbe bceda bdgac fgcabe cebfa dbe eafd fbdceg dbfeac de | acfeb cagbfe feda aedf
cd adgcfb adgc dfabce fdbgace gbdfea dfabg dfgbc dcb gcbfe | bcd badfecg cd gfeabdc
bedc cgbdfa ec cge efdagc dgacb acgdeb agfbe bcage baecgdf | gce cedb ce cge
fagecd gdfb ecgab fcbgade fcdge bdgce eadbfc gbecdf bd bde | gdecfab cedgf gbace abcfde
debf fbc fcgeab dfgec dgacfe debgcf dgcfb bf cbdga aebdfcg | cdgefa adcefg dfbe agdefbc
bcafe bdga debgacf ecafgd ad ecbgdf bfcgd bgfdca cad bcdfa | aefgcbd acdegf eagfdc ad
fbdce de fcbdg bfecad cfaeb adegcb facedgb bde faed bfaegc | cfdbg afde fadbec bde
fcdgbea cfe cebgf fe fbcdg abcge dbfe gfdbce dgcafb fdgace | eafdgc fec fce cef
fagdebc fcbdg ba adecbf dgebcf cbga edagf bad agfdb gfcdba | fgbcd ab dcfbeag ba
efgadc gbeac fadebg gbaef gec dabce cgfaeb bgfc gc fdacegb | begdfa gc gc fcbg
ec fgbde befgda caegfbd agbcd bdcgef egc bcfe dcgeb edcfga | gecadf gadbcef ebgcd dgefb
cfadgbe cdef ebagdc cadfb cfbade afbde deb gdcbfa aebgf ed | dbe decf ed ed
ef fcae gdbeac bcedgf def ebfacd edbac eacbfgd bfdae dbagf | eabfcdg dfe edf bcdefa
bafgd dc gdebacf decg fagedc cad gcdaf egacfb aecgf aedfcb | ecgd bgafec acd bcedaf
fcegb dafcgb bgd bagcdfe gfebd fdbae eagfbd gd edga aedcfb | dfgbea gdb bgd fbgacd
dec cgebaf fdgc afedbc dgbce cd fgbdce bcgfe egdba ebafdgc | bacedfg gbacef dgcf acbfed
edfgab bgeaf eacbfd ebcagd dafbegc afe gfde bgfca fe eabdg | ef afe cfdeab abegd
ae egbaf eacg gdfecb bacdef efbcg adcfbge fae dfabg ecfagb | agbef cbefag eaf adfcbe
gce edfgbac dgefac fcega adcebg fagcd fgdbca aecfb ge fgde | degf egfac begafdc eg
ebacd gdace bc bdefa gdefac eafgdbc ecgb dcb ecbadg dcbafg | dbc ceagdb gbdacf cdb
aecdb bfae gcefd af cdafe fda eadfbc gabcdf begadc cabdgef | edcgfba acgbdfe cadbefg afd
cad cefadg geacbfd debag cbag egbdca ca cdabe eagbdf ecbdf | ca cda acd cgab
cbf adcgb dbfa bf fdeabgc eacdbg fabcg fgaec ebcfgd fdagbc | gcdfbe cgafe bfc agdcb
dfeagc dacgf fdcea bdgacf defbga aefcb dgce ed dae adfbcge | cedg afegbd gfbceda eabdgcf
egacd ebdgfc fc cfg dcfgea cdfa dgaecb efbag aegcf bgecdaf | cgf fc cbdega badefcg
gedb fgabc adfbec bfedag dbafg dfcgabe gfd adgefc gd dfabe | adgfbe bfcgade gdf fdg
deg fcgbdea gdbacf cgbade bfgcd fcaed fdgbce fgeb edcfg ge | ge gfeb gbcdfa gebf
cdfag ebfdgac ecga gfade bfdeg gfacbd gdefca ea afe adfecb | ecga abcgedf dcfabge efa
gadebcf aebg bdgac fdbac dcage bg acedbg dbg efgdcb egfdca | dgb bage ecfgadb gbd
ecfd efgcb dcgbf fcbdga afdgcbe gfe bgedfa dcbgfe cgbae fe | fge ebfcdag bgeca cdef
dfbgca dfbac ebgfdca abefd ebdgac fcgbd ca cab cfag fedgbc | cgaf bedcgaf gbcfd bac
begfac faedc fedcb fecdga dcbgefa fegca adc dega cbafdg ad | da adc ad cda
cebgdf fegdb ea efgabdc gcbda bdgea ebgdaf dcagfe bfae ead | ea ade cegdfab bfgaecd
aefcbdg dbafg agdeb agfcbd fa adf efcabd gbefcd gfac cgdbf | fa adf af gdbcf
cdgabef bfcaed cdf gcdbf bdgac fc dcebag gcaf bfged afdgbc | cf cgadeb fcd abdfgc
fbcea egfcad dcbf adbef befgca dfa cagfdbe feacbd ebadg df | cfbd dgecbaf df decfba
ebdcg ecbag egbcfd dgc cdefb dbfg dg bacefgd dcefba fgdcae | fbedc fedgac fecadb dg
gcebd gf dafg acegbf fcadb eacbdfg cgf dbagfc dcbfg defabc | dagf agdf egacdfb dfbcga
ebafgd cafdg gbfad bf eacbfd bfa edfbcag abgde cagdbe bfge | cgabed fadbg bcdfea bf
ebdca dgbef facebg fba acdf bdafce daebf cgdebfa gadbce af | eafcgb egcdba gebacfd dcgbefa
gafcdb dfcbe ae bacdg ecfagdb fcageb agde baced deabgc bea | cdgab cdfagbe ae acbde
ecgdf acdg adfegb edc cgdeaf bedgafc dfgae cd bfceg afbedc | cebdaf dce acgd aegfd
ab gdcebf bga ecgbf bdgafce gfbac afdgc cegadb gbefac eafb | fcgad eabf afbe ab
bgefca fd bdegaf fdb debfg agefb gdbcaf afde dgbce dfbeagc | cafgbe fdb dfb dbafgce
febcadg ecabg agdbec dafegb cegbfa baf bfgc bf febac dafce | gfcb fcbg efcad cefabdg
bcgaf bgfcda acbdfge ad bagd dca gfacde bdcef ebfgac abcfd | ad ad agfdec cad
gaefdc dbgfae bd fbd gdefbc cbdef cebfa cdfeg ebcfdag bdgc | bedfc bd gdcfe db
dbgeac fdcgbe eg gace cbaed adgbe gecdbfa gbe fgbda dfaecb | ebg gadeb aegc eabfgdc
bcgde cfgdbe gefc bfg fcdbg fdegab dfcba gf adbgecf bdgeca | bdcge efbdga cegabfd gfb
fcgeb ced cd begcd dbgcae dabeg gdefac gabedf bgefdca bcda | gfbaced aebgd dcab acbfged
ebadg fdbecg cgbfe fabc agf efabgc fa dfaecgb bfgea dagcfe | bcfa fag af cfadeg
ga cbegd fgae fdgaec bfacgd cgade gfecdba adcfe fdbeca gac | acg gacfdeb gefa fedbac
gbe debcf cgdfeab bdegfa agec ecdagb acfbgd ge gcdeb gcdba | egb gbadfec cgae cage
cdfg fgdebca gadbfe cg egfac adgfe egc fbace dcbaeg dfcaeg | ecg gdeaf cgfd gdcf
fagecdb fed gedac cfgd aefcb df gbaedf fcade dgafce geabdc | bfdcega df fd agbedf
eaf af gdeac gfeadb egdbf bafd bdgceaf afegd gefbdc eacgbf | dfab fea fae edgca
ea bgaec fbcge bdagc bcfdeg afdbeg abfegc eba cgbefda ecfa | ae ea bagcd ea
de abegdf dea fdbaceg gdcab dgaeb gfbae cbedaf egdf cbgaef | cebdgaf bfgcead ebcafg dea
cgaefb ca bagdcf caf badc fdgceab dgfea cgfbd cgfad cefdgb | ac bfdagec fca bcedgf
dbefcg bdgacf fegbd ebgc bgdfcea dgb gb aegfd bfdcea dcfbe | bgfdca fgbdca fcdeb bcge
gcadb dbage adbgfc deafbgc cbdf gcd dc eacfgb gdface gcfab | cdg cgbad gfdace gdc
dfc cadg bacfgde cd fagbc gbedf acedbf gfbdc cbefga cbafdg | cd gcdbf cd gbfde
adebf dbgae dfe adfcb gcbfeda ef bgfdae gadecf gfbe dbgcea | bgfe ebdfcga beadcg badcf
eafbgcd bfdcae egfac dbaeg cafegd bfcgae gcfb ebacg cab cb | aedfgbc bc ebdag bgcea
bdace abgdef dfc bdecf fc gbefcd cfegad fcgb degfb dgbafec | dfc dfc gcbf fgabecd
gbfdeca gca gcdb fgebac bcdfag bdface dcbfa agdcf gc afgde | cbadf fcgeab afgdc gbdc
daceg ceg eabg abefgcd ge gbcda gbcedf geabdc adecf fdagcb | fdbceag eg adceg gce
bd acdgb fdegca egadc edgfacb ceabgd gdafeb dgb cbde fcabg | cebd gbd ecbd ecdb
gdeca gcafbe gfecabd ecfd adbgc fdgea dfagbe ec cea gecfad | efgabdc dgabcef cea ce
adefcgb gbfd cbedf gf gefbdc cgfeb acbeg cfbeda fge fcgaed | dbcegf fge egdacf abgfdce
bfegca fdgb fdcga dga fbcga cfbgade dagbec gd caefd cdgbaf | dbceag dgfbeca dbfg cagdbfe
cbad cageb egbdaf cgdbaef da dea efgcd bgcade gaefbc dcage | dea ad ade gcdae
fgeab adecfgb beagcf debafc acbged fb cagbe gefad feb cbfg | badcge bfe fbe bgcf
cbfag bd fdaeg afdbg dba bagdec gbeafd abfegcd dfeb fadegc | abgdec gfbcdae bdgfaec fcgba
eabdgf fadbcg aedcfgb eg bge bagfce bgecf bfced fbgca cgae | ecfgb eagc eg ge
cgab bc abfdec cgefd adbeg ceb fabgde cadegb cbedg bgdeacf | gabc gacb cbag gefabd
bfecgd facdeg bcgfd fd fgcab bcegadf bdegc debagc dfg dbef | cgbdea dfg efbd gdf
bedf debcag abfcg afd acdbe fd dabfc deabcfg abedcf egfdac | efcgda bfde ebdf dabfec
gacbf gdebfc aedfb eg eacfdb egad febag aefgbdc egb gfabed | defba gbe eg eagd
fcdb cgadef bgfcad gabef eabdgc cgb gefbacd bc dagfc gbcaf | bgc bgacf cb dbfc
ebgfcad ebdga efdbac gefacb ge eafbd dgfe bagdfe gbe bgacd | bgedfa eadcgbf beg degf
gfabcd ba abedcfg bfae bca edafc bcade gcfeda efdacb egcdb | ba ba afbe dbgacf
ecdga deagcb gdbe ecgafd gabfced gfdcab gb cbg efcab ecgab | fagcbd dbagecf gbecad deacgf
fegbcd dg gfaedc egafc befgca eacdb edacg ged decbgaf agdf | fedacgb dge dg gecfad
cba abfeg gecabf cfdbae eacgbdf gfca ebdafg bcdeg ceagb ca | eabfg gcaf gafc acegb
dgefc fbgecd bafec da cdafeg fecad dca bfecadg dfga gcedba | egdcf aecdbfg aecfd acd
gdcf afcgbd cad gebda dfeacb fcabedg abgcd bfcag dc abefcg | cdgf cabfg gcfd adgbe
ef cafeb gbcfea agcbdf agbfc gfae bfe cgfbeda bdefcg dceab | eafg dgcbef cfgba bcagedf
eadgcf cadb ac cdbge bcaeg bcdfeg egabf ace fbgcead dbeacg | afgdecb egafcdb cdgefa ca
da bfacg bdfgace adfgbc badc fcdga gdaebf caebfg dga gecfd | fdbaecg ad cdgfaeb fbcdgea
cfd fc gfebacd bdcge cefgd afcged abdfcg acef adfge gdefab | bagfde gaebfd cf cf
dag bdfgac agbc fgbdc fdcgabe ga gbdfce gdafc ecfda bfdaeg | cbfdg bcag dag ga
gacfe aegfcdb fdgca dafcb cedgbf fgd ebacfg dega afdcge dg | gacfe gcebfd aedgbcf dg
cdafb cbfag ecfdba acgdefb daegbc fcgd dcafbg gbc gc aefbg | gbfea gbc cgfd cabefdg
bcdeg cfeabd fc afgc gecdf gdefab dgecaf efc bdaefcg egdaf | gbfaed efdcg bgecd dbagecf
cdeb cadgfb dagcb deacgf bgfae aebdfgc badeg eda aebgdc ed | ecbd fageb ebcd ed
cfde eacgfd gaefc agbdc gcdaf edgfba cebgaf gbcdefa gfd df | cfde fgd ecfd df
egfdb fgadc bgc cb cafdebg ecfgab cdab ceafgd bfgcda cfgdb | cabd ecdabgf adfgc egdcbaf
edcbagf fa gdcbea egcfd aecbd fadce adf ebafdc gbafed fabc | ebgdcfa daf acebd dcfge
bdfec cg dabegc cgbde fcegbad dcagef abcg cge dgfabe badeg | bcfed bacged gc cgadfbe
dagbf fbgdc fcbaed geaf ga ebadcg agd begafd cgedafb afbed | fgcdbea fdeab fdabe dga
fae dacfeg cade acdfbg ea dacefbg gcfabe cadfg degaf fbdge | bdafecg afdge fceadg fgbceda
becagdf gabfce gb bfdca agb dbgafe agfec egbc cegfda gcbfa | cgbe bga bdgeaf faegc)";
