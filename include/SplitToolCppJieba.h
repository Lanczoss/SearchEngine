#ifndef SPLIT_TOOL_CPPJIEBA_H_
#define SPLIT_TOOL_CPPJIEBA_H_

#include "SplitTool.h"
#include "cppjieba/Jieba.hpp"

class SplitToolCppJieba : public SplitTool {
  const char* const DICT_PATH = "../dict/jieba.dict.utf8";
  const char* const HMM_PATH = "../dict/hmm_model.utf8";
  const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
  const char* const IDF_PATH = "../dict/idf.utf8";
  const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

 public:
  SplitToolCppJieba();
  vector<string> cut(string) override;

 private:
  cppjieba::Jieba jieba;
};

#endif  // !SPLIT_TOOL_CPPJIEBA_H_
