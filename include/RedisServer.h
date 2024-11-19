#ifndef REDIS_SERVER_H_
#define REDIS_SERVER_H_

#include <sw/redis++/redis++.h>

using namespace sw::redis;

#include <string>
using std::string;

class RedisServer {
 public:
  RedisServer();
  void put(const string &title, const string &json);

  // 找不到返回false
  bool get(const string &title, string &value);

 private:
  int _capacity;
  Redis _redis;
};

#endif  // !REDIS_SERVER_H_
