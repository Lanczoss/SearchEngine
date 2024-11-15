#include "Dictionary.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "CandidateResult.h"
#include "Configuration.h"
using std::cerr;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

// 静态成员初始化
pthread_once_t Dictionary::_once = PTHREAD_ONCE_INIT;
Dictionary* Dictionary::_pInstance = getInstance();

Dictionary* Dictionary::getInstance() {
  pthread_once(&_once, initReady);
  return _pInstance;
}

void Dictionary::initReady() {
  _pInstance =
      new Dictionary(Configuration::getInstance()->dp("save", "dict"),
                     Configuration::getInstance()->dp("save", "index"));
  atexit(destory);
}

Dictionary::~Dictionary() {}

void Dictionary::destory() {
  if (_pInstance) {
    delete _pInstance;
    _pInstance = nullptr;
  }
}

Dictionary::Dictionary(const string& dictPath, const string& indexPath)
    : _dictPath(dictPath), _indexPath(indexPath) {}

void Dictionary::showDict() {
  for (size_t idx = 0; idx < 10; ++idx) {
    cerr << "string = " << _dict[idx].first << ", int = " << _dict[idx].second
         << "\n";
  }
  for (size_t idx = 40000; idx < 40020; ++idx) {
    cerr << "string = " << _dict[idx].first << ", int = " << _dict[idx].second
         << "\n";
  }
}

void Dictionary::showIndex() {
  int cnt = 10;
  for (auto& pair : _index) {
    cerr << "string = " << pair.first << ", ";
    for (auto& i : pair.second) {
      cerr << i << " ";
    }
    cerr << '\n';
    cnt--;
    if (cnt < 0) break;
  }
}

void Dictionary::init() {
  ifstream difs(_dictPath);
  if (!difs.good()) {
    cerr << "open dict.txt failed in Dictionary!\n";
    return;
  }

  ifstream iifs(_indexPath);
  if (!iifs.good()) {
    cerr << "open index.txt failed in Dictionary!\n";
    return;
  }

  string line;
  while (std::getline(difs, line)) {
    istringstream iss(line);
    string word;
    size_t frequency;
    iss >> word;
    // cerr << "word = " << word << "\n";
    iss >> frequency;
    _dict.push_back(std::make_pair(word, frequency));
  }
  // showDict();

  while (std::getline(iifs, line)) {
    istringstream iss(line);
    string word;
    iss >> word;
    size_t number;
    while (iss >> number) {
      _index[word].insert(number);
    }
  }
  // showIndex();

  difs.close();
  iifs.close();
}

// 用户输入一个关键词
// 返回十个关键词
// 使用set并集
// 使用最小编辑距离计算相似度
// 词频和相似度共同构成vector<string>
vector<CandidateResult> Dictionary::doQuery(const string& inputWord) {
  // 首先得到共同拥有inputWord的_index
  // 这里取需要是并集
  // set<int> unionSet;
  vector<int> unionSet;
  queryIndex(unionSet, inputWord);

  vector<pair<string, int>> sortFrequency;
  // 根据相似度排序，若相似度相等，用词频排序
  // 从_dict中取出并集
  // 存储需要排序的pair
  for (auto& index : unionSet) {
    sortFrequency.push_back(_dict[index]);
  }
  std::sort(sortFrequency.begin(), sortFrequency.end(),
            [inputWord, this](const pair<string, int>& lhs,
                              const pair<string, int>& rhs) {
              // 先算相似度
              int dlhs = distance(inputWord, lhs.first);
              int drhs = distance(inputWord, rhs.first);
              // cerr << "dlhs = " << dlhs << "\n";
              // cerr << "drhs = " << drhs << "\n";
              if (dlhs != drhs) {
                return dlhs < drhs;
              } else {
                // 如果相似度相等，用词频排序
                return lhs.second > rhs.second;
              }
            });

  vector<CandidateResult> readyVt;
  for (size_t idx = 0; idx < 10; ++idx) {
    CandidateResult cr(sortFrequency[idx].first, sortFrequency[idx].second,
                       distance(inputWord, sortFrequency[idx].first));
    readyVt.push_back(cr);
  }
  // showReadyVt(readyVt);
  return readyVt;
}

size_t Dictionary::nBytesCode(const char ch) {
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

void Dictionary::showUnion(vector<int>& set) {
  for (auto& i : set) {
    cerr << i << " ";
  }
  cerr << '\n';
}

void Dictionary::queryIndex(vector<int>& unionSet, const string& inputWord) {
  // 遍历单词
  // 区分英文还是中文
  bool flag = true;  // 如果是第一次
  for (size_t idx = 0; idx < inputWord.size();) {
    int count = nBytesCode(inputWord[idx]);
    // cerr << "count = " << count << ", idx = " << idx << '\n';
    string word = inputWord.substr(idx, count);
    auto temp = _index.find(word);
    if (temp != _index.end()) {
      // 与unionSet取交集
      if (flag == false) {
        vector<int> copySet = unionSet;
        unionSet.clear();
        std::set_intersection(copySet.begin(), copySet.end(),
                              temp->second.begin(), temp->second.end(),
                              std::back_inserter(unionSet));
      } else {
        for (auto& value : temp->second) {
          unionSet.push_back(value);
        }
        flag = false;
      }
    }
    idx += count;
  }
  // showUnion(unionSet);
}

void Dictionary::showReadyVt(vector<CandidateResult>& crv) {
  for (auto& cr : crv) {
    cerr << "string = " << cr._word << ", frequency = " << cr._freq
         << ", distance = " << cr._dist << '\n';
  }
}

int Dictionary::distance(const string& lhs, const string& rhs) {
  // 计算最小编辑距离-包括处理中英文
  size_t lhs_len = lhs.size();
  size_t rhs_len = rhs.size();
  vector<vector<int>> editDist(lhs_len + 1, vector<int>(rhs_len + 1, 0));
  for (size_t idx = 0; idx <= lhs_len; ++idx) {
    editDist[idx][0] = idx;
  }
  for (size_t idx = 0; idx <= rhs_len; ++idx) {
    editDist[0][idx] = idx;
  }
  std::string sublhs, subrhs;
  for (std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len;
       ++dist_i, ++lhs_idx) {
    size_t nBytes = nBytesCode(lhs[lhs_idx]);
    sublhs = lhs.substr(lhs_idx, nBytes);
    lhs_idx += (nBytes - 2);
    for (std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len;
         ++dist_j, ++rhs_idx) {
      nBytes = nBytesCode(rhs[rhs_idx]);
      subrhs = rhs.substr(rhs_idx, nBytes);
      rhs_idx += (nBytes - 2);
      if (sublhs == subrhs) {
        editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
      } else {
        editDist[dist_i][dist_j] = triple_min(
            editDist[dist_i][dist_j - 1] + 1, editDist[dist_i - 1][dist_j] + 1,
            editDist[dist_i - 1][dist_j - 1] + 1);
      }
    }
  }
  return editDist[lhs_len][rhs_len];
}

int Dictionary::triple_min(const int& a, const int& b, const int& c) {
  return a < b ? (a < c ? a : c) : (b < c ? b : c);
}
