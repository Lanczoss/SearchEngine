#include <iostream>

#include "Configuration.h"
using std::cout;
int main() {
  // Configuration::getInstance()->read_config("");
  // Configuration::read_config("../settings.json");

  cout << Configuration::get_dp_config("yuliao", "chinese.directory") << '\n';
  cout << Configuration::get_dp_config("stop", "english") << '\n';
  cout << Configuration::get_dp_config("jieba", "dict") << "\n";
  cout << Configuration::get_dp_config("jieba", "hmm") << '\n';
  cout << Configuration::get_dp_config("jieba", "user") << '\n';
  cout << Configuration::get_dp_config("jieba", "idf") << '\n';
  cout << Configuration::get_dp_config("jieba", "stop") << '\n';
}
