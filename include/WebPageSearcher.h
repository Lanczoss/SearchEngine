#ifndef WEB_PAGE_SEARCHER_H_
#define WEB_PAGE_SEARCHER_H_

#include <string>

#include "reactor/TcpServer.h"
using std::string;

class Webpage;

class WebPageSearcher {
 public:
  WebPageSearcher(const string &sought, const TcpConnectionPtr &conn);
  void doQuery();

 private:
  string _sought;
  TcpConnectionPtr _conn;
};

#endif  // !WEB_PAGE_SEARCHER_H_
