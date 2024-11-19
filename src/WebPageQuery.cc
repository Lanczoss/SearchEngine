#include "WebPageQuery.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Configuration.h"
#include "SplitTool.h"
#include "SplitToolCppJieba.h"
#include "WebPage.h"

using std::cerr;
using std::ifstream;
using std::istringstream;

// 先读取去重网页偏移库
// 倒排索引库
// 停用词
WebPageQuery::WebPageQuery()
    : _offsetLib(), _invertIndexLib(), _wordCutTool(new SplitToolCppJieba()) {
  loadLib();
}

WebPageQuery::~WebPageQuery() { delete _wordCutTool; }

void WebPageQuery::loadLib() {
  // 读取去重网页偏移库
  ifstream ifs(Configuration::getInstance()->page("save", "doffset.lib"));
  if (!ifs) {
    cerr << "open doffset.dat failed!\n";
    return;
  }
  string line;
  while (std::getline(ifs, line)) {
    istringstream iss(line);
    int docid;
    int pos;
    int pageLength;
    iss >> docid;
    iss >> pos;
    iss >> pageLength;
    _offsetLib[docid] = std::make_pair(pos, pageLength);
  }
  ifs.close();
  // 读取倒排索引库
  ifstream iifs(Configuration::getInstance()->page("save", "invert.lib"));
  if (!iifs) {
    cerr << "open invert.dat failed!\n";
    return;
  }
  while (std::getline(iifs, line)) {
    istringstream iss(line);
    string word;
    iss >> word;
    int docid;
    double weight;
    while (iss >> docid >> weight) {
      // cerr << "string = " << word << ", docid = " << docid
      //      << ", weight = " << weight << '\n';
      _invertIndexLib[word].insert(std::make_pair(docid, weight));
    }
  }
  iifs.close();
  // 读取停用词
  ifstream sifs(Configuration::getInstance()->dp("stop", "chinese"));
  if (!sifs) {
    cerr << "open stop_words_cn failed!\n";
    return;
  }
  while (std::getline(sifs, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);  // 去掉末尾空白符
    _stopWords.insert(line);
  }
  sifs.close();
}

