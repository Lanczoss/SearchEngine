#include "SplitToolCppJieba.h"

SplitToolCppJieba::SplitToolCppJieba(Configuration* config)
    : _config(config),
      _jieba(_config->get_dp_config("jieba", "dict"),
             _config->get_dp_config("jieba", "hmm"),
             _config->get_dp_config("jieba", "user"),
             _config->get_dp_config("jieba", "idf"),
             _config->get_dp_config("jieba", "stop")) {}

vector<string> SplitToolCppJieba::cut(string sentence) {
  vector<string> words;
  _jieba.Cut(sentence, words, true);
  return words;
}
