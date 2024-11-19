#include "KeyRecommander.h"

#include <iostream>

#include "Configuration.h"
#include "Dictionary.h"
#include "reactor/TcpConnection.h"
using std::cerr;

KeyRecommander::KeyRecommander(const string &word, const TcpConnectionPtr &con)
    : _sought(word), _prique(), _con(con) {
  // showPrique();
}

void KeyRecommander::doQuery() {
  vector<CandidateResult> dictResult =
      Dictionary::getInstance()->doQuery(_sought);
  for (auto &cd : dictResult) {
    _prique.push(cd);
  }
  vector<string> words;
  while (!_prique.empty()) {
    CandidateResult cr = _prique.top();
    words.push_back(cr._word);
    // cerr << cr._word << '\n';
    _prique.pop();
  }
  string jwords = Configuration::getInstance()->createWords(words);
  string msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: application/json; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(jwords.size()) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  msg.append(jwords);
  // cout << _msg;
  // cerr << msg;
  _con->sendToLoop(msg);
}

void KeyRecommander::showPrique() {
  while (!_prique.empty()) {
    CandidateResult cr = _prique.top();
    cerr << "string = " << cr._word << ", frequency = " << cr._freq
         << ", dist = " << cr._dist << '\n';
    _prique.pop();
  }
}
