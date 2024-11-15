#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <pthread.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "CandidateResult.h"
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

class Dictionary {
 public:
  static Dictionary *getInstance();
  vector<CandidateResult> doQuery(const string &);
  void init();

 private:
  void queryIndex(vector<int> &, const string &);
  int distance(const string &inputWord, const string &candidate);
  int triple_min(const int &, const int &, const int &);

  void showDict();
  void showIndex();
  void showReadyVt(vector<CandidateResult> &);

  size_t nBytesCode(const char);
  void showUnion(vector<int> &);

 private:
  Dictionary();
  Dictionary(const string &, const string &);
  ~Dictionary();
  Dictionary(const Dictionary &rhs) = delete;
  Dictionary &operator=(const Dictionary &rhs) = delete;

  // 静态成员函数给pthread_once调用
  static void initReady();

  static void destory();

 private:
  string _dictPath;
  string _indexPath;
  vector<pair<string, int>> _dict;
  map<string, set<int>> _index;
  static pthread_once_t _once;
  static Dictionary *_pInstance;
};

#endif  // !DICTIONARY_H_
