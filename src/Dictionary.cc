#include "Dictionary.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Configuration.h"
using std::cerr;
using std::ifstream;
using std::istringstream;

// 静态成员初始化
pthread_once_t Dictionary::_once = PTHREAD_ONCE_INIT;
Dictionary* Dictionary::_pInstance = getInstance();

Dictionary* Dictionary::getInstance() {
  pthread_once(&_once, initReady);
  return _pInstance;
}

void Dictionary::initReady() {
  _pInstance = new Dictionary(Configuration::get_dp_config("save", "dict"),
                              Configuration::get_dp_config("save", "index"));
  atexit(destory);
}

Dictionary::~Dictionary() {}

void Dictionary::destory() {
  if (_pInstance) {
    delete _pInstance;
    _pInstance = nullptr;
  }
}

Dictionary::Dictionary(const string& dictPath, const string& indexPath)
    : _dictPath(dictPath), _indexPath(indexPath) {}

void Dictionary::showDict() {
  for (size_t idx = 0; idx < 10; ++idx) {
    cerr << "string = " << _dict[idx].first << ", int = " << _dict[idx].second
         << "\n";
  }
  for (size_t idx = 40000; idx < 40020; ++idx) {
    cerr << "string = " << _dict[idx].first << ", int = " << _dict[idx].second
         << "\n";
  }
}

void Dictionary::showIndex() {
  int cnt = 10;
  for (auto& pair : _index) {
    cerr << "string = " << pair.first << ", ";
    for (auto& i : pair.second) {
      cerr << i << " ";
    }
    cerr << '\n';
    cnt--;
    if (cnt < 0) break;
  }
}

void Dictionary::init() {
  ifstream difs(_dictPath);
  if (!difs.good()) {
    cerr << "open dict.txt failed in Dictionary!\n";
    return;
  }

  ifstream iifs(_indexPath);
  if (!iifs.good()) {
    cerr << "open index.txt failed in Dictionary!\n";
    return;
  }

  string line;
  while (std::getline(difs, line)) {
    istringstream iss(line);
    string word;
    size_t frequency;
    iss >> word;
    // cerr << "word = " << word << "\n";
    iss >> frequency;
    _dict.push_back(std::make_pair(word, frequency));
  }
  // showDict();

  while (std::getline(iifs, line)) {
    istringstream iss(line);
    string word;
    iss >> word;
    size_t number;
    while (iss >> number) {
      _index[word].insert(number);
    }
  }
  // showIndex();

  difs.close();
  iifs.close();
}
