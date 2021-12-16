#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <array>

char const* pTest = R"(D2FE28)";
char const* pTest1 = R"(38006F45291200)";
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

BitString read_bits(int bit_count, std::istream& bin) {
  BitString result;
  std::copy_n(std::istream_iterator<char>{bin},bit_count,std::back_inserter(result));
  return result;
}

void parse_packet(std::istream& in);
void parse_packets_size(std::istream& in,size_t bit_count);
void parse_packets_count(std::istream& in,size_t package_count);

void parse_packet(std::istream& in) {
  
}
void parse_packets_size(std::istream& in,size_t bit_count) {}
void parse_packets_count(std::istream& in,size_t package_count) {}


Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (in >> line) {
    std::cout << "\nin HEX[" << line.size() << "]:" << line;
  }
  std::string bin_digit_string{};
  for (auto const& hex_digit : line) {
    bin_digit_string += to_bit_string(hex_digit);
  }
  std::cout << "\nin binary:[" << bin_digit_string.size() << "]" << bin_digit_string;
  std::string indent{"\n  "};
  std::stringstream bin{bin_digit_string};

  // parse the bit stream
  {
    // Every packet begins with a standard header: the first three bits encode the packet version,
    // and the next three bits encode the packet type ID.
    auto packet_version = read_bits(3,bin);
    std::cout << indent << "packet version:" << packet_version;

    auto packet_type_ID = read_bits(3,bin);
    std::cout << indent << "packet type ID:" << packet_type_ID;
    
    if (packet_type_ID=="100") {
      /*
      Packets with type ID 4 represent a literal value. Literal value packets encode a single binary number. To do this, the binary number is padded with leading zeroes until its length is a multiple of four bits, and then it is broken into groups of four bits. Each group is prefixed by a 1 bit except the last group, which is prefixed by a 0 bit. These groups of five bits immediately follow the packet header.
       */
      indent += "  ";
      std::cout << indent << "<Literal Value>";
      bool not_last_package = true;
      BitString literal_value{};
      while (not_last_package) {
        not_last_package = read_bits(1, bin) == "1";
        std::copy_n(std::istream_iterator<char>{bin},4,std::back_inserter(literal_value));
        std::cout << indent << "*" << literal_value;
      }
      if (literal_value.size()<=64) {
        std::cout << indent << "= " << std::bitset<128>{literal_value}.to_ullong();
      }
      else {
        std::cout << indent <<  "OVERFLOW ERROR";
      }
    }
    else {
      /*
       Every other type of packet (any packet with a type ID other than 4) represent an operator that performs some calculation on one or more sub-packets contained within.
       */
      
      /*
       An operator packet contains one or more packets. To indicate which subsequent binary data represents its sub-packets, an operator packet can use one of two modes indicated by the bit immediately after the packet header; this is called the length type ID:
       */
      indent += "  ";
      if (read_bits(1, bin) == "0") {
        /*
         If the length type ID is 0, then the next 15 bits are a number that represents the total length in bits of the sub-packets contained by this packet.
         */
        std::cout << indent << "<total lengths in bits>";
        auto package_length = read_bits(15, bin);
        std::cout << indent << package_length;
        std::cout << indent << "= " << std::bitset<15>{package_length}.to_ullong();
      }
      else {
        /*
         If the length type ID is 1, then the next 11 bits are a number that represents the number of sub-packets immediately contained by this packet.
         */
        std::cout << indent << "<sub packet count>";
      }
    }
    std::cout << "\n...TBD";
  }
  return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
    Result result{};
    std::stringstream in{ pData };
    auto data_model = parse(in);
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
  answers.push_back({"Part 1 Test1",part1::solve_for(pTest1)});
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

char const* pData = R"(00569F4A0488043262D30B333FCE6938EC5E5228F2C78A017CD78C269921249F2C69256C559CC01083BA00A4C5730FF12A56B1C49A480283C0055A532CF2996197653005FC01093BC4CE6F5AE49E27A7532200AB25A653800A8CAE5DE572EC40080CD26CA01CAD578803CBB004E67C573F000958CAF5FC6D59BC8803D1967E0953C68401034A24CB3ACD934E311004C5A00A4AB9CAE99E52648401F5CC4E91B6C76801F59DA63C1F3B4C78298014F91BCA1BAA9CBA99006093BFF916802923D8CC7A7A09CA010CD62DF8C2439332A58BA1E495A5B8FA846C00814A511A0B9004C52F9EF41EC0128BF306E4021FD005CD23E8D7F393F48FA35FCE4F53191920096674F66D1215C98C49850803A600D4468790748010F8430A60E1002150B20C4273005F8012D95EC09E2A4E4AF7041004A7F2FB3FCDFA93E4578C0099C52201166C01600042E1444F8FA00087C178AF15E179802F377EC695C6B7213F005267E3D33F189ABD2B46B30042655F0035300042A0F47B87A200EC1E84306C801819B45917F9B29700AA66BDC7656A0C49DB7CAEF726C9CEC71EC5F8BB2F2F37C9C743A600A442B004A7D2279125B73127009218C97A73C4D1E6EF64A9EFDE5AF4241F3FA94278E0D9005A32D9C0DD002AB2B7C69B23CCF5B6C280094CE12CDD4D0803CF9F96D1F4012929DA895290FF6F5E2A9009F33D796063803551006E3941A8340008743B8D90ACC015C00DDC0010B873052320002130563A4359CF968000B10258024C8DF2783F9AD6356FB6280312EBB394AC6FE9014AF2F8C381008CB600880021B0AA28463100762FC1983122D2A005CBD11A4F7B9DADFD110805B2E012B1F4249129DA184768912D90B2013A4001098391661E8803D05612C731007216C768566007280126005101656E0062013D64049F10111E6006100E90E004100C1620048009900020E0006DA0015C000418000AF80015B3D938)";
