#include "reactor/MyTask.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <memory>
using std::unique_ptr;

#include "Configuration.h"
#include "ProtocolParser.h"
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
  if (ProtocolParser::getInstance()->getMethod() == "GET" &&
      ProtocolParser::getInstance()->getUrl() == "/") {
    responseIndex();
  }
  if (ProtocolParser::getInstance()->getMethod() == "GET" &&
      ProtocolParser::getInstance()->getUrl() == "/static/mdui.css") {
    responseCss();
  }
  if (ProtocolParser::getInstance()->getMethod() == "GET" &&
      ProtocolParser::getInstance()->getUrl() == "/static/mdui.global.js") {
    responseJs();
  }
  // thread(threadPool) informs EventLoop that msg processed
  _con->sendToLoop(_msg);
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
}
