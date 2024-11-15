#include "Configuration.h"

#include <fstream>
#include <iostream>
using std::cerr;
using std::ifstream;

// 对静态数据成员的定义
// nlohmann::json* Configuration::_settings = nullptr;
Configuration* Configuration::_pInstance = getInstance();
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;

Configuration* Configuration::getInstance() {
  pthread_once(&_once, initReady);
  return _pInstance;
}

Configuration::~Configuration() {}

void Configuration::initReady() {
  // _settings = new nlohmann::json();
  _pInstance = new Configuration("../settings.json");
  atexit(destory);
}

Configuration::Configuration(const string& configFilePath)
    : _configFilePath(configFilePath) {
  ifstream ifs(_configFilePath);
  if (!ifs.good()) {
    cerr << "open settings.json failed!\n";
    return;
  }
  // cerr << filePtr.get() << "\n";
  _settings = nlohmann::json::parse(ifs);
  // cerr << _settings.dump();
  ifs.close();
}

string Configuration::dp(const char* purpose, const char* key) {
  auto value = _settings["dict.producer"][purpose][key];
  if (value == nullptr) {
    cerr << "settings.json is not correct!\n";
    return "wrong";
  } else {
    return value;
  }
}

void Configuration::destory() {
  if (_pInstance) {
    delete _pInstance;
    _pInstance = nullptr;
  }
  // if (_settings) {
  //   delete _settings;
  //   _settings = nullptr;
  // }
}

void Configuration::init(const string& configFilePath) {
  _configFilePath = configFilePath;
  ifstream ifs(_configFilePath);
  if (!ifs.good()) {
    cerr << "open settings.json failed!\n";
    return;
  }
  // cerr << filePtr.get() << "\n";
  _settings = nlohmann::json::parse(ifs);
  // cerr << _settings.dump();
  ifs.close();
}

string Configuration::network(const char* purpose, const char* key) {
  auto value = _settings["network"][purpose][key];
  if (value == nullptr) {
    cerr << "settings.json is not correct!\n";
    return "wrong";
  } else {
    return value;
  }
}
