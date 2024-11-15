#include "SplitToolCppJieba.h"

SplitToolCppJieba::SplitToolCppJieba()
    : SplitTool(),
      _jieba(Configuration::getInstance()->dp("jieba", "dict"),
             Configuration::getInstance()->dp("jieba", "hmm"),
             Configuration::getInstance()->dp("jieba", "user"),
             Configuration::getInstance()->dp("jieba", "idf"),
             Configuration::getInstance()->dp("jieba", "stop")) {}

vector<string> SplitToolCppJieba::cut(string sentence) {
  vector<string> words;
  _jieba.Cut(sentence, words, true);
  return words;
}
