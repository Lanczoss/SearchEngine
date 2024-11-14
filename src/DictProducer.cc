#include "DictProducer.h"

#include <dirent.h>
#include <string.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
using std::cerr;
using std::ifstream;
using std::istringstream;
using std::ofstream;
using std::unique_ptr;

DictProducer::DictProducer(const string file, SplitTool *tool)
    : _files(), _dict(), _index(), _config(), _cuttor(tool) {
  // 遍历yuliao文件夹里的art文件夹的所有txt文件
  // 先用C3-Art0019.txt做测试
  // 第一个保证是英文文件
  // vector<string>后面的全部都是中文语料
  _files.push_back(file);
  _files.push_back("../yuliao/C3-Art0019.txt");
  // DIR *dirp =
  // opendir(_config->get_config(string("chinese_yuliao_diractory"))); if (dirp
  // == nullptr) {
  //   perror("opendir");
  //   return;
  // }
  // struct dirent *dp;
  // while ((dp = readdir(dirp)) != nullptr) {
  //   if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
  //   } else {
  //     // cout << "filename = " << dp->d_name << "\n";
  //     //
  //     _files.push_back(CnDir + string(dp->d_name));
  //   }
  // }
  // closedir(dirp);
}

void DictProducer::buildEnDict() {
  ifstream ifs(_files.front());
  if (!ifs.good()) {
    cerr << "open english yuliao failed!\n";
    return;
  }

  // 读取停用词文件
  ifstream ifsStop(_config->get_config("stop_words_eng"));
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
      if (stopWords.find(word) == stopWords.end() && word != "\0") {
        // 统计词频
        ++dictInt[word];
      }
    }
  }

  // 统计完map后转成vector
  for (auto &w : dictInt) {
    _dict.push_back(std::make_pair(w.first, w.second));
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

void DictProducer::buildCnDict() {
  // 从_files的第二个元素开始（下标为1）全都是中文语料
  // 测试时只有一个文件
  // 读取中文停用词文件
  ifstream ifsStop(stopWordsCn);
  if (!ifsStop.good()) {
    cerr << "open stop_words_cn failed!\n";
    return;
  }

  map<string, int> dictInt;
  set<string> stopWords;
  string line;
  while (std::getline(ifsStop, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);  // 去掉末尾空白符
    stopWords.insert(line);
  }

  for (auto idx = 1; idx < _files.size(); ++idx) {
    ifstream ifs(_files[idx]);
    if (!ifs.good()) {
      cerr << "open one chinese yuliao failed!\n";
      return;
    }
    auto fileSize = std::filesystem::file_size(_files[idx]);
    unique_ptr<char[]> fileStr(new char[fileSize + 1]());
    ifs.read(fileStr.get(), fileSize);
    // cerr << fileStr.get();

    // 清洗
    string readyStr = cleanFileStr(fileStr.get(), fileSize + 1);
    vector<string> words = _cuttor->cut(readyStr);

    // 切割完后转为进行词频统计
    for (auto &word : words) {
      if (stopWords.find(word) == stopWords.end() && word != "\0") {
        ++dictInt[word];
      }
    }

    // for (auto &word : dictInt) {
    //   cerr << "string = " << word.first << ", int = " << word.second <<
    //   "\n";
    // }

    // 词频统计后转为vector
    for (auto &w : dictInt) {
      _dict.push_back(std::make_pair(w.first, w.second));
    }

    ifs.close();
  }
}

// 用于中文语料清洗的函数
string DictProducer::cleanFileStr(const char *fileStr, size_t length) {
  string str(fileStr);
  string readyStr;
  // readyStr.resize(length, 0);

  size_t ridx = 0;
  for (size_t idx = 0; idx < str.size();) {
    // 需要清洗掉\n
    int count = nBytesCode(str[idx]);
    if (count == 1) {
      // 排除一个字节的字符
      idx += count;
    } else {
      string word = str.substr(idx, count);
      // cerr << "word = " << word << ", count = " << count << "\n";
      readyStr.insert(ridx, word);
      ridx += count;
      idx += count;
    }
  }

  // cerr << "readyStr = " << readyStr << "\n";
  // readyStr.shrink_to_fit();
  return readyStr;
}

size_t DictProducer::nBytesCode(const char ch) {
  if (ch & (1 << 7)) {
    int nBytes = 1;
    for (int idx = 0; idx != 6; ++idx) {
      if (ch & (1 << (6 - idx))) {
        ++nBytes;
      } else
        break;
    }
    return nBytes;
  }
  return 1;
}

void DictProducer::createIndex() {
  // 遍历vector<pair<string, int>>
  // 装入_index (map<string, set<int>>)
  for (size_t idx = 0; idx < _dict.size(); ++idx) {
    string word = _dict[idx].first;
    for (size_t sidx = 0; sidx < word.size();) {
      int count = nBytesCode(word[sidx]);
      if (count == 1) {
        // 英文
        string readyEn = word.substr(sidx, count);
        _index[readyEn].insert(idx);
        sidx += count;
      } else {
        // 中文
        string readyCn = word.substr(sidx, count);
        _index[readyCn].insert(idx);
        sidx += count;
      }
    }
  }
}

void DictProducer::store() {
  // 词典的持久化存储
  ofstream dofs("../dict/dict.txt");
  if (!dofs) {
    cerr << "create dict.txt failed!";
    return;
  }

  for (auto &dict : _dict) {
    dofs << dict.first << " " << dict.second << "\n";
  }

  // 单词位置索引的持久化存储
  ofstream iofs("../dict/index.txt");
  if (!iofs) {
    cerr << "create index.txt failed!";
    return;
  }

  for (auto &index : _index) {
    iofs << index.first << " ";
    for (auto &set : index.second) {
      iofs << set << " ";
    }
    iofs << "\n";
  }

  dofs.close();
  iofs.close();
}
