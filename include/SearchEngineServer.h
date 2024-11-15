#ifndef SEARCH_ENGINE_SERVER_H_
#define SEARCH_ENGINE_SERVER_H_

#include "reactor/TcpServer.h"
#include "reactor/ThreadPool.h"

class SearchEngineServer {
 public:
  SearchEngineServer();
  void start();
  void stop();
  void onNewConnection(const TcpConnectionPtr &);
  void onMessage(const TcpConnectionPtr &);
  void onClosed(const TcpConnectionPtr &);
  void doTaskThread(const TcpConnectionPtr &, string &msg);

 private:
  ThreadPool _threadPool;
  TcpServer _tcpServer;
};

#endif  // !SEARCH_ENGINE_SERVER_H_
