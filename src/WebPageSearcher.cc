#include "WebPageSearcher.h"

#include <iostream>

#include "Configuration.h"
#include "WebPageQuery.h"
#include "reactor/TcpConnection.h"
using std::cerr;

WebPageSearcher::WebPageSearcher(const string &sought,
                                 const TcpConnectionPtr &conn)
    : _sought(sought), _conn(conn) {}

void WebPageSearcher::doQuery() {
  WebPageQuery wpq;
  string pages =
      Configuration::getInstance()->createPages(wpq.doQuery(_sought));
  string msg =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: application/json; charset=UTF-8\r\n"
      "Content-Length: " +
      std::to_string(pages.size()) +
      "\r\n"
      "Connection: keep-alive\r\n"
      "\r\n";
  msg.append(pages);
  // cout << _msg;

  // cerr << msg;
  _conn->sendToLoop(msg);
}
