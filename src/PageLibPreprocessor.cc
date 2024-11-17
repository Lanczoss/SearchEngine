#include "PageLibPreprocessor.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Configuration.h"
#include "WebPage.h"
using std::cerr;
using std::ifstream;
using std::istringstream;
using std::ostream;

PageLibPreprocessor::PageLibPreprocessor()
    : _offsetLib(), _pageLib(), _invertIndexLib() {
  readOffsetLib();
  cutRedundantPage();
  storeOnDisk();
}

void PageLibPreprocessor::cutRedundantPage() {
  // 使用谷歌simhash计算所有页面的指纹
  // 先读取网页库
  ifstream ifs(Configuration::getInstance()->page("save", "page.lib"));
  if (!ifs) {
    cerr << "open page.dat failed!\n";
    return;
  }
  ofstream pofs(Configuration::getInstance()->page("save", "dpage.lib"));
  if (!pofs) {
    cerr << "create dpage.dat failed!\n";
    return;
  }

  Simhasher hasher(Configuration::getInstance()->dp("jieba", "dict"),
                   Configuration::getInstance()->dp("jieba", "hmm"),
                   Configuration::getInstance()->dp("jieba", "user"),
                   Configuration::getInstance()->dp("jieba", "idf"),
                   Configuration::getInstance()->dp("jieba", "stop"));

  WebPage wp;
  vector<int> earseDocID;
  for (auto& offset : _offsetLib) {
    wp.clear();
    wp.processDoc(offset.second.first, offset.second.second);
    uint64_t u64;
    hasher.make(wp.getDocContent(), 5, u64);
    // cerr << "frigerprint = " << u64 << '\n';
    // 将指纹加入到
    auto check = _pageLib.insert(u64);
    if (check.second) {
      // 如果返回插入成功
      pofs << wp.getDoc();
    } else if (wp.getDocID() != -1) {
      cerr << "erase = " << wp.getDocID() << "\n";
      earseDocID.push_back(wp.getDocID());
    }
  }

  for (auto& e : earseDocID) {
    _offsetLib.erase(e);
  }
  pofs.close();
  ifs.close();
}

void PageLibPreprocessor::readOffsetLib() {
  // 读取所有的网页偏移
  ifstream ifs(Configuration::getInstance()->page("save", "offset.lib"));
  if (!ifs) {
    cerr << "open offset.dat failed!\n";
    return;
  }

  string line;
  while (std::getline(ifs, line)) {
    istringstream iss(line);
    int index;
    int pos;
    int length;
    iss >> index;
    iss >> pos;
    iss >> length;
    _offsetLib.insert(std::make_pair(index, std::make_pair(pos, length)));
  }
  ifs.close();
}

void PageLibPreprocessor::storeOnDisk() {
  // 现在的两个数据成员是去重后的效果
  // 存储新的网络偏移库
  ofstream ofs(Configuration::getInstance()->page("save", "doffset.lib"));
  if (!ofs) {
    cerr << "create doffset.dat failed!\n";
    return;
  }

  for (auto& offset : _offsetLib) {
    ofs << offset.first << " " << offset.second.first << " "
        << offset.second.second << '\n';
  }

  ofs.close();
}
