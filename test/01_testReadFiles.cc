// #include <fstream>
// #include <iostream>
// using std::cerr;

#include "DictProducer.h"
int main() {
  DictProducer dp("./yuliao/The_Holy_Bible.txt");
  dp.buildEnDict();
  // 读取停用词文件
  // std::ifstream ifsStop("yuliao/stop_words_eng.txt");
  // if (!ifsStop.good()) {
  //   cerr << "open stop_words_eng failed!\n";
  //   return -1;
  // }
  //
  // set<string> stopWords;
  // string line;
  // int cnt = 10;
  // while (std::getline(ifsStop, line)) {
  //   line.erase(line.find_last_not_of(" \n\r\t") + 1);  // 去掉末尾空白符
  //   // line.erase(0, line.find_first_not_of(" \n\r\t"));  // 去掉开头空白符
  //   for (auto &ch : line) {
  //     if (!isalpha(ch)) {
  //       // 不是英文字母
  //       ch = ' ';
  //     } else if (isupper(ch)) {
  //       ch = tolower(ch);
  //     }
  //   }
  //   cerr << line << "\n";
  //   stopWords.insert(line);
  //   cnt--;
  //   if (cnt < 0) {
  //     break;
  //   }
  // }
  //
  // cerr << (stopWords.find("a") == stopWords.end());
}
