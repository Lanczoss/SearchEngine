#include "reactor/TcpConnection.h"

#include <stdio.h>

#include <iostream>
using std::cout;
using std::endl;

#include "reactor/EventLoop.h"

TcpConnection::TcpConnection(int fd, EventLoop *loop)
    : _sockIO(fd),
      _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()),
      _loop(loop) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const string &msg) {
  _sockIO.writen(msg.data(), msg.size());
}

string TcpConnection::receive() {
  char buf[65535] = {0};
  _sockIO.readLine(buf, sizeof(buf));
  return buf;
}

InetAddress TcpConnection::getLocalAddr() {
  struct sockaddr_in addr;
  socklen_t length = sizeof(addr);
  int ret = getsockname(_sockIO.fd(), (struct sockaddr *)&addr, &length);
  if (-1 == ret) {
    perror("getsockname");
  }

  return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr() {
  struct sockaddr_in addr;
  socklen_t length = sizeof(addr);
  int ret = getpeername(_sockIO.fd(), (struct sockaddr *)&addr, &length);
  if (-1 == ret) {
    perror("getpeername");
  }

  return InetAddress(addr);
}

string TcpConnection::toString() {
  return _localAddr.ip() + ":" + _localAddr.port() + " <-----> " +
         _peerAddr.ip() + ":" + _peerAddr.port() + " connected";
}

bool TcpConnection::isClosed() const {
  char buf[10] = {0};
  int ret = ::recv(_sockIO.fd(), buf, sizeof(buf), MSG_PEEK);
  return (0 == ret);
}

// connected event
void TcpConnection::setNewConnectionCallback(TcpConnectionCallback &cb) {
  _onNewConnection = cb;
}
// Message arrived event
void TcpConnection::setMessageCallback(TcpConnectionCallback &cb) {
  _onMessage = cb;
}
// disconnected event
void TcpConnection::setCloseCallback(TcpConnectionCallback &cb) {
  _onClose = cb;
}

// execute three functions
void TcpConnection::handleNewConnectionCallback() {
  if (_onNewConnection) {
    _onNewConnection(shared_from_this());
  }
}
void TcpConnection::handleMessageCallback() {
  if (_onMessage) {
    _onMessage(shared_from_this());
  }
}
void TcpConnection::handleCloseCallback() {
  if (_onClose) {
    _onClose(shared_from_this());
  }
}

void TcpConnection::sendToLoop(const string &msg) {
  // EventLoop executes the callback function
  // send TcpConnection, msg and send function
  if (_loop) {
    // void runInLoop(function<void()> &&)
    _loop->runInLoop([this, msg]() { send(msg); });
  }
}
