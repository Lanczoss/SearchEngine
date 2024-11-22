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

// KeyRecommander::KeyRecommander(const string &word)
//     : _sought(word), _prique() {
//   // showPrique();
// }

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
  nlohmann::json jp;
  for (auto &word : words) {
    jp.push_back(word);
  }
  string msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: application/json; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(jp.dump().size()) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  msg.append(jp.dump());
  // cout << _msg;
  // cerr << msg;
  cerr << "send once\n";
  _con->sendToLoop(msg);
}

// string KeyRecommander::doQuery(const string &word) {
//   vector<CandidateResult> dictResult =
//       Dictionary::getInstance()->doQuery(word);
//   for (auto &cd : dictResult) {
//     _prique.push(cd);
//   }
//   vector<string> words;
//   while (!_prique.empty()) {
//     CandidateResult cr = _prique.top();
//     words.push_back(cr._word);
//     // cerr << cr._word << '\n';
//     _prique.pop();
//   }
//   string jwords = Configuration::getInstance()->createWords(words);
//   return jwords;
// }

void KeyRecommander::showPrique() {
  while (!_prique.empty()) {
    CandidateResult cr = _prique.top();
    cerr << "string = " << cr._word << ", frequency = " << cr._freq
         << ", dist = " << cr._dist << '\n';
    _prique.pop();
  }
}
