#include "KeyRecommander.h"

#include <iostream>

#include "Dictionary.h"
using std::cerr;

KeyRecommander::KeyRecommander(const string &word) : _sought(word) {
  vector<CandidateResult> dictResult =
      Dictionary::getInstance()->doQuery(_sought);
  for (auto &cd : dictResult) {
    _prique.push(cd);
  }

  showPrique();
}

void KeyRecommander::showPrique() {
  while (!_prique.empty()) {
    CandidateResult cr = _prique.top();
    cerr << "string = " << cr._word << ", frequency = " << cr._freq
         << ", dist = " << cr._dist << '\n';
    _prique.pop();
  }
}
