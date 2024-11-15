#include "DictProducer.h"
#include "Dictionary.h"
#include "KeyRecommander.h"
#include "SplitToolCppJieba.h"

int main() {
  // SplitToolCppJieba jieba;
  // DictProducer dp("../settings.json", &jieba);
  // dp.buildEnDict();
  // dp.buildCnDict();
  // dp.createIndex();
  // dp.store();

  Dictionary::getInstance()->init();
  // Dictionary::getInstance()->doQuery("中国");
  KeyRecommander kr("hello");
  // kr.doQuery();
}
