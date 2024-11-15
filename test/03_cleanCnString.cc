#include "DictProducer.h"
#include "SplitToolCppJieba.h"

int main() {
  SplitToolCppJieba jieba;
  DictProducer dp("../yuliao/english.txt", &jieba);
  dp.buildEnDict();
  dp.buildCnDict();
  dp.createIndex();
  dp.store();
}
