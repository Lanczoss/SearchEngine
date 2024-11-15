#include "SplitToolCppJieba.h"

SplitToolCppJieba::SplitToolCppJieba()
    : SplitTool(),
      _jieba(Configuration::get_dp_config("jieba", "dict"),
             Configuration::get_dp_config("jieba", "hmm"),
             Configuration::get_dp_config("jieba", "user"),
             Configuration::get_dp_config("jieba", "idf"),
             Configuration::get_dp_config("jieba", "stop")) {}

vector<string> SplitToolCppJieba::cut(string sentence) {
  vector<string> words;
  _jieba.Cut(sentence, words, true);
  return words;
}
