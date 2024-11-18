#include <fstream>
#include <iostream>

#include "Configuration.h"
using std::cerr;
using std::ifstream;

int main() {
  ifstream ifs(Configuration::getInstance()->page("save", "dpage.lib"));
  if (!ifs) {
    return -1;
  }
  ifs.seekg(17040477);
  char str[13561] = {0};
  ifs.read(str, 13560);
  cerr << str;
  ifs.close();
}
