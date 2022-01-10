#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <functional>
#include <numeric>

char const* pTest = R"(D2FE28)";
char const* pTest1 = R"(38006F45291200)";
char const* pTest2 = R"(EE00D40C823060)";

char const* pTest3 = R"(8A004A801A8002F478)"; // 16
char const* pTest4 = R"(620080001611562C8802118E34)"; // 12
char const* pTest5 = R"(C0015000016115A2E0802F182340)"; // 23
char const* pTest6 = R"(A0016C880162017C3686B18A3D4780)"; // 31

std::vector<std::string> part_2_test_data = {
  {"C200B40A82"} // finds the sum of 1 and 2, resulting in the value 3.
  ,{"04005AC33890"} // finds the product of 6 and 9, resulting in the value 54.
  ,{"880086C3E88112"} // finds the minimum of 7, 8, and 9, resulting in the value 7.
  ,{"CE00C43D881120"} // finds the maximum of 7, 8, and 9, resulting in the value 9.
  ,{"D8005AC2A8F0"} // produces 1, because 5 is less than 15.
  ,{"F600BC2D8F"} // produces 0, because 5 is not greater than 15.
  ,{"9C005AC2F8F0"} // produces 0, because 5 is not equal to 15.
  ,{"9C0141080250320F1802104A08"} // produces 1, because 1 + 3 = 2 * 2.
};

extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

using BitString = std::string;
using Model = BitString;

BitString to_bit_string(char hex_digit) {
  BitString result;
  std::bitset<4> bit_set{};
  switch (hex_digit) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      bit_set = std::bitset<4>{static_cast<size_t>(hex_digit-'0')};
      break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      bit_set = std::bitset<4>{static_cast<size_t>(hex_digit-'A' + 0x0A)};
      break;
  }
  result = bit_set.to_string();
  return result;
}

BitString read_bits(int read_count, std::istream& bin,int& acc) {
  BitString result;
  std::copy_n(std::istream_iterator<char>{bin},read_count,std::back_inserter(result));
  acc += read_count;
  return result;
}

using Bin = std::istream;

std::string indent{"\n  "};

struct EvalResult {
  std::vector<size_t> val;
  size_t version_acc{0};
  int bits_read{0};
};

EvalResult eval_packet(Bin& bin);
EvalResult eval_packets_size(Bin& bin,int bit_count);
EvalResult eval_packets_count(Bin& bin,int package_count);

