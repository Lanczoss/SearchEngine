#include "RedisServer.h"

#include "Configuration.h"

RedisServer::RedisServer()
    : _redis("tcp://" + Configuration::getInstance()->cache("redis.ip") + ":" +
             Configuration::getInstance()->cache("redis.port")) {}

void RedisServer::put(const string &title, const string &json) {
  // redis
  _redis.set(title, json);
}

// 找不到返回false
bool RedisServer::get(const string &title, string &value) {
  auto getV = _redis.get(title);
  if (getV) {
    value = *getV;
    return true;
  } else {
    return false;
  }
}
