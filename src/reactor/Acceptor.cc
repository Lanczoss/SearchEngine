#include "reactor/Acceptor.h"

#include <stdio.h>

#include <thread>

Acceptor::Acceptor(const string &ip, const string &port)
    : _sock(), _addr(ip, port) {}

Acceptor::~Acceptor() {}

// return fd
int Acceptor::accept() {
  int net_fd = ::accept(_sock.getFd(), nullptr, nullptr);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  if (net_fd == -1) {
    perror("accept");
    return -1;
  }
  return net_fd;
}

void Acceptor::ready() {
  setReuseAddr();
  setReusePort();
  bind();
  listen();
}

void Acceptor::setReuseAddr() {
  int opt = 1;
  int ret =
      setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (ret == -1) {
    perror("setsockopt reuseaddr");
    return;
  }
}

void Acceptor::setReusePort() {
  int opt = 1;
  int ret =
      setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (ret == -1) {
    perror("setsockopt reuseport");
    return;
  }
}

void Acceptor::bind() {
  int ret = ::bind(_sock.getFd(), (struct sockaddr *)_addr.getInetAddress(),
                   sizeof(_addr));
  if (ret == -1) {
    perror("bind");
    return;
  }
}

void Acceptor::listen() {
  int ret = ::listen(_sock.getFd(), 128);
  if (ret == -1) {
    perror("listen");
    return;
  }
}

int Acceptor::getSocketFd() { return _sock.getFd(); }
