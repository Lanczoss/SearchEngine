#ifndef PAGE_LIB_PREPROCESSOR_H_
#define PAGE_LIB_PREPROCESSOR_H_

#include <stdint.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "simhash/Simhasher.hpp"
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using namespace simhash;
class SplitTool;
class WebPage;

struct EqualFringer {
  bool operator()(const uint64_t &lhs, const uint64_t &rhs) const {
    return Simhasher::isEqual(lhs, rhs);
  }
};

class PageLibPreprocessor {
 public:
  PageLibPreprocessor(SplitTool *tool);
  void cutRedundantPage();
  void buildInvertIndexMap();
  void storeOnDisk();

 private:
  void readOffsetLib();
  double calWeight(const int &, const int &, const int &);
  // 识别字符的字节数
  size_t nBytesCode(const char);

 private:
  // 网络偏移库
  unordered_map<int, pair<int, int>, std::hash<int>> _offsetLib;
  // 去重网络库指纹
  unordered_set<uint64_t, std::hash<uint64_t>, EqualFringer> _pageLib;
  // 倒排索引
  unordered_map<string, set<pair<int, double>>> _invertIndexLib;
  // jieba
  SplitTool *_wordCutter;
};

#endif  // !PAGE_LIB_PRE_PROCESSOR_H_
