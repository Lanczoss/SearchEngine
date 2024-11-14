#include "DictProducer.h"

#include <fstream>
#include <iostream>
#include <sstream>
using std::cerr;
using std::ifstream;
using std::istringstream;

DictProducer::DictProducer(string file) : _files(), _dict() {
  // 遍历yuliao文件夹里的art文件夹的所有txt文件
  // 先用C3-Art0019.txt做测试
  // 第一个保证是英文文件
  // vector<string>后面的全部都是中文语料
  _files.push_back(file);
  _files.push_back("../yuliao/C3-Art0019.txt");
}

void DictProducer::buildEnDict() {
  ifstream ifs(_files.front());
  if (!ifs.good()) {
    cerr << "open english yuliao failed!\n";
    return;
  }

  // 读取停用词文件
  ifstream ifsStop(stopWordsEng);
  if (!ifsStop.good()) {
    cerr << "open stop_words_eng failed!\n";
    return;
  }

  set<string> stopWords;
  string line;
  while (std::getline(ifsStop, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);  // 去掉末尾空白符
    stopWords.insert(line);
  }

  map<string, int> dictInt;
  while (std::getline(ifs, line)) {
    // 将该行所有符号转成空格，大写转小写
    cleanLine(line);

    istringstream iss(line);
    string word;
    while (iss >> word) {
      if (stopWords.find(word) == stopWords.end()) {
        // 统计词频
        ++dictInt[word];
      }
    }
  }

  // 统计完map后转成vector
  for (auto &w : dictInt) {
    _dict.push_back(std::make_pair(w.first, w.second));
  }
  // print
  for (auto idx = 0; idx < 10; ++idx) {
    cerr << "string = " << _dict[idx].first << ", int = " << _dict[idx].second
         << "\n";
  }
}

// 用于英文语料清洗的函数
void DictProducer::cleanLine(string &line) {
  for (auto &ch : line) {
    if (!isalpha(ch)) {
      // 不是英文字母
      ch = ' ';
    } else if (isupper(ch)) {
      ch = tolower(ch);
    }
  }
}
