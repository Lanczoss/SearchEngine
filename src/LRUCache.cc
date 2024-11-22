#include "LRUCache.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Configuration.h"

using std::cerr;
using std::ifstream;
using std::istringstream;
using std::ofstream;

LRUCache::LRUCache(int ppid, int number)
    : _resultList(),
      _hashMap(),
      _pendingUpdateList(),
      _capacity(number),
      _ppid(ppid) {}

LRUCache::LRUCache(const LRUCache &rhs)
    : _resultList(rhs._resultList),
      _hashMap(rhs._hashMap),
      _pendingUpdateList(rhs._pendingUpdateList),
      _capacity(rhs._capacity) {}

void LRUCache::readFromFile(const string &filename) {
  // 从文件中读
  ifstream ifs(filename);
  if (!ifs) {
    cerr << "create/open server.cache.dat failed!\n";
    return;
  }
  string line;
  while (std::getline(ifs, line)) {
    istringstream iss(line);
    string title;
    string json;
    iss >> title;
    iss >> json;
    _resultList.push_back(std::make_pair(title, json));
    _hashMap.insert({title, _resultList.end()});
  }
  ifs.close();
}

void LRUCache::addElement(const string &key, const string &json) {
  // 往缓存添加数据
  auto it = _hashMap.find(key);
  if (it != _hashMap.end()) {
    // 如果存在这个节点
    _resultList.erase(it->second);
    _hashMap.erase(it);
  }
  // 在前面插入
  _resultList.push_front({key, json});
  _hashMap.insert({key, _resultList.begin()});
  // pending记录插入的元素key
  _pendingUpdateList.push_back(key);

  if (_resultList.size() > _capacity) {
    _hashMap.erase(_resultList.back().first);
    _resultList.pop_back();
  }
}

void LRUCache::update(const LRUCache &rhs) {}

void LRUCache::writeToFile(const string &filename) {
  // 将信息写回文件
  ofstream ofs(filename);
  if (!ofs) {
    cerr << "create/open server.cache.dat failed!\n";
    return;
  }
  for (auto &pair : _resultList) {
    ofs << pair.first << " " << pair.second << '\n';
  }
  ofs.close();
}
