#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <functional>
#include <memory>
#include <string>

#include "Acceptor.h"
#include "EventLoop.h"
using std::function;
using std::shared_ptr;
using std::string;

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

class TcpServer {
 public:
  TcpServer(const string &ip, const string &port);
  ~TcpServer();
  void start();
  void stop();
  void setAllCallback(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2,
                      TcpConnectionCallback &&cb3);

 private:
  Acceptor _acceptor;
  EventLoop _loop;
};

#endif  // !TCPSERVER_H_
