#include "reactor/EchoServer.h"

#include <iostream>

#include "reactor/MyTask.h"
#include "reactor/TcpConnection.h"
using std::cout;
using std::endl;

EchoServer::EchoServer(size_t threadNum, size_t queueSize, const string &ip,
                       const string &port)
    : _pool(threadNum, queueSize), _tcp(ip, port) {}

EchoServer::~EchoServer() {}

void EchoServer::start() {
  _pool.start();
  _tcp.setAllCallback(
      [this](const TcpConnectionPtr &con) { onNewConnection(con); },
      [this](const TcpConnectionPtr &con) { onMessage(con); },
      [this](const TcpConnectionPtr &con) { onClosed(con); });
  _tcp.start();
}

void EchoServer::stop() {
  _tcp.stop();
  _pool.stop();
}

// TcpConnectionPtr
// 连接建立
void EchoServer::onNewConnection(const TcpConnectionPtr &con) {
  cout << con->toString() << " has connected!!!" << endl;
}

// 文件描述符可读，读客户端的数据
void EchoServer::onMessage(const TcpConnectionPtr &con) {
  string msg = con->receive();
  cout << ">>recv msg from client: " << msg << endl;

  // 接收到客户端的数据之后，可以将数据做业务逻辑的处理
  // add the task(msg) to threadPool

  /// auto mt = std::make_shared<MyTask>(msg, con);

  _pool.addTask([msg, con]() {
    MyTask mt(msg, con);
    mt.process();
  });
  // Task can send now
  // con->send(msg);
}

// 连接断开
void EchoServer::onClosed(const TcpConnectionPtr &con) {
  cout << con->toString() << " has closed!!!" << endl;
}
