#ifndef EPHEMERIS_JCG_FILE_HPP_
#define EPHEMERIS_JCG_FILE_HPP_

#include <cstdlib>   // for EXIT_XXXX
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

namespace ephemeris_jcg {

class File {

public:
  unsigned int get_delta_t(unsigned int);  // 取得: ΔT
  void get_param(
      unsigned int, double, double,
      std::unordered_map<std::string, std::tuple<unsigned int, unsigned int>>&,
      std::unordered_map<std::string, std::vector<double>>&);  // 取得: 係数
};

}  // namespace ephemeris_jcg

#endif

