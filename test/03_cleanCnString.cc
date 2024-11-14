#include "DictProducer.h"
#include "SplitToolCppJieba.h"

int main() {
  Configuration config;
  SplitToolCppJieba jieba(&config);
  DictProducer dp("../yuliao/english.txt", &config, &jieba);
  dp.buildEnDict();
  dp.buildCnDict();
  dp.createIndex();
  dp.store();
}
