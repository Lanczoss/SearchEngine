#include <iostream>

#include "SplitToolCppJieba.h"
#include "WebPageQuery.h"
using std::cerr;

int main() {
  SplitToolCppJieba jieba;
  WebPageQuery wpq(&jieba);
  vector<WebPage> v = wpq.doQuery("你好中国");
  for (auto& s : v) {
    cerr << s.getDoc();
  }
}
