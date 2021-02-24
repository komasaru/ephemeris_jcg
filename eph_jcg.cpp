#include "eph_jcg.hpp"

namespace ephemeris_jcg {

// 定数
static constexpr double       kHourDay = 24.0;     // Hours   in a day
static constexpr double       kMinDay  = 1440.0;   // Minutes in a day
static constexpr double       kSecDay  = 86400.0;  // Seconds in a day
static constexpr double       kPi      = atan(1.0) * 4;  // PI
static constexpr unsigned int kSizeS   = 18;      // 係数の数: 太陽, etc.
static constexpr unsigned int kSizeR   = 8;       // 係数の数: R, 黄道傾角
static constexpr unsigned int kSizeM   = 30;      // 係数の数: 月
static constexpr double       kS0Sun   = 16.02;   // SD 計算用係数: （太陽; ′）
static constexpr double       kS0Vns   = 8.3;     // SD 計算用係数: （金星; ″）
static constexpr double       kS0Mrs   = 4.7;     // SD 計算用係数: （火星; ″）
static constexpr double       kS0JptP  = 92.1;    // SD 計算用係数: （木星・極半径; ″）
static constexpr double       kS0JptE  = 98.4;    // SD 計算用係数: （木星・赤道半径; ″）
static constexpr double       kS0SatP  = 73.8;    // SD 計算用係数: （土星・極半径; ″）
static constexpr double       kS0SatE  = 82.7;    // SD 計算用係数: （土星・赤道半径; ″）
static constexpr double       kS0Mon   = 0.2725;  // SD 計算用係数: （月）

/*
 * @brief  コンストラクタ
 *
 * @param[in]  UT1 (timespec)
 */
EphJcg::EphJcg(struct timespec ts) {
  this->ts = ts;  // UT1
  get_ut1();                      // 取得: UT1（年月日時分秒）
  File o_f;
  dlt_t = o_f.get_delta_t(year);  // 取得: ΔT
  if (dlt_t == 0) {
    std::cout << "[ERROR] " << year << " is out of range!" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  calc_t();                // 計算: 通日 T
  calc_f();                // 計算: 世界時 UT（時・分・秒） の日の端数
  calc_tm();               // 計算: 計算用時刻引数
  o_f.get_param(year, tm, tm_r, ab, param);  // 取得: 係数
  calc_val();              // 計算: 各種
}

// -------------------------------------
// 以下、 private functions
// -------------------------------------

/*
 * @brief   取得: UT1（年・月・日・時・分・秒・ナノ秒）
 *
 * @param   <none>
 * @return  <none>
 */
void EphJcg::get_ut1() {
  struct tm t;

  try {
    localtime_r(&ts.tv_sec, &t);
    year  = t.tm_year + 1900;
    month = t.tm_mon + 1;
    day   = t.tm_mday;
    hour  = t.tm_hour;
    min   = t.tm_min;
    sec   = t.tm_sec;
    nsec  = ts.tv_nsec;
  } catch (...) {
    throw;
  }
}

/*
 * @brief   計算: 通日 T
 *
 * @param   <none>
 * @return  <none>
 */
void EphJcg::calc_t() {
  unsigned int p;
  unsigned int q;
  unsigned int y;
  unsigned int s;

  try {
    p = month - 1;
    q = static_cast<int>((month + 7) / 10.0);
    y = static_cast<int>((year / 4.0) - static_cast<int>(year / 4.0) + 0.77);
    s = static_cast<int>(p * 0.55 - 0.33);
    t = 30 * p + q * (s - y) + p * (1 - q) + day;
  } catch (...) {
    throw;
  }
}

/*
 * @brief   計算: 世界時 UT の日の端数 F
 *
 * @param   <none>
 * @return  <none>
 */
void EphJcg::calc_f() {
  try {
    f = hour / kHourDay
      + min / kMinDay
      + sec / kSecDay
      +  nsec / (kSecDay * 1.0e9);
  } catch (...) {
    throw;
  }
}

/*
 * @brief   計算: 計算用時刻引数 t
 *
 * @param   <none>
 * @return  <none>
 */
void EphJcg::calc_tm() {
  try {
    tm_r = t + f;
    tm   = tm_r + dlt_t / kSecDay;
  } catch (...) {
    throw;
  }
}

/*
 * @brief   計算: 各種
 *
 * @param   <none>
 * @return  <none>
 */
void EphJcg::calc_val() {
  try {
    sun_ra   = calc_cmn("SUN_RA",   tm);        // R.A. (太陽) (h)
    sun_dec  = calc_cmn("SUN_DEC",  tm);        // Dec. (太陽) (°)
    sun_dist = calc_cmn("SUN_DIST", tm);        // Dist.(太陽) (au)
    vns_ra   = calc_cmn("VNS_RA",   tm);        // R.A. (金星) (h)
    vns_dec  = calc_cmn("VNS_DEC",  tm);        // Dec. (金星) (°)
    vns_dist = calc_cmn("VNS_DIST", tm);        // Dist.(金星) (au)
    mrs_ra   = calc_cmn("MRS_RA",   tm);        // R.A. (火星) (h)
    mrs_dec  = calc_cmn("MRS_DEC",  tm);        // Dec. (火星) (°)
    mrs_dist = calc_cmn("MRS_DIST", tm);        // Dist.(火星) (au)
    jpt_ra   = calc_cmn("JPT_RA",   tm);        // R.A. (木星) (h)
    jpt_dec  = calc_cmn("JPT_DEC",  tm);        // Dec. (木星) (°)
    jpt_dist = calc_cmn("JPT_DIST", tm);        // Dist.(木星) (au)
    sat_ra   = calc_cmn("SAT_RA",   tm);        // R.A. (土星) (h)
    sat_dec  = calc_cmn("SAT_DEC",  tm);        // Dec. (土星) (°)
    sat_dist = calc_cmn("SAT_DIST", tm);        // Dist.(土星) (au)
    mon_ra   = calc_cmn("MON_RA",   tm);        // R.A. (月) (h)
    mon_dec  = calc_cmn("MON_DEC",  tm);        // Dec. (月) (°)
    mon_hp   = calc_cmn("MON_HP",   tm);        // H.P. (月) (°)
    r        = calc_cmn("R",      tm_r);        // R
    eps      = calc_cmn("EPS",      tm);        // ε
    sun_hg   = calc_hg(sun_ra);                 // グリニッジ時角（太陽）
    vns_hg   = calc_hg(vns_ra);                 // グリニッジ時角（金星）
    mrs_hg   = calc_hg(mrs_ra);                 // グリニッジ時角（火星）
    jpt_hg   = calc_hg(jpt_ra);                 // グリニッジ時角（木星）
    sat_hg   = calc_hg(sat_ra);                 // グリニッジ時角（土星）
    mon_hg   = calc_hg(mon_ra);                 // グリニッジ時角（月）
    sun_sd   = calc_sd_sun();                   // 視半径（太陽）
    vns_sd   = calc_sd_etc(kS0Vns,  vns_dist);  // 視半径（金星）
    mrs_sd   = calc_sd_etc(kS0Mrs,  mrs_dist);  // 視半径（火星）
    jpt_sd_p = calc_sd_etc(kS0JptP, jpt_dist);  // 視半径（木星）
    jpt_sd_e = calc_sd_etc(kS0JptE, jpt_dist);  // 視半径（木星）
    sat_sd_p = calc_sd_etc(kS0SatP, sat_dist);  // 視半径（土星）
    sat_sd_e = calc_sd_etc(kS0SatE, sat_dist);  // 視半径（土星）
    mon_sd   = calc_sd_mon();                   // 視半径（月）
  } catch (...) {
    throw;
  }
}

/*
 * @brief      計算: 共通
 *
 * @param[in]  区分 (string)
 * @param[in]  時刻引数 (double)
 * @return     <none>
 */
double EphJcg::calc_cmn(std::string div, double tm) {
  unsigned int a = 0;
  unsigned int b = 0;
  double       theta;
  double       v = 0.0;

  try {
    if        (div == "SUN_RA" || div == "SUN_DEC" || div == "SUN_DIST") {
      a = std::get<0>(ab["SUN"]);
      b = std::get<1>(ab["SUN"]);
    } else if (div == "VNS_RA" || div == "VNS_DEC" || div == "VNS_DIST") {
      a = std::get<0>(ab["VNS"]);
      b = std::get<1>(ab["VNS"]);
    } else if (div == "MRS_RA" || div == "MRS_DEC" || div == "MRS_DIST") {
      a = std::get<0>(ab["MRS"]);
      b = std::get<1>(ab["MRS"]);
    } else if (div == "JPT_RA" || div == "JPT_DEC" || div == "JPT_DIST") {
      a = std::get<0>(ab["JPT"]);
      b = std::get<1>(ab["JPT"]);
    } else if (div == "SAT_RA" || div == "SAT_DEC" || div == "SAT_DIST") {
      a = std::get<0>(ab["SAT"]);
      b = std::get<1>(ab["SAT"]);
    } else if (div == "MON_RA" || div == "MON_DEC" || div == "MON_HP"  ) {
      a = std::get<0>(ab["MON"]);
      b = std::get<1>(ab["MON"]);
    } else if (div == "R" || div == "EPS") {
      a = std::get<0>(ab[div]);
      b = std::get<1>(ab[div]);
    }
    theta = calc_theta(a, b, tm);
    v = calc_ft(div, theta);
    if (div == "SUN_RA" || div == "VNS_RA" || div == "MRS_RA" ||
        div == "JPT_RA" || div == "SAT_RA" || div == "MON_RA" ||
        div == "R") {
      while (v >= 24.0) v -= 24.0;
      while (v <   0.0) v += 24.0;
    }
  } catch (...) {
    throw;
  }

  return v;
}

/*
 * @brief      計算: θ
 *
 * @param[in]  期間（開始） a (unsigned int)
 * @param[in]  期間（終了） b (unsigned int)
 * @param[in]  時刻引数 (double)
 * @return     θ (double)
 */
double EphJcg::calc_theta(unsigned int a, unsigned int b, double tm) {
  double v;
  double theta;

  try {
    if (b < tm) b = tm;  // 年末のΔT秒分も計算可能とするための応急処置
    v = (2 * tm - (a + b)) / (b - a);
    if (v >  1.0) v =  1.0;
    if (v < -1.0) v = -1.0;
    theta = acos(v) * 180.0 / kPi;
  } catch (...) {
    throw;
  }

  return theta;
}

/*
 * @brief      計算: 所要値
 *             * θ, 係数配列から次式により所要値を計算する。
 *                 f(t) = C_0 + C_1 * cos(θ) + C_2 * cos(2θ) + ...
 *                      + C_N * cos(Nθ)
 *
 * @param[in]  区分 (string)
 * @param[in]  θ (double)
 * @return     ft (double)
 */
double EphJcg::calc_ft(std::string div, double theta) {
  unsigned int i;
  unsigned int i_max;
  double ft = 0.0;

  try {
    if (div == "R" || div == "EPS") {
      i_max = kSizeR;
    } else if (div == "MON_RA" || div == "MON_DEC" || div == "MON_HP") {
      i_max = kSizeM;
    } else {
      i_max = kSizeS;
    }
    for (i = 0; i < i_max; ++i) {
      ft += param[div][i] * cos(theta * i * kPi / 180.0);
    }
  } catch (...) {
    throw;
  }

  return ft;
}

/*
 * @brief      計算: グリニッジ時角
 *
 * @param[in]  R.A. (double)
 * @return     グリニッジ時角 (double)
 */
double EphJcg::calc_hg(double ra) {
  double hg;

  try {
    hg = r - ra + f * 24.0;;
  } catch (...) {
    throw;
  }

  return hg;
}

/*
 * @brief   計算: 視半径（太陽）
 *          * 次式により視半径を計算する。
 *              S.D. = 16.02 ′/ Dist.
 *
 * @param   <none>
 * @return  視半径 (double)
 */
double EphJcg::calc_sd_sun() {
  double sd;

  try {
    sd = kS0Sun / sun_dist;
  } catch (...) {
    throw;
  }

  return sd;
}

/*
 * @brief   計算: 視半径（月）
 *          * 次式により視半径を計算する。
 *              S.D. = sin^(-1) (0.2725 * sin(H.P.))
 *
 * @param   <none>
 * @return  視半径 (double)
 */
double EphJcg::calc_sd_mon() {
  double sd;

  try {
    sd = asin(kS0Mon * sin(mon_hp * kPi / 180.0)) * 60.0 * 180.0 / kPi;
  } catch (...) {
    throw;
  }

  return sd;
}

/*
 * @brief   計算: 視半径（金・火・木・土星）
 *          * 次式により視半径を計算する。
 *              S.D. = S_0 * Dist.
 *
 * @param[in]  Dist. (double)
 * @return     視半径 (double)
 */
double EphJcg::calc_sd_etc(double s_0, double dist) {
  double sd;

  try {
    sd = s_0 / dist;
  } catch (...) {
    throw;
  }

  return sd;
}

}  // namespace ephemeris_jcg

