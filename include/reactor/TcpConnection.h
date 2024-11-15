#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include <functional>
#include <memory>

#include "InetAddress.h"
#include "SocketIO.h"
using std::function;
using std::shared_ptr;

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  using TcpConnectionPtr = shared_ptr<TcpConnection>;
  using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

 public:
  explicit TcpConnection(int fd, EventLoop *loop);
  ~TcpConnection();
  void send(const string &msg);
  string receive();

  string toString();

  bool isClosed() const;

 private:
  // get ip and port
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

 public:
  // connected event
  void setNewConnectionCallback(TcpConnectionCallback &cb);
  // Message arrived event
  void setMessageCallback(TcpConnectionCallback &cb);
  // disconnected event
  void setCloseCallback(TcpConnectionCallback &cb);

 private:
  TcpConnectionCallback _onNewConnection;
  TcpConnectionCallback _onMessage;
  TcpConnectionCallback _onClose;

 public:
  // execute three functions
  void handleNewConnectionCallback();
  void handleMessageCallback();
  void handleCloseCallback();

  void sendToLoop(const string &);

 private:
  SocketIO _sockIO;
  InetAddress _localAddr;
  InetAddress _peerAddr;

  // send information to EventLoop from pointer
  EventLoop *_loop;
};

#endif  // !TCPCONNECTION_H_