EvalResult eval_packet(Bin& bin) {
  EvalResult result{};
  std::cout << indent << "<eval_packet>";
  indent += "  ";
  // Every packet begins with a standard header: the first three bits encode the packet version,
  // and the next three bits encode the packet type ID.
  auto packet_version_bs = read_bits(3,bin,result.bits_read);
  std::cout << indent << "packet version:" << packet_version_bs;
  result.version_acc += std::bitset<3>{packet_version_bs}.to_ullong();

  auto packet_type_ID_bs = read_bits(3,bin,result.bits_read);
  std::cout << indent << "packet type ID:" << packet_type_ID_bs;
  auto packet_type_ID = std::bitset<3>{packet_type_ID_bs}.to_ulong();

  std::function<size_t(std::vector<size_t>)> vals_eval = [](std::vector<size_t> const& vals){return 4711;};
  
  if (packet_type_ID ==4) {
    /*
    Packets with type ID 4 represent a literal value. Literal value packets encode a single binary number. 
    To do this, the binary number is padded with leading zeroes until its length is a multiple of four bits, 
    and then it is broken into groups of four bits. Each group is prefixed by a 1 bit except the last group, 
    which is prefixed by a 0 bit. These groups of five bits immediately follow the packet header.
     */
    indent += "  ";
    std::cout << indent << "<Literal Value>";
    bool not_last_package = true;
    BitString literal_bs{};
    while (not_last_package) {
      not_last_package = read_bits(1, bin,result.bits_read) == "1";
      std::copy_n(std::istream_iterator<char>{bin},4,std::back_inserter(literal_bs));
      result.bits_read += 4;
      std::cout << indent << "*" << literal_bs;
    }
    if (literal_bs.size()<=64) {
      auto literal_value = std::bitset<64>{literal_bs}.to_ullong();
      std::cout << indent << "= " << literal_value;
      vals_eval = [&literal_value](std::vector<size_t> const& vals) {return literal_value;};
    }
    else {
      std::cout << indent <<  "OVERFLOW ERROR";
    }
    indent.resize(indent.size()-2);
  }
  else {
    /*
     Every other type of packet (any packet with a type ID other than 4) represent an operator that performs some calculation on one or more sub-packets contained within.
     */
    /*
     Packets with type ID 0 are sum packets - their value is the sum of the values of their sub-packets. If they only have a single sub-packet, their value is the value of the sub-packet.
     Packets with type ID 1 are product packets - their value is the result of multiplying together the values of their sub-packets. If they only have a single sub-packet, their value is the value of the sub-packet.
     Packets with type ID 2 are minimum packets - their value is the minimum of the values of their sub-packets.
     Packets with type ID 3 are maximum packets - their value is the maximum of the values of their sub-packets.
     Packets with type ID 5 are greater than packets - their value is 1 if the value of the first sub-packet is greater than the value of the second sub-packet; otherwise, their value is 0. These packets always have exactly two sub-packets.
     Packets with type ID 6 are less than packets - their value is 1 if the value of the first sub-packet is less than the value of the second sub-packet; otherwise, their value is 0. These packets always have exactly two sub-packets.
     Packets with type ID 7 are equal to packets - their value is 1 if the value of the first sub-packet is equal to the value of the second sub-packet; otherwise, their value is 0. These packets always have exactly two sub-packets.
     */
    switch (packet_type_ID) {
      case 0:
        std::cout << indent << "operation SUM";
        vals_eval = [](std::vector<size_t> const& vals){
          return std::accumulate(vals.begin(),vals.end(),size_t{0});
        };
        break;
      case 1:
        std::cout << indent << "operation MULTIPLY";
        vals_eval = [](std::vector<size_t> const& vals){
          return std::accumulate(vals.begin(),vals.end(),size_t{1},std::multiplies{});
        };
        break;
      case 2:
        std::cout << indent << "operation MINIMUM";
        vals_eval = [](std::vector<size_t> const& vals){
          return *std::min_element(vals.begin(),vals.end());
        };
        break;
      case 3:
        std::cout << indent << "operation MAXIMUM";
        vals_eval = [](std::vector<size_t> const& vals){
          return *std::max_element(vals.begin(),vals.end());
        };
        break;
      case 5:
        std::cout << indent << "operation IS GREATER";
        vals_eval = [](std::vector<size_t> const& vals){
          return (vals[0] > vals[1])?1:0;
        };
        break;
      case 6:
        std::cout << indent << "operation IS LESS";
        vals_eval = [](std::vector<size_t> const& vals){
          return (vals[0] < vals[1])?1:0;
        };
        break;
      case 7:
        std::cout << indent << "operation IS EQUAL";
        vals_eval = [](std::vector<size_t> const& vals){
          return (vals[0] == vals[1])?1:0;
        };
        break;
      default:
        std::cout << indent << "ERROR - packet_type_ID = ??";
    }
    
    /*
     An operator packet contains one or more packets. To indicate which subsequent binary data represents its sub-packets, an operator packet can use one of two modes indicated by the bit immediately after the packet header; this is called the length type ID:
     */
    indent += "  ";
    if (read_bits(1, bin,result.bits_read) == "0") {
      /*
       If the length type ID is 0, then the next 15 bits are a number that represents the total length in bits of the sub-packets contained by this packet.
       */
      std::cout << indent << "<total lengths in bits>";
      auto package_length = read_bits(15, bin,result.bits_read);
      std::cout << indent << package_length;
      int bit_count = static_cast<int>(std::bitset<15>{package_length}.to_ullong());
      std::cout << indent << "= " << bit_count;
      auto eval_result = eval_packets_size(bin, bit_count);
      result.bits_read += eval_result.bits_read;
      result.val = eval_result.val;
      result.version_acc += eval_result.version_acc;
    }
    else {
      /*
       If the length type ID is 1, then the next 11 bits are a number that represents the number of sub-packets immediately contained by this packet.
       */
      std::cout << indent << "<sub packet count>";
      auto package_count_bs = read_bits(11, bin, result.bits_read);
      std::cout << indent << package_count_bs;
      int package_count = static_cast<int>(std::bitset<11>{package_count_bs}.to_ulong());
      std::cout << indent << "= " << package_count;
      auto eval_result = eval_packets_count(bin,package_count);
      result.bits_read += eval_result.bits_read;
      result.val = eval_result.val;
      result.version_acc += eval_result.version_acc;
    }
    indent.resize(indent.size()-2);
  }
  std::cout << indent << "vals:";
  for (auto const& val : result.val) std::cout << " " << val;
  auto result_val = vals_eval(result.val); // reduce val array of sub-packets into single value
  std::cout << " evals to " << result_val;
  result.val.clear();
  result.val.push_back(result_val);
  indent.resize(indent.size()-2);
  return result;
}
EvalResult eval_packets_size(Bin& bin,int bit_count) {
  indent += "  ";
  std::cout << indent << "eval_packets_size bit_count:" << bit_count;
  EvalResult result{};
  try {
    while (result.bits_read < bit_count) {
      auto eval_result = eval_packet(bin); // Recurse
      result.bits_read += eval_result.bits_read;
      result.val.push_back(eval_result.val[0]);
      result.version_acc += eval_result.version_acc;
      std::cout << indent << "bits_read:" << result.bits_read;
    }
  }
  catch (std::ios::failure const& e) {
    std::cout << indent << " bits_read:" << result.bits_read << " EXCEPTION = " << e.what();
  }
  indent.resize(indent.size()-2);
  return result;
}
EvalResult eval_packets_count(std::istream& bin,int package_count) {
  indent += "  ";
  std::cout << indent << "<eval_packets_count> package_count:" << package_count;
  EvalResult result{};
  try {
    for (int i=0;i<package_count;i++) {
      auto eval_result = eval_packet(bin); // Recurse
      result.bits_read += eval_result.bits_read;
      result.val.push_back(eval_result.val[0]);
      result.version_acc += eval_result.version_acc;
    }
  }
  catch (std::ios::failure const& e) {
    std::cout << indent << " bits_read:" << result.bits_read << " EXCEPTION = " << e.what();
  }
  indent.resize(indent.size()-2);
  return result;
}

