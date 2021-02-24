#ifndef EPHEMERIS_JCG_COMMON_HPP_
#define EPHEMERIS_JCG_COMMON_HPP_

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace ephemeris_jcg {

// -------------------------------------
//   Functions
// -------------------------------------
struct timespec jst2utc(struct timespec);
std::string gen_time_str(struct timespec);
std::string hour2hms(double);
std::string deg2dms(double);

}  // namespace ephemeris_jcg

#endif

