#ifndef CANDIDATE_RESULT_H_
#define CANDIDATE_RESULT_H_

#include <string>
using std::string;

class CandidateResult {
 public:
  CandidateResult(const string &word, int frequency, int distance);

 public:
  string _word;
  int _freq;
  int _dist;
};

#endif  // !CANDIDATE_RESULT_H_
