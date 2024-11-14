#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

#include "nlohmann/json.hpp"
using std::string;

class Configuration {
  const char* const configFile = "../settings.json";

 public:
  Configuration();
  string get_dp_config(const char* purpose, const char* key);

 private:
  nlohmann::json _settings;
};

#endif  // !CONFIGURATION_H_
