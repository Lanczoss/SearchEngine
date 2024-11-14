#ifndef SPLIT_TOOL_H_
#define SPLIT_TOOL_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

class SplitTool {
 public:
  virtual vector<string> cut(string) = 0;
};

#endif  // !SPLIT_TOOL_H_
