#ifndef WEB_QUERY_H_
#define WEB_QUERY_H_

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SplitTool.h"
#include "WebPage.h"
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class WebPage;

class WebPageQuery {
 public:
  WebPageQuery(SplitTool* tool);
  vector<WebPage> doQuery(const string& key);

 private:
  void loadLib();
  // 识别字符的字节数
  size_t nBytesCode(const char);
  double calWeight(const int& freqInOnePage, const int& pageNumber,
                   const int& pageSumNumber);

 private:
  // 去重网页偏移库
  unordered_map<int, pair<int, int>> _offsetLib;
  // 倒排索引库
  unordered_map<string, set<pair<int, double>>> _invertIndexLib;
  // 停用词
  unordered_set<string> _stopWords;
  // jieba
  SplitTool* _wordCutTool;
};

#endif  // !WEB_QUERY_H_
