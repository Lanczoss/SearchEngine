#ifndef KEY_RECOMMANDER_H_
#define KEY_RECOMMANDER_H_

#include <queue>
#include <string>
#include <vector>

#include "CandidateResult.h"
#include "reactor/TcpServer.h"
using std::priority_queue;
using std::string;
using std::vector;

struct CompareCandidate {
  bool operator()(const CandidateResult& lhs, const CandidateResult& rhs) {
    if (lhs._dist != rhs._dist) {
      return lhs._dist > rhs._dist;
    } else {
      return lhs._freq < rhs._freq;
    }
  }
};

class KeyRecommander {
 public:
  KeyRecommander(const string& word, const TcpConnectionPtr& con);
  // KeyRecommander(const string& word);
  void doQuery();
  // string doQuery(const string &word);

 private:
  void showPrique();

 private:
  string _sought;
  priority_queue<CandidateResult, vector<CandidateResult>, CompareCandidate>
      _prique;
  TcpConnectionPtr _con;
};

#endif  // !KEY_RECOMMANDER_H_
