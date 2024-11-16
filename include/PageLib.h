#ifndef PAGE_LIB_H_
#define PAGE_LIB_H_

#include <fstream>
#include <map>
using std::ofstream;

#include "DirScanner.h"
#include "tinyxml2.h"
using std::map;
using std::pair;

struct PageMsg {
  size_t _docid;
  string _title;
  string _link;
  string _content;
};

class PageLib {
 public:
  PageLib();
  void create();

 private:
  void createWebLib();
  void createOffsetLib();
  void regexExecute(string &);
  void showTwoPage();

  void store(ofstream &, PageMsg &, size_t &);

 private:
  DirScanner _dirScanner;
  vector<string> _pages;
  map<int, pair<int, int>> _offsetLib;
};

#endif  // !PAGE_LIB_H_
