/***********************************************************
  海上保安庁の天測暦より太陽・月の視位置を計算
  （視黄経・視黄緯の計算を追加したもの）

    DATE          AUTHOR          VERSION
    2020.12.07    mk-mode.com     1.00 新規作成

  Copyright(C) 2020 mk-mode.com All Rights Reserved.

  引数 : UT1（世界時1）
           書式：最大23桁の数字
                 （先頭から、西暦年(4), 月(2), 日(2), 時(2), 分(2), 秒(2),
                             1秒未満(9)（小数点以下9桁（ナノ秒）まで））
                 無指定なら現在(システム日時)と判断。
***********************************************************/
#include "common.hpp"
#include "eph_jcg.hpp"

#include <cstdlib>   // for EXIT_XXXX
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::string tm_str;   // time string
  unsigned int s_tm;    // size of time string
  int s_nsec;           // size of nsec string
  int ret;              // return of functions
  struct timespec ut1;  // UTC
  struct tm t = {};     // for work
  namespace ns = ephemeris_jcg;

  try {
    // 日付取得
    if (argc > 1) {
      // コマンドライン引数より取得
      tm_str = argv[1];
      s_tm = tm_str.size();
      if (s_tm > 23) {
        std::cout << "[ERROR] Over 23-digits!" << std::endl;
        return EXIT_FAILURE;
      }
      s_nsec = s_tm - 14;
      std::istringstream is(tm_str);
      is >> std::get_time(&t, "%Y%m%d%H%M%S");
      ut1.tv_sec  = mktime(&t);
      ut1.tv_nsec = 0;
      if (s_tm > 14) {
        ut1.tv_nsec = std::stod(
            tm_str.substr(14, s_nsec) + std::string(9 - s_nsec, '0'));
      }
    } else {
      // 現在日時の取得
      ret = std::timespec_get(&ut1, TIME_UTC);
      if (ret != 1) {
        std::cout << "[ERROR] Could not get now time!" << std::endl;
        return EXIT_FAILURE;
      }
    }

    // Calculation & display
    ns::EphJcg o_e(ut1);
    std::cout << "[ UT1: " << ns::gen_time_str(ut1) << " ]" << std::endl;
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "SUN     R.A. =  "
              << std::setw(12) << o_e.sun_ra   << " h"
              << " (= " << ns::hour2hms(o_e.sun_ra) << ")" << std::endl;
    std::cout << "        Dec. =  "
              << std::setw(12) << o_e.sun_dec  << " °"
              << " (= " << ns::deg2dms(o_e.sun_dec) << ")" << std::endl;
    std::cout << "       Dist. =  "
              << std::setw(12) << o_e.sun_dist << " AU" << std::endl;
    std::cout << "          hG =  "
              << std::setw(12) << o_e.sun_hg   << " h"
              << " (= " << ns::hour2hms(o_e.sun_hg) << ")" << std::endl;
    std::cout << "        S.D. =  "
              << std::setw(12) << o_e.sun_sd   << " ′"
              << " (= " << ns::deg2dms(o_e.sun_sd / 60.0) << ")" << std::endl;
    std::cout << "VENUS   R.A. =  "
              << std::setw(12) << o_e.vns_ra   << " h"
              << " (= " << ns::hour2hms(o_e.vns_ra) << ")" << std::endl;
    std::cout << "        Dec. =  "
              << std::setw(12) << o_e.vns_dec  << " °"
              << " (= " << ns::deg2dms(o_e.vns_dec) << ")" << std::endl;
    std::cout << "       Dist. =  "
              << std::setw(12) << o_e.vns_dist << " AU" << std::endl;
    std::cout << "          hG =  "
              << std::setw(12) << o_e.vns_hg   << " h"
              << " (= " << ns::hour2hms(o_e.vns_hg) << ")" << std::endl;
    std::cout << "        S.D. =  "
              << std::setw(12) << o_e.vns_sd   << " ″"
              << " (= " << ns::deg2dms(o_e.vns_sd / 3600.0) << ")" << std::endl;
    std::cout << "MARS    R.A. =  "
              << std::setw(12) << o_e.mrs_ra   << " h"
              << " (= " << ns::hour2hms(o_e.mrs_ra) << ")" << std::endl;
    std::cout << "        Dec. =  "
              << std::setw(12) << o_e.mrs_dec  << " °"
              << " (= " << ns::deg2dms(o_e.mrs_dec) << ")" << std::endl;
    std::cout << "       Dist. =  "
              << std::setw(12) << o_e.mrs_dist << " AU" << std::endl;
    std::cout << "          hG =  "
              << std::setw(12) << o_e.mrs_hg   << " h"
              << " (= " << ns::hour2hms(o_e.mrs_hg) << ")" << std::endl;
    std::cout << "        S.D. =  "
              << std::setw(12) << o_e.mrs_sd   << " ″"
              << " (= " << ns::deg2dms(o_e.mrs_sd / 3600.0) << ")" << std::endl;
    std::cout << "JUPITER R.A. =  "
              << std::setw(12) << o_e.jpt_ra   << " h"
              << " (= " << ns::hour2hms(o_e.jpt_ra) << ")" << std::endl;
    std::cout << "        Dec. =  "
              << std::setw(12) << o_e.jpt_dec  << " °"
              << " (= " << ns::deg2dms(o_e.jpt_dec) << ")" << std::endl;
    std::cout << "       Dist. =  "
              << std::setw(12) << o_e.jpt_dist << " AU" << std::endl;
    std::cout << "          hG =  "
              << std::setw(12) << o_e.jpt_hg   << " h"
              << " (= " << ns::hour2hms(o_e.jpt_hg) << ")" << std::endl;
    std::cout << "     S.D.(P) =  "
              << std::setw(12) << o_e.jpt_sd_p << " ″"
              << " (= " << ns::deg2dms(o_e.jpt_sd_p / 3600.0) << ")" << std::endl;
    std::cout << "     S.D.(E) =  "
              << std::setw(12) << o_e.jpt_sd_e << " ″"
              << " (= " << ns::deg2dms(o_e.jpt_sd_e / 3600.0) << ")" << std::endl;
    std::cout << "SATURN  R.A. =  "
              << std::setw(12) << o_e.sat_ra   << " h"
              << " (= " << ns::hour2hms(o_e.sat_ra) << ")" << std::endl;
    std::cout << "        Dec. =  "
              << std::setw(12) << o_e.sat_dec  << " °"
              << " (= " << ns::deg2dms(o_e.sat_dec) << ")" << std::endl;
    std::cout << "       Dist. =  "
              << std::setw(12) << o_e.sat_dist << " AU" << std::endl;
    std::cout << "          hG =  "
              << std::setw(12) << o_e.sat_hg   << " h"
              << " (= " << ns::hour2hms(o_e.sat_hg) << ")" << std::endl;
    std::cout << "     S.D.(P) =  "
              << std::setw(12) << o_e.sat_sd_p << " ″"
              << " (= " << ns::deg2dms(o_e.sat_sd_p / 3600.0) << ")" << std::endl;
    std::cout << "     S.D.(E) =  "
              << std::setw(12) << o_e.sat_sd_e << " ″"
              << " (= " << ns::deg2dms(o_e.sat_sd_e / 3600.0) << ")" << std::endl;
    std::cout << "R            =  "
              << std::setw(12) << o_e.r        << " h"
              << " (= " << ns::hour2hms(o_e.r) << ")" << std::endl;
    std::cout << "EPSILON      =  "
              << std::setw(12) << o_e.eps      << " °"
              << " (= " << ns::deg2dms(o_e.eps) << ")" << std::endl;
    std::cout << "MOON    R.A. =  "
              << std::setw(12) << o_e.mon_ra   << " h"
              << " (= " << ns::hour2hms(o_e.mon_ra) << ")" << std::endl;
    std::cout << "        Dec. =  "
              << std::setw(12) << o_e.mon_dec  << " °"
              << " (= " << ns::deg2dms(o_e.mon_dec) << ")" << std::endl;
    std::cout << "        H.P. =  "
              << std::setw(12) << o_e.mon_hp   << " °"
              << " (= " << ns::deg2dms(o_e.mon_hp) << ")" << std::endl;
    std::cout << "          hG =  "
              << std::setw(12) << o_e.mon_hg   << " h"
              << " (= " << ns::hour2hms(o_e.mon_hg) << ")" << std::endl;
    std::cout << "        S.D. =  "
              << std::setw(12) << o_e.mon_sd   << " ′"
              << " (= " << ns::deg2dms(o_e.mon_sd / 60.0) << ")" << std::endl;
  } catch (...) {
      std::cerr << "EXCEPTION!" << std::endl;
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

