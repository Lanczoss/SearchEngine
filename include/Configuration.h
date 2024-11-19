#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <pthread.h>

#include <string>
#include <vector>

#include "CandidateResult.h"
#include "WebPage.h"
#include "nlohmann/json.hpp"
using std::string;
using std::vector;

// 单例模式的配置类
class Configuration {
 public:
  static Configuration* getInstance();
  string dp(const char* purpose, const char* key);
  string network(const char* purpose, const char* key);
  string page(const char* purpose, const char* key);
  string page(const char* key);
  string cache(const char* purpose, const char* key);
  string cache(const char* key);
  void init(const string& configFilePath);

  string createPages(vector<WebPage>);
  string createWords(vector<string>);

 private:
  Configuration();
  ~Configuration();
  static void initReady();
  Configuration(const string& configFilePath);
  // 禁止拷贝构造和赋值
  Configuration(const Configuration& rhs) = delete;
  Configuration& operator=(const Configuration& rhs) = delete;

  static void destory();

 private:
  string _configFilePath;
  nlohmann::json _settings;
  static Configuration* _pInstance;
  static pthread_once_t _once;
};

#endif  // !CONFIGURATION_H_
