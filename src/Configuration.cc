#include "Configuration.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
using std::cerr;
using std::ifstream;
using std::unique_ptr;

Configuration::Configuration() : _settings() {
  ifstream ifs(configFile);
  if (!ifs.good()) {
    cerr << "open settings.json failed!\n";
    return;
  }
  auto fileSize = std::filesystem::file_size(configFile);
  unique_ptr<char[]> filePtr(new char[fileSize + 1]());
  ifs.read(filePtr.get(), fileSize);
  _settings = nlohmann::json::parse(filePtr.get());
  ifs.close();
}

string Configuration::get_config(const char* key) {
  string value = _settings["DictProducer"][key];
  cerr << value << "\n";
  if (value == "") {
    cerr << "settings.json is not complete!\n";
    return "1";
  } else {
    return value;
  }
}
