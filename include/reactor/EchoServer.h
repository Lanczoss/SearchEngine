#ifndef ECHOSERVER_H_
#define ECHOSERVER_H_

#include <iostream>
#include <string>

#include "TcpServer.h"
#include "ThreadPool.h"
using std::string;

class EchoServer {
 public:
  EchoServer(size_t threadNum, size_t queueSize, const string &ip,
             const string &port);
  ~EchoServer();
  void start();
  void stop();

 private:
  // three callback functions are member functions now
  void onNewConnection(const TcpConnectionPtr &con);
  void onMessage(const TcpConnectionPtr &con);
  void onClosed(const TcpConnectionPtr &con);

 private:
  ThreadPool _pool;
  TcpServer _tcp;
};

#endif  // !ECHOSERVER_H_
