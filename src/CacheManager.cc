#include "CacheManager.h"

#include "Configuration.h"
#include "LRUCache.h"

pthread_once_t CacheManager::_once = PTHREAD_ONCE_INIT;
CacheManager* CacheManager::_pInstance = getInstance();

CacheManager::CacheManager() {}
CacheManager::~CacheManager() {}

CacheManager* CacheManager::getInstance() {
  pthread_once(&_once, initReady);
  return _pInstance;
}

void CacheManager::initReady() {
  _pInstance = new CacheManager();
  atexit(destory);
}

void CacheManager::destory() {
  if (_pInstance) {
    delete _pInstance;
    _pInstance = nullptr;
  }
}

void CacheManager::init(const string& filename) {
  thread_local LRUCache lc(
      std::stoi(Configuration::getInstance()->cache("size")));
  lc.readFromFile(filename);
  // 记录在map中
  // 触发拷贝构造
  _caches.push_back(LRUCache(lc));
}
