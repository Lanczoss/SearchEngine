#include <iostream>
#include <string>
#include <vector>

#include "cppjieba/Jieba.hpp"
using std::cout;
using std::string;
using std::vector;

int main() {
  const char* const DICT_PATH = "../dict/jieba.dict.utf8";
  const char* const HMM_PATH = "../dict/hmm_model.utf8";
  const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
  const char* const IDF_PATH = "../dict/idf.utf8";
  const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";
  cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH,
                        STOP_WORD_PATH);
  // string sentence = "我从北京清华大学来，去北京航空航天大学";
  string sentence = "【分 类 号】J1";
  vector<string> words;
  jieba.Cut(sentence, words, true);
  for (auto word : words) {
    cout << "word = " << word << "\n";
  }
}
