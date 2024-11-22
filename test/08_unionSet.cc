#include "DictProducer.h"
#include "Dictionary.h"
#include "KeyRecommander.h"
#include "SplitToolCppJieba.h"

#include <iostream>
using std::cerr;

int main() {
  // SplitToolCppJieba jieba;
  // DictProducer dp("../settings.json", &jieba);
  // dp.buildEnDict();
  // dp.buildCnDict();
  // dp.createIndex();
  // dp.store();

  Dictionary::getInstance()->init();
  // Dictionary::getInstance()->doQuery("中国");
  KeyRecommander kr("你好中国");
  cerr << kr.doQuery("你好中国");
  cerr <<kr.doQuery("中国");
  cerr <<kr.doQuery("你好国");
  cerr <<kr.doQuery("你好中");
  cerr <<kr.doQuery("好中");
  cerr <<kr.doQuery("你中国");
  cerr <<kr.doQuery("你中国");
  cerr <<kr.doQuery("你好国");
  cerr <<kr.doQuery("好国");
  cerr <<kr.doQuery("");
  cerr <<kr.doQuery("你好中国");
  cerr <<kr.doQuery("你好中");
  cerr <<kr.doQuery("你好国");

  cerr <<kr.doQuery("你好中国");
  cerr <<kr.doQuery("你好中国");
  cerr <<kr.doQuery("你好中国");
  cerr <<kr.doQuery("你好中国");

  cerr <<kr.doQuery("你好中国");

  cerr <<kr.doQuery("你好中国");
  cerr <<kr.doQuery("你好中国");

  // kr.doQuery();
}
