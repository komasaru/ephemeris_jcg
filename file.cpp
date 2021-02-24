#include "file.hpp"

namespace ephemeris_jcg {

// 定数
const constexpr char kDeltaT[]  = "txt/delta_t.txt";
const constexpr char kParamP[]  = "txt/na";
const constexpr char kParamS[]  = "-data.txt";
const constexpr char kStrSun[]  = "^太陽の";
const constexpr char kStrVns[]  = "^金星の";
const constexpr char kStrMrs[]  = "^火星の";
const constexpr char kStrJpt[]  = "^木星の";
const constexpr char kStrSat[]  = "^土星の";
const constexpr char kStrMon[]  = "^月の";
const constexpr char kStrR[]    = "^Ｒ，黄道";
const constexpr char kStrKos[]  = "^恒星の";
const constexpr char kStrSp[]   = "^[\\s　]+|[\\s　]+$";
const constexpr char kStrNul[]  = "^$";
const constexpr char kStrAB[]   =
    "^.*"
    "a\\s*=\\s*(\\d+)\\s*,\\s*b\\s*=\\s*(\\d+).*"
    "a\\s*=\\s*(\\d+)\\s*,\\s*b\\s*=\\s*(\\d+).*"
    "a\\s*=\\s*(\\d+)\\s*,\\s*b\\s*=\\s*(\\d+)$";
const constexpr char kStrVal9[] =
    "(\\d+)\\s+"
    "([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+"
    "([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+"
    "([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+"
    "\\d+";
const constexpr char kStrVal6[] =
    "(\\d+)\\s+"
    "([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+"
    "([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+([\\-\\d\\.]+)\\s+"
    "\\d+";

/*
 * @brief      ΔT 一覧取得
 *
 * @param[in]  西暦年(unsigned int)
 * @return     ΔT(unsigned int)
 *             （対象年の ΔT データが存在しない場合、 0）
 */
unsigned int File::get_delta_t(unsigned int year) {
  std::string f(kDeltaT);  // ファイル名
  std::string buf;         // 1行分バッファ
  std::string s;           // 1行分文字列
  unsigned int k;          // 連想配列: キー
  unsigned int v;          // 連想配列: 値
  unsigned int dlt_t = 0;  // ΔT

  try {
    // ファイル OPEN
    std::ifstream ifs(f);
    if (!ifs) {
      std::cout << "[ERROR] Could not open \"" << f << "\"!" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // ファイル READ
    while (getline(ifs, buf)) {
      std::istringstream iss(buf);
      iss >> k >> v;
      if (k == year) {
        dlt_t = v;
        break;
      }
    }
  } catch (...) {
    throw;
  }

  return dlt_t;
}

/*
 * @brief       係数取得
 *
 *                R, 黄道傾角:  8 件
 *                月         : 30 件
 *                その他     : 18 件
 *                であるはずだが、件数のチェックは行わない。（現時点）
 *
 * @param[in]   西暦年 (unsigned int)
 * @param[in]   計算用時刻引数 (double)
 * @param[in]   計算用時刻引数 (double)
 * @param[ref]  係数適用期間一覧
 *              (unordered_map<string, tuple<unsigned int, unsigned int>>)
 * @param[ref]  係数一覧 (unordered_map<string, vector>)
 * @return      <none>
 */
void File::get_param(
    unsigned int year, double tm, double tm_r,
    std::unordered_map<std::string, std::tuple<unsigned int, unsigned int>>& ab,
    std::unordered_map<std::string, std::vector<double>>& param) {
  std::string f;                  // ファイル名
  std::string buf;                // 1行分バッファ
  std::smatch sm;                 // 正規表現マッチ
  std::regex re_sun(kStrSun);     // 正規表現: 太陽
  std::regex re_vns(kStrVns);     // 正規表現: 金星
  std::regex re_mrs(kStrMrs);     // 正規表現: 火星
  std::regex re_jpt(kStrJpt);     // 正規表現: 木星
  std::regex re_sat(kStrSat);     // 正規表現: 土星
  std::regex re_mon(kStrMon);     // 正規表現: 月
  std::regex re_r(kStrR);         // 正規表現: Ｒ，黄道傾角
  std::regex re_kos(kStrKos);     // 正規表現: 恒星
  std::regex re_sp(kStrSp);       // 正規表現: 行頭スペース
  std::regex re_nul(kStrNul);     // 正規表現: null 文字列
  std::regex re_ab(kStrAB);       // 正規表現: a=...,b=...
  std::regex re_val_9(kStrVal9);  // 正規表現: 値9列
  std::regex re_val_6(kStrVal6);  // 正規表現: 値6列
  std::string s;                  // 1行分文字列
  std::string div = "";           // 区分
  int c = -1;                     // 対象カラム(-1(無し), 0 - 2)
  unsigned int i;                 // loop index

  try {
    // ファイル名
    f = kParamP + std::to_string(year).substr(2, 2) + kParamS;

    // ファイル OPEN
    std::ifstream ifs(f);
    if (!ifs) {
      std::cout << "[ERROR] Could not open \"" << f << "\"!" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // ファイル READ
    while (getline(ifs, buf)) {
      std::istringstream iss(buf);
      s = std::regex_replace(buf, re_sp, "");
      // 区分取得
      if        (std::regex_search(s, sm, re_sun)) {
        div = "SUN";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_vns)) {
        div = "VNS";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_mrs)) {
        div = "MRS";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_jpt)) {
        div = "JPT";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_sat)) {
        div = "SAT";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_mon)) {
        div = "MON";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_r  )) {
        div = "R";
        c = -1;
        continue;
      } else if (std::regex_search(s, sm, re_nul)) {
        continue;
      } else if (std::regex_search(s, sm, re_kos)) {
        break;
      } else {
        // 係数一覧取得
        if (c != -1) {
          if        (std::regex_search(s, sm, re_val_9)) {
            // R, EPS 以外
            if        (div == "SUN") {
              param["SUN_RA"  ].push_back(stod(sm[c * 3 + 2]));
              param["SUN_DEC" ].push_back(stod(sm[c * 3 + 3]));
              param["SUN_DIST"].push_back(stod(sm[c * 3 + 4]));
            } else if (div == "VNS") {
              param["VNS_RA"  ].push_back(stod(sm[c * 3 + 2]));
              param["VNS_DEC" ].push_back(stod(sm[c * 3 + 3]));
              param["VNS_DIST"].push_back(stod(sm[c * 3 + 4]));
            } else if (div == "MRS") {
              param["MRS_RA"  ].push_back(stod(sm[c * 3 + 2]));
              param["MRS_DEC" ].push_back(stod(sm[c * 3 + 3]));
              param["MRS_DIST"].push_back(stod(sm[c * 3 + 4]));
            } else if (div == "JPT") {
              param["JPT_RA"  ].push_back(stod(sm[c * 3 + 2]));
              param["JPT_DEC" ].push_back(stod(sm[c * 3 + 3]));
              param["JPT_DIST"].push_back(stod(sm[c * 3 + 4]));
            } else if (div == "SAT") {
              param["SAT_RA"  ].push_back(stod(sm[c * 3 + 2]));
              param["SAT_DEC" ].push_back(stod(sm[c * 3 + 3]));
              param["SAT_DIST"].push_back(stod(sm[c * 3 + 4]));
            } else if (div == "MON") {
              param["MON_RA" ].push_back(stod(sm[c * 3 + 2]));
              param["MON_DEC"].push_back(stod(sm[c * 3 + 3]));
              param["MON_HP" ].push_back(stod(sm[c * 3 + 4]));
            }
          } else if (std::regex_search(s, sm, re_val_6)) {
            // R, EPS
            param["R"  ].push_back(stod(sm[c * 2 + 2]));
            param["EPS"].push_back(stod(sm[c * 2 + 3]));
          }
        }
      }
      if (div == "") continue;
      // 適用期間 a, b, 列取得
      if (std::regex_match(s, sm, re_ab)) {
        if (div == "R") {
          for (i = 0; i < 3; ++i) {
            if (stod(sm[i * 2 + 1]) <= tm_r && tm_r < stod(sm[i * 2 + 2])) {
              ab["R"  ] = std::make_tuple(stoi(sm[i * 2 + 1]),
                                          stoi(sm[i * 2 + 2]));
              ab["EPS"] = std::make_tuple(stoi(sm[i * 2 + 1]),
                                          stoi(sm[i * 2 + 2]));
              c = i;
              break;
            }
          }
        } else {
          for (i = 0; i < 3; ++i) {
            if (stod(sm[i * 2 + 1]) <= tm && tm < stod(sm[i * 2 + 2])) {
              ab[div] = std::make_tuple(stoi(sm[i * 2 + 1]),
                                        stoi(sm[i * 2 + 2]));
              c = i;
              break;
            }
          }
        }
        continue;
      }
    }
  } catch (...) {
    throw;
  }
}

}  // namespace ephemeris_jcg

