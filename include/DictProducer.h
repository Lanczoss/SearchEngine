#ifndef DICT_PRODUCER_H_
#define DICT_PRODUCER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

class DictProducer {
  const char* const stopWordsEng = "../yuliao/stop_words_eng.txt";

 public:
  // DicProducer(string, tool);
  DictProducer(string);
  void buildEnDict();
  void buildCnDict();
  void createIndex();
  void store();

 private:
  // 用于英文语料清洗的函数
  void cleanLine(string&);

 private:
  // 加载词典文件
  vector<string> _files;

  // 为了让每个单词都有一个编号，用vector保存每个单词及其词频
  vector<pair<string, int>> _dict;

  // 单词位置索引
  // map第一个参数是字母/汉字
  // map第二个参数是出现单词的索引集合，int保存的是单词的编号
  map<string, set<int>> _index;
  // SplitTool *_cuttor;
};

#endif  // !DICT_PRODUCER_H_
