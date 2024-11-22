#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_

#include <list>
#include <string>
#include <thread>
#include <unordered_map>
using std::list;
using std::pair;
using std::string;
using std::unordered_map;

class LRUCache {
 public:
  LRUCache(int ppid, int number = 100);
  LRUCache(const LRUCache &rhs);

  void addElement(const string &key, const string &value);
  void update(const LRUCache &rhs);

  // private:
  void writeToFile(const string &filename);
  void readFromFile(const string &filename);

 private:
  // 存json
  list<pair<string, string>> _resultList;
  // 以查询词hash
  unordered_map<string, list<pair<string, string>>::iterator> _hashMap;
  // 里面存的是同步所需信息
  list<string> _pendingUpdateList;
  int _capacity;
  int _ppid;
};

#endif  // !LRU_CACHE_H_
