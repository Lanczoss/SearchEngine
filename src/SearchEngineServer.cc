#include "SearchEngineServer.h"

#include <iostream>

#include "Configuration.h"
#include "reactor/MyTask.h"
#include "reactor/TcpConnection.h"
using std::cout;

SearchEngineServer::SearchEngineServer()
    : _threadPool(
          std::stoi(Configuration::getInstance()->network("pool", "number")),
          std::stoi(
              Configuration::getInstance()->network("pool", "queue.size"))),
      _tcpServer(Configuration::getInstance()->network("tcp", "ip"),
                 Configuration::getInstance()->network("tcp", "port")) {}

void SearchEngineServer::start() {
  _threadPool.start();
  _tcpServer.setAllCallback(
      [this](const TcpConnectionPtr &con) { onNewConnection(con); },
      [this](const TcpConnectionPtr &con) { onMessage(con); },
      [this](const TcpConnectionPtr &con) { onClosed(con); });
  _tcpServer.start();
}

void SearchEngineServer::stop() {
  _tcpServer.stop();
  _threadPool.stop();
}

// TcpConnectionPtr
// 连接建立
void SearchEngineServer::onNewConnection(const TcpConnectionPtr &con) {
  cout << con->toString() << " has connected!!!\n";
}

// 文件描述符可读，读客户端的数据
void SearchEngineServer::onMessage(const TcpConnectionPtr &con) {
  string msg = con->receive();
  cout << ">>recv msg from client: " << msg << '\n';

  // 接收到客户端的数据之后，可以将数据做业务逻辑的处理
  // add the task(msg) to threadPool

  _threadPool.addTask([msg, con]() {
    MyTask mt(msg, con);
    mt.process();
  });
  // Task can send now
  // con->send(msg);
}

// 连接断开
void SearchEngineServer::onClosed(const TcpConnectionPtr &con) {
  cout << con->toString() << " has closed!!!" << '\n';
}
