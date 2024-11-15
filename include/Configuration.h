#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <pthread.h>

#include <string>

#include "nlohmann/json.hpp"
using std::string;

// 单例模式的配置类
class Configuration {
 public:
  static Configuration* getInstance();
  static string get_dp_config(const char* purpose, const char* key);

 private:
  Configuration();
  ~Configuration();
  static void initReady();
  Configuration(const string configFilePath);
  // 禁止拷贝构造和赋值
  Configuration(const Configuration& rhs) = delete;
  Configuration& operator=(const Configuration& rhs) = delete;

  static void destory();

 private:
  string _configFilePath;
  static nlohmann::json* _settings;
  static Configuration* _pInstance;
  static pthread_once_t _once;
};

#endif  // !CONFIGURATION_H_
