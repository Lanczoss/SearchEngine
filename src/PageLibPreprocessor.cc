#include "PageLibPreprocessor.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Configuration.h"
#include "SplitTool.h"
#include "WebPage.h"
using std::cerr;
using std::ifstream;
using std::istringstream;
using std::log2;
using std::ostream;

PageLibPreprocessor::PageLibPreprocessor(SplitTool* tool)
    : _offsetLib(), _pageLib(), _invertIndexLib(), _wordCutter(tool) {
  readOffsetLib();
  cerr << "去重开始\n";
  cutRedundantPage();
  cerr << "建立倒排索引开始\n";
  buildInvertIndexMap();
  cerr << "建立倒排索引结束\n";
  storeOnDisk();
}

void PageLibPreprocessor::cutRedundantPage() {
  // 使用谷歌simhash计算所有页面的指纹
  // 先读取网页库
  ifstream ifs(Configuration::getInstance()->page("save", "page.lib"));
  if (!ifs) {
    cerr << "open page.dat failed!\n";
    return;
  }
  ofstream pofs(Configuration::getInstance()->page("save", "dpage.lib"));
  if (!pofs) {
    cerr << "create dpage.dat failed!\n";
    return;
  }

  Simhasher hasher(Configuration::getInstance()->dp("jieba", "dict"),
                   Configuration::getInstance()->dp("jieba", "hmm"),
                   Configuration::getInstance()->dp("jieba", "user"),
                   Configuration::getInstance()->dp("jieba", "idf"),
                   Configuration::getInstance()->dp("jieba", "stop"));

  WebPage wp(Configuration::getInstance()->page("save", "page.lib"));
  vector<int> earseDocID;
  size_t position = 0;
  for (auto& offset : _offsetLib) {
    wp.clear();
    wp.processDoc(offset.second.first, offset.second.second);
    uint64_t u64;
    hasher.make(wp.getDocContent(), 5, u64);
    // cerr << "frigerprint = " << u64 << '\n';
    // 将指纹加入到
    auto check = _pageLib.insert(u64);
    if (check.second) {
      // 如果返回插入成功
      pofs << wp.getDoc();
      // 生成新的偏移
      offset.second.first = position;
      offset.second.second = wp.getDoc().size();
      position += wp.getDoc().size();
    } else if (wp.getDocID() != -1) {
      // cerr << "erase = " << wp.getDocID() << "\n";
      earseDocID.push_back(wp.getDocID());
    }
  }

  for (auto& e : earseDocID) {
    _offsetLib.erase(e);
  }
  pofs.close();
  ifs.close();
}

void PageLibPreprocessor::readOffsetLib() {
  // 读取所有的网页偏移
  ifstream ifs(Configuration::getInstance()->page("save", "offset.lib"));
  if (!ifs) {
    cerr << "open offset.dat failed!\n";
    return;
  }

  string line;
  while (std::getline(ifs, line)) {
    istringstream iss(line);
    int index;
    int pos;
    int length;
    iss >> index;
    iss >> pos;
    iss >> length;
    _offsetLib.insert(std::make_pair(index, std::make_pair(pos, length)));
  }
  ifs.close();
}

void PageLibPreprocessor::buildInvertIndexMap() {
  // 先遍历网页库
  // 对文章全文进行分词
  // 得出一个单词在文章的id，以及在文章中的权重
  // 注意过滤停用词
  // 读取中文停用词文件
  ifstream ifsStop(Configuration::getInstance()->dp("stop", "chinese"));
  if (!ifsStop.good()) {
    cerr << "open stop_words_cn failed!\n";
    return;
  }

  set<string> stopWords;
  string line;
  while (std::getline(ifsStop, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);  // 去掉末尾空白符
    stopWords.insert(line);
  }
  // 存储单词，包含该单词文章的集合，文章id，在文章中出现的次数
  map<string, set<pair<int, int>>> calculateMap;
  WebPage wp(Configuration::getInstance()->page("save", "dpage.lib"));
  // 遍历偏移库
  for (auto& offset : _offsetLib) {
    // 现在是一篇文章
    wp.clear();
    wp.processDoc(offset.second.first, offset.second.second);
    // 使用jieba进行分词
    vector<string> cutWords = _wordCutter->cut(wp.getDocContent());

    // 过滤之后的存储
    map<string, int> freq;
    // 过滤停用词
    for (auto& word : cutWords) {
      if (stopWords.find(word) == stopWords.end()) {
        // 不是停用词
        // 这里可以做筛选
        // 存储到map数据结构
        if (!word.empty()) {
          bool flag = true;
          for (size_t idx = 0; idx < word.size();) {
            // 需要清洗掉\n
            int count = nBytesCode(word[idx]);
            if (count == 1) {
              // 排除一个字节的字符
              flag = false;
              break;
            } else {
              idx += count;
            }
          }
          if (flag) {
            ++freq[word];
          }
        }
      }
    }
    for (auto& f : freq) {
      calculateMap[f.first].insert(std::make_pair(wp.getDocID(), f.second));
    }
  }

  // 计算权重
  // 需要某个单词在一篇文章中出现的次数
  // 需要包含该单词的文章数量
  // 文章总数
  int pageSumNumber = _offsetLib.size();
  unordered_map<string, set<pair<int, double>>> weightMap;
  // 某文章中所有单词权重的平方和
  unordered_map<int, double> wSquaresSum;
  // 遍历一篇文章的所有单词
  // 计算他们的权重
  for (auto& cal : calculateMap) {
    // 一个单词的文章集合
    for (auto& set : cal.second) {
      // 一个文章id
      double temp = calWeight(set.second, cal.second.size(), pageSumNumber);
      weightMap[cal.first].insert(std::make_pair(set.first, temp));
      wSquaresSum[set.first] += temp * temp;
    }
  }
  // 归一化
  for (auto& cal : weightMap) {
    for (auto& set : cal.second) {
      double temp = set.second;
      temp /= sqrt(wSquaresSum[set.first]);
      _invertIndexLib[cal.first].insert(std::make_pair(set.first, temp));
    }
  }
}

double PageLibPreprocessor::calWeight(const int& freqInOnePage,
                                      const int& pageNumber,
                                      const int& pageSumNumber) {
  // IDF = log2(N/(DF+1))
  double IDF =
      log2(((double)pageSumNumber / (double)pageNumber + 1) + (double)1);
  return IDF * freqInOnePage;  // IDF * TF
}

void PageLibPreprocessor::storeOnDisk() {
  // 现在的两个数据成员是去重后的效果
  // 存储新的网络偏移库
  ofstream ofs(Configuration::getInstance()->page("save", "doffset.lib"));
  if (!ofs) {
    cerr << "create doffset.dat failed!\n";
    return;
  }
  for (auto& offset : _offsetLib) {
    ofs << offset.first << " " << offset.second.first << " "
        << offset.second.second << '\n';
  }
  ofs.close();
  ofstream oofs(Configuration::getInstance()->page("save", "invert.lib"));
  if (!oofs) {
    cerr << "create invert.dat failed!\n";
    return;
  }
  for (auto& i : _invertIndexLib) {
    oofs << i.first << " ";
    for (auto& s : i.second) {
      oofs << s.first << " " << s.second << " ";
    }
    oofs << '\n';
  }
  oofs.close();
}

size_t PageLibPreprocessor::nBytesCode(const char ch) {
  if (ch & (1 << 7)) {
    int nBytes = 1;
    for (int idx = 0; idx != 6; ++idx) {
      if (ch & (1 << (6 - idx))) {
        ++nBytes;
      } else
        break;
    }
    return nBytes;
  }
  return 1;
}
