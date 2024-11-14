#include "Configuration.h"

#include <fstream>
#include <iostream>
using std::cerr;
using std::ifstream;

Configuration::Configuration() : _settings() {
  ifstream ifs(configFile);
  if (!ifs.good()) {
    cerr << "open settings.json failed!\n";
    return;
  }
  // cerr << filePtr.get() << "\n";
  _settings = nlohmann::json::parse(ifs);
  // cerr << _settings.dump();
  ifs.close();
}

string Configuration::get_dp_config(const char* purpose, const char* key) {
  auto value = _settings["DictProducer"][purpose][key];
  if (value == nullptr) {
    cerr << "settings.json is not correct!\n";
    return "";
  } else {
    return value;
  }
}
