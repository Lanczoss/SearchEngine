#include "reactor/MyTask.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <memory>
using std::unique_ptr;

#include "Configuration.h"
#include "KeyRecommander.h"
#include "ProtocolParser.h"
#include "WebPageSearcher.h"
#include "reactor/TcpConnection.h"
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

MyTask::MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg), _con(con) {}

MyTask::~MyTask() {}

void MyTask::process() {
  // mission main logic!!!!!!!!!!
  ProtocolParser::getInstance()->parse(_msg);
  string method = ProtocolParser::getInstance()->getMethod();
  string url = ProtocolParser::getInstance()->getUrl();
  if (method == "GET") {
    if (url == "/" || url == "/search") {
      responseIndex();
    } else if (url == "/static/mdui.css") {
      responseCss();
    } else if (url == "/static/mdui.global.js") {
      responseJs();
    } else if (url.substr(0, 8) == "/search?") {
      responseRecommand();
    } else {
      // 这里回复一个404
      responseError();
    }
    // thread(threadPool) informs EventLoop that msg processed
  } else if (method == "POST") {
    if (url.substr(0, 9) == "/suggest?") {
      responseCandidate();
    } else {
      responseError();
    }
  } else {
    responseError();
  }
}

void MyTask::responseIndex() {
  // 先回复一个HTML
  ifstream ifs(Configuration::getInstance()->page("index"));
  if (!ifs) {
    cerr << "open index.html failed!\n";
    return;
  }
  // string page;
  auto pageLength =
      std::filesystem::file_size(Configuration::getInstance()->page("index"));
  unique_ptr<char[]> page(new char[pageLength + 1]());
  ifs.read(page.get(), pageLength);
  _msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(pageLength) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  _msg.append(page.get());
  // cout << _msg;
  ifs.close();
  _con->sendToLoop(_msg);
}

void MyTask::responseCss() {
  ifstream ifs(Configuration::getInstance()->page("css"));
  if (!ifs) {
    cerr << "open index.html failed!\n";
    return;
  }
  // string page;
  auto pageLength =
      std::filesystem::file_size(Configuration::getInstance()->page("css"));
  unique_ptr<char[]> page(new char[pageLength + 1]());
  ifs.read(page.get(), pageLength);
  _msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/css; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(pageLength) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  _msg.append(page.get());
  // cout << _msg;
  ifs.close();
  _con->sendToLoop(_msg);
}

void MyTask::responseJs() {
  ifstream ifs(Configuration::getInstance()->page("js"));
  if (!ifs) {
    cerr << "open index.html failed!\n";
    return;
  }
  // string page;
  auto pageLength =
      std::filesystem::file_size(Configuration::getInstance()->page("js"));
  unique_ptr<char[]> page(new char[pageLength + 1]());
  ifs.read(page.get(), pageLength);
  _msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/js; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(pageLength) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  _msg.append(page.get());
  // cout << _msg;
  ifs.close();
  _con->sendToLoop(_msg);
}

void MyTask::responseRecommand() {
  string searchKey = ProtocolParser::getInstance()->getUrl().substr(10);
  searchKey = urlDecode(searchKey);
  // cerr << searchKey << '\n';
  WebPageSearcher wps(searchKey, _con);
  wps.doQuery();
}

void MyTask::responseCandidate() {
  string canKey = ProtocolParser::getInstance()->getUrl().substr(11);
  canKey = urlDecode(canKey);
  // cerr << canKey << '\n';
  KeyRecommander kr(canKey, _con);
  kr.doQuery();
}

std::string MyTask::urlDecode(const std::string &url) {
  std::string decoded_str;
  char ch;
  int i, ii;
  for (i = 0; i < url.length(); i++) {
    if (url[i] == '%') {
      sscanf(url.substr(i + 1, 2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      decoded_str += ch;
      i = i + 2;
    } else {
      decoded_str += url[i];
    }
  }
  return decoded_str;
}

void MyTask::responseError() {
  // 先回复一个HTML
  ifstream ifs(Configuration::getInstance()->page("404"));
  if (!ifs) {
    cerr << "open index.html failed!\n";
    return;
  }
  // string page;
  auto pageLength =
      std::filesystem::file_size(Configuration::getInstance()->page("404"));
  unique_ptr<char[]> page(new char[pageLength + 1]());
  ifs.read(page.get(), pageLength);
  _msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(pageLength) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  _msg.append(page.get());
  // cout << _msg;
  ifs.close();
  _con->sendToLoop(_msg);
}
