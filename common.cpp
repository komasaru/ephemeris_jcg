#include "common.hpp"

namespace ephemeris_jcg {

// -------------------------------------
//   Constants
// -------------------------------------
static constexpr unsigned int kJstOffset = 9;     // JST - UTC (hours)
static constexpr unsigned int kSecInHour = 3600;  // Seconds in an hour

// -------------------------------------
//   Functions
// -------------------------------------
/*
 * @brief      JST -> UTC 変換
 *
 * @param[in]  JST (timespec)
 * @return     UTC (timespec)
 */
struct timespec jst2utc(struct timespec ts_jst) {
  struct timespec ts;

  try {
    ts.tv_sec  = ts_jst.tv_sec - kJstOffset * kSecInHour;
    ts.tv_nsec = ts_jst.tv_nsec;
  } catch (...) {
    throw;
  }

  return ts;
}

/*
 * @brief      日時文字列生成
 *
 * @param[in]  日時 (timespec)
 * @return     日時文字列 (string)
 */
std::string gen_time_str(struct timespec ts) {
  struct tm t;
  std::stringstream ss;
  std::string str_tm;

  try {
    localtime_r(&ts.tv_sec, &t);
    ss << std::setfill('0')
       << std::setw(4) << t.tm_year + 1900 << "-"
       << std::setw(2) << t.tm_mon + 1     << "-"
       << std::setw(2) << t.tm_mday        << " "
       << std::setw(2) << t.tm_hour        << ":"
       << std::setw(2) << t.tm_min         << ":"
       << std::setw(2) << t.tm_sec         << "."
       << std::setw(3) << ts.tv_nsec / 1000000;
    return ss.str();
  } catch (...) {
    throw;
  }
}

/*
 * @brief      99.999h -> 99h99m99s 変換
 *
 * @param[in]  時間(hour) (double)
 * @return     時分秒文字列 (string)
 */
std::string hour2hms(double hour) {
  int    h;
  double h_r;
  int    m;
  double m_r;
  double s;
  std::stringstream ss;
  std::string hms = "";

  try {
    h   = hour;
    h_r = hour - h;
    m   = h_r * 60.0;
    m_r = h_r * 60.0 - m;
    s   = m_r * 60.0;
    if (m < 0  ) m *= -1;
    if (s < 0.0) s *= -1.0;
    ss << std::setw(3) << h << " h "
       << std::setw(2) << m << " m "
       << std::setw(6) << std::fixed << std::setprecision(3) << s << " s";
    hms = ss.str();
  } catch (...) {
    throw;
  }

  return hms;
}

/*
 * @brief      99.999° -> 99°99′99″ 変換
 *
 * @param[in]  度 (double)
 * @return     時分秒文字列 (string)
 */
std::string deg2dms(double deg) {
  std::string pm = " ";
  int    d;
  double d_r;
  int    m;
  double m_r;
  double s;
  std::stringstream ss;
  std::string dms = "";

  try {
    if (deg < 0.0) {
      pm = "-";
      deg *= -1;
    }
    d   = deg;
    d_r = deg - d;
    m   = d_r * 60.0;
    m_r = d_r * 60.0 - m;
    s   = m_r * 60.0;
    if (m < 0  ) m *= -1;
    if (s < 0.0) s *= -1.0;
    ss << pm
       << std::setw(3) << d << " °"
       << std::setw(2) << m << " ′"
       << std::setw(6) << std::fixed << std::setprecision(3) << s << " ″";
    dms = ss.str();
  } catch (...) {
    throw;
  }

  return dms;
}

}  // namespace ephemeris_jcg

