#ifndef CACHE_MANAGER_H_
#define CACHE_MANAGER_H_

#include <pthread.h>

#include <vector>

#include "LRUCache.h"
using std::vector;

// 这是一个单例模式
class CacheManager {
 public:
  static CacheManager *getInstance();
  void init(const string &filename);
  LRUCache &getCache(const int &ppid);

 private:
  CacheManager();
  ~CacheManager();
  CacheManager(const CacheManager &) = delete;
  CacheManager operator=(const CacheManager &) = delete;

  static void destory();
  static void initReady();

 private:
  vector<LRUCache> _caches;
  static pthread_once_t _once;
  static CacheManager *_pInstance;
};

#endif  // !CACHE_MANAGER_H_
