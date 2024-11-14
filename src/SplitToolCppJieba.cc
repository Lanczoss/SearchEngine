#include "SplitToolCppJieba.h"

SplitToolCppJieba::SplitToolCppJieba()
    : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH) {}

vector<string> SplitToolCppJieba::cut(string sentence) {
  vector<string> words;
  jieba.Cut(sentence, words, true);
  return words;
}
