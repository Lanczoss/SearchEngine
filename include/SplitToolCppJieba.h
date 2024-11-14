#ifndef SPLIT_TOOL_CPPJIEBA_H_
#define SPLIT_TOOL_CPPJIEBA_H_

#include "Configuration.h"
#include "SplitTool.h"
#include "cppjieba/Jieba.hpp"

class SplitToolCppJieba : public SplitTool {
 public:
  SplitToolCppJieba(Configuration*);
  vector<string> cut(string) override;

 private:
  Configuration* _config;
  cppjieba::Jieba _jieba;
};

#endif  // !SPLIT_TOOL_CPPJIEBA_H_
