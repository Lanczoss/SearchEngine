#include "DictProducer.h"
#include "Dictionary.h"
#include "SplitToolCppJieba.h"

int main() {
  SplitToolCppJieba jieba;
  DictProducer dp("../settings.json", &jieba);
  dp.buildEnDict();
  dp.buildCnDict();
  dp.createIndex();
  dp.store();

  Dictionary::getInstance()->init();
  Dictionary::getInstance()->doQuery("中国hello");
}
