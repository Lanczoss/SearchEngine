#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <pthread.h>

#include <map>
#include <set>
#include <string>
#include <vector>
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

class Dictionary {
 public:
  static Dictionary *getInstance();
  vector<string> doQuery(const string &);
  void init();

 private:
  void queryIndex();
  int distance(string candidate);
  void showDict();
  void showIndex();

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