vector<WebPage> WebPageQuery::doQuery(const string &key) {
  // 先用jieba进行分词
  vector<string> jiebaWords = _wordCutTool->cut(key);
  // 分词后过滤停用词，并且算出在句子中的词频
  map<string, int> readyWords;
  for (auto &word : jiebaWords) {
    bool flag = true;
    // 如果单词非空
    if (!word.empty()) {
      // 过滤停用词
      if (flag && _stopWords.find(word) == _stopWords.end()) {
        // cerr << word << '\n';
        if (readyWords.find(word) == readyWords.end()) {
          // double temp = readyWords[word];
          // ++temp;
          ++readyWords[word];
        }
      }
    }
  }

  if (readyWords.empty()) {
    return vector<WebPage>();
  }
  // 取交集专用
  // 这里只取docid
  vector<int> readyDocids;
  // 取一个单词的集合准备进行交集工作
  for (auto &ready : _invertIndexLib[readyWords.begin()->first]) {
    readyDocids.push_back(ready.first);
  }

  // 计算一个句子的向量
  // 这里是文章总数加上句子数
  // 句子也当成一个page
  int pageSumNumber = _offsetLib.size();
  // 先算出每个关键词的权重系数
  // 组合成基准向量base
  vector<double> base;
  unordered_map<string, double> sentenceMap;
  // 现在有readyWords在句子中的词频有了
  // 现在有一个句子的所有权重的平方和
  // 归一化， 并且获得对应单词在不同网页的向量
  // 需要注意的是，这里的不同网页在不同单词中需要取交集
  // 第一个参数是docid，第二个参数是单词对应文章的向量
  // 遍历一个句子的所有单词
  for (auto &v : readyWords) {
    // 取出每个单词的索引集合
    vector<int> oneWordDocid;
    for (auto &oneWordSet : _invertIndexLib[v.first]) {
      oneWordDocid.push_back(oneWordSet.first);
    }
    // 这里要取交集
    vector<int> copyIntersection = readyDocids;
    readyDocids.clear();
    std::set_intersection(copyIntersection.begin(), copyIntersection.end(),
                          oneWordDocid.begin(), oneWordDocid.end(),
                          std::back_inserter(readyDocids));
  }
  for (auto &ready : readyWords) {
    // TF-IDF
    // 现在是一个单词
    // 需要单词在句子中的出现次数
    // 需要该单词在所有文章中出现的次数
    // 需要文章总数（包括句子）
    double oneWord = calWeight(ready.second, readyDocids.size(), pageSumNumber);
    oneWord *= ready.second;
    sentenceMap.insert({ready.first, oneWord});
  }
  double sentenceSquares = 0;
  for (auto &s : sentenceMap) {
    // 计算基准向量
    // 这里是算平方和
    sentenceSquares += s.second * s.second;
  }
  for (auto &ready : readyWords) {
    // 计算基准向量
    // 这里是归一化
    base.push_back(sentenceMap[ready.first] / sqrt(sentenceSquares));
  }
  // 现在得到了单词的docid交集
  // 取出每个docid的权重组成向量
  // 余弦相似算法计算相似度
  // 即排序标准为基准向量和向量集合每一个向量
  unordered_set<int> docidIntersection;
  for (auto &m : readyDocids) {
    docidIntersection.insert(m);
  }

  unordered_map<int, vector<double>> docidWeights;
  for (auto &word : readyWords) {
    for (auto &oneWordSet : _invertIndexLib[word.first]) {
      if (docidIntersection.find(oneWordSet.first) != docidIntersection.end()) {
        // 如果存在，说明是交集里面的
        // 取出来
        docidWeights[oneWordSet.first].push_back(oneWordSet.second);
      }
    }
  }

  // 根据相似度排序
  std::sort(
      readyDocids.begin(), readyDocids.end(),
      [base, &docidWeights](const int &lhs, const int &rhs) {
        // 比较两个docid的余弦值
        double lcosUpNumber = 0;
        double lcosDownFirstNumber = 0;
        double lcosDownSecondNumber = 0;
        double rcosUpNumber = 0;
        double rcosDownFirstNumber = 0;
        double rcosDownSecondNumber = 0;
        // 一个docid
        for (size_t idx = 0; idx < base.size(); ++idx) {
          lcosUpNumber += base[idx] * docidWeights[lhs][idx];
          lcosDownFirstNumber += base[idx] * base[idx];
          lcosDownSecondNumber +=
              docidWeights[lhs][idx] * docidWeights[lhs][idx];
          rcosUpNumber += base[idx] * docidWeights[rhs][idx];
          rcosDownFirstNumber += base[idx] * base[idx];
          rcosDownSecondNumber +=
              docidWeights[rhs][idx] * docidWeights[rhs][idx];
        }
        // 计算最终值
        double dlhs = lcosUpNumber / lcosDownFirstNumber * lcosDownSecondNumber;
        double drhs = rcosUpNumber / rcosDownFirstNumber * rcosDownSecondNumber;
        return dlhs > drhs;
      });

  cerr << "readyDocids = " << readyDocids.size() << '\n';
  // 返回前十个
  vector<WebPage> readyWebPages;
  for (size_t idx = 0; idx < 10 && idx < readyDocids.size(); ++idx) {
    WebPage wp(Configuration::getInstance()->page("save", "dpage.lib"));
    wp.processDoc(_offsetLib[readyDocids.at(idx)].first,
                  _offsetLib[readyDocids.at(idx)].second);
    readyWebPages.push_back(wp);
  }
  return readyWebPages;
}

double WebPageQuery::calWeight(const int &freqInOnePage, const int &pageNumber,
                               const int &pageSumNumber) {
  // IDF = log2(N/(DF+1))
  double IDF =
      log2(((double)pageSumNumber / (double)pageNumber + 1) + (double)1);
  return IDF * freqInOnePage;  // IDF * TF
}

size_t WebPageQuery::nBytesCode(const char ch) {
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