EvalResult eval(std::istream& in) {
  EvalResult result{};
  std::string line{};
  while (in >> line) {
    std::cout << "\nin HEX[" << line.size() << "]:" << line;
  }
  std::string bin_digit_string{};
  for (auto const& hex_digit : line) {
    bin_digit_string += to_bit_string(hex_digit);
  }
  std::cout << "\nin binary:[" << bin_digit_string.size() << "]" << bin_digit_string;
  std::stringstream bin{bin_digit_string};

  // parse the bit stream
  result = eval_packet(bin);
  std::cout << indent << "<parse> read " << result.bits_read << " of " << bin_digit_string.size();
  return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto eval_result = eval(in);
    result = eval_result.version_acc;
    return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto eval_result = eval(in);
    if (eval_result.val.size()>1) std::cout << "\nERROR - eval should return only one value";
    result = eval_result.val[0];
    return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  // answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  // answers.push_back({"Part 1 Test1",part1::solve_for(pTest1)});
  // answers.push_back({"Part 1 Test2",part1::solve_for(pTest2)});
  // answers.push_back({"Part 1 Test3",part1::solve_for(pTest3)}); // sum 16
  // answers.push_back({"Part 1 Test4",part1::solve_for(pTest4)}); // sum 12
  // answers.push_back({"Part 1 Test5",part1::solve_for(pTest5)}); // sum 23
  // answers.push_back({"Part 1 Test6",part1::solve_for(pTest6)}); // sum 31
  // answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // for (int i=0;i<part_2_test_data.size();i++) {
  //   std::string label{std::string{"Part 2 Test"} + std::to_string(i)};
  //   answers.push_back({label,part2::solve_for(part_2_test_data[i].c_str())});
  // }
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pData = R"(00569F4A0488043262D30B333FCE6938EC5E5228F2C78A017CD78C269921249F2C69256C559CC01083BA00A4C5730FF12A56B1C49A480283C0055A532CF2996197653005FC01093BC4CE6F5AE49E27A7532200AB25A653800A8CAE5DE572EC40080CD26CA01CAD578803CBB004E67C573F000958CAF5FC6D59BC8803D1967E0953C68401034A24CB3ACD934E311004C5A00A4AB9CAE99E52648401F5CC4E91B6C76801F59DA63C1F3B4C78298014F91BCA1BAA9CBA99006093BFF916802923D8CC7A7A09CA010CD62DF8C2439332A58BA1E495A5B8FA846C00814A511A0B9004C52F9EF41EC0128BF306E4021FD005CD23E8D7F393F48FA35FCE4F53191920096674F66D1215C98C49850803A600D4468790748010F8430A60E1002150B20C4273005F8012D95EC09E2A4E4AF7041004A7F2FB3FCDFA93E4578C0099C52201166C01600042E1444F8FA00087C178AF15E179802F377EC695C6B7213F005267E3D33F189ABD2B46B30042655F0035300042A0F47B87A200EC1E84306C801819B45917F9B29700AA66BDC7656A0C49DB7CAEF726C9CEC71EC5F8BB2F2F37C9C743A600A442B004A7D2279125B73127009218C97A73C4D1E6EF64A9EFDE5AF4241F3FA94278E0D9005A32D9C0DD002AB2B7C69B23CCF5B6C280094CE12CDD4D0803CF9F96D1F4012929DA895290FF6F5E2A9009F33D796063803551006E3941A8340008743B8D90ACC015C00DDC0010B873052320002130563A4359CF968000B10258024C8DF2783F9AD6356FB6280312EBB394AC6FE9014AF2F8C381008CB600880021B0AA28463100762FC1983122D2A005CBD11A4F7B9DADFD110805B2E012B1F4249129DA184768912D90B2013A4001098391661E8803D05612C731007216C768566007280126005101656E0062013D64049F10111E6006100E90E004100C1620048009900020E0006DA0015C000418000AF80015B3D938)";
