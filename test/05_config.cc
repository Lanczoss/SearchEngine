#include <iostream>

#include "Configuration.h"
using std::cout;
int main() {
  // Configuration::getInstance()->read_config("");
  // Configuration::read_config("../settings.json");

  cout << Configuration::get_dp_config("yuliao", "chinese.directory") << '\n';
  cout << Configuration::get_dp_config("stop", "english") << '\n';
}
