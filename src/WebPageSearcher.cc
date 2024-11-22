#include "WebPageSearcher.h"

#include <iostream>

#include "Configuration.h"
#include "RedisServer.h"
#include "WebPageQuery.h"
#include "reactor/TcpConnection.h"
using std::cerr;

WebPageSearcher::WebPageSearcher(const string &sought,
                                 const TcpConnectionPtr &conn)
    : _sought(sought), _conn(conn) {}

void WebPageSearcher::doQuery() {
  // 先从redis中找
  RedisServer rs;
  string msg;
  if (!rs.get(_sought, msg)) {
    // 没有找到
    WebPageQuery wpq;
    vector<WebPage> pages = wpq.doQuery(_sought);
    nlohmann::json jp;
    for (auto &web : pages) {
      jp.push_back({web.getTitle(), web.getUrl(), web.getDocContent()});
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

    // 添加到redis中
    rs.put(_sought, msg);

    // cerr << msg;
  }
  _conn->sendToLoop(msg);
}
