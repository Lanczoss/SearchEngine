#include <iostream>

#include "Configuration.h"
using std::cout;
int main() {
  Configuration conf;

  cout << conf.get_dp_config("yuliao", "chinese.directory") << '\n';
  cout << conf.get_dp_config("stop", "english") << '\n';
}
