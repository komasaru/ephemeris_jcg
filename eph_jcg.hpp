#ifndef EPHEMERIS_JCG_EPH_JCG_HPP_
#define EPHEMERIS_JCG_EPH_JCG_HPP_

#include "file.hpp"

#include <cmath>
#include <cstdlib>   // for EXIT_XXXX
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace ephemeris_jcg {

class EphJcg {
  std::unordered_map<unsigned int, unsigned int> l_dlt_t;   // List of ΔT
  struct timespec ts;      // timespec of UT1
  unsigned int year;       // 西暦年(UT1)
  unsigned int month;      // 月(UT1)
  unsigned int day;        // 日(UT1)
  unsigned int hour;       // 時(UT1)
  unsigned int min;        // 分(UT1)
  unsigned int sec;        // 秒(UT1)
  unsigned int nsec;       // ナノ秒(UT1)
  unsigned int t;          // 通日 T（1月0日を第0日とする）
  double f;                // UT1 の日の端数
  unsigned int dlt_t;      // ΔT（TT（地球時） - UT1（世界時1））
  double tm;               // 計算用時刻引数
  double tm_r;             // 計算用時刻引数（R 計算用）
  std::unordered_map<std::string,
                     std::tuple<unsigned int, unsigned int>> ab;  // 係数適用期間一覧
  std::unordered_map<std::string, std::vector<double>> param;     // 係数一覧

public:
  double sun_ra;            // SUN R.A.
  double sun_dec;           // SUN Dec.
  double sun_dist;          // SUN Dist.
  double vns_ra;            // VNS R.A.
  double vns_dec;           // VNS Dec.
  double vns_dist;          // VNS Dist.
  double mrs_ra;            // MRS R.A.
  double mrs_dec;           // MRS Dec.
  double mrs_dist;          // MRS Dist.
  double jpt_ra;            // JPT R.A.
  double jpt_dec;           // JPT Dec.
  double jpt_dist;          // JPT Dist.
  double sat_ra;            // SAT R.A.
  double sat_dec;           // SAT Dec.
  double sat_dist;          // SAT Dist.
  double mon_ra;            // MON R.A.
  double mon_dec;           // MON Dec.
  double mon_hp;            // MON H.P.
  double r;                 // R
  double eps;               // ε
  double sun_hg;            // SUN グリニッジ時角
  double vns_hg;            // VNS グリニッジ時角
  double mrs_hg;            // MRS グリニッジ時角
  double jpt_hg;            // JPT グリニッジ時角
  double sat_hg;            // SAT グリニッジ時角
  double mon_hg;            // MON グリニッジ時角
  double sun_sd;            // SUN 視半径
  double vns_sd;            // VNS 視半径
  double mrs_sd;            // MRS 視半径
  double jpt_sd;            // JPT 視半径
  double sat_sd;            // SAT 視半径
  double jpt_sd_p;          // JPT 視半径（極半径）
  double jpt_sd_e;          // JPT 視半径（赤道半径）
  double sat_sd_p;          // SAT 視半径（極半径）
  double sat_sd_e;          // SAT 視半径（赤道半径）
  double mon_sd;            // MON 視半径
  EphJcg(struct timespec);  // コンストラクタ

private:
  void get_ut1();      // 取得: UT1（年・月・日・時・分・秒・ナノ秒）
  void calc_t();       // 計算: 通日 T
  void calc_f();       // 計算: UT1 の日の端数
  void calc_tm();      // 計算: 計算用時刻引数
  void calc_val();     // 計算: 各種
  double calc_cmn(std::string, double);                   // 計算: 共通
  double calc_theta(unsigned int, unsigned int, double);  // 計算: θ
  double calc_ft(std::string, double);                    // 計算: 所要値
  double calc_hg(double);                                 // 計算: グリニッジ時角
  double calc_sd_sun();                                   // 計算: 視半径（太陽）
  double calc_sd_mon();                                   // 計算: 視半径（月）
  double calc_sd_etc(double, double);                     // 計算: 視半径（金・火・木・土星）
};

}  // namespace ephemeris_jcg

#endif

