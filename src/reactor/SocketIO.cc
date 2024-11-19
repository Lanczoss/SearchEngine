#include "reactor/SocketIO.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "ProtocolParser.h"
using std::cout;
using std::endl;

SocketIO::SocketIO(int fd) : _netSock(fd) {}

SocketIO::~SocketIO() {}

int SocketIO::readn(void *buf, int length) {
  int remind = length;
  char *pstr = static_cast<char *>(buf);
  ssize_t ret;

  while (remind > 0) {
    ret = read(_netSock.getFd(), pstr, remind);
    if (ret == -1 && errno == EINTR) {
      // interrupt by a signal handler
      continue;
    } else if (ret == -1) {
      perror("read in readn");
      return -1;
    } else if (ret == 0) {
      cout << "disconnected by peer" << endl;
      break;
    } else {
      pstr += ret;
      remind -= ret;
    }
  }
  return length - remind;
}

int SocketIO::writen(const char *buf, int length) {
  int remind = length;
  const char *pstr = buf;
  ssize_t ret = 0;

  while (remind > 0) {
    ret = write(_netSock.getFd(), buf, remind);
    if (ret == -1 && errno == EINTR) {
      continue;
    } else if (ret == -1) {
      perror("write in writen");
      return -1;
    } else if (ret == 0) {
      cout << "disconnected by peer" << endl;
      break;
    } else {
      pstr += ret;
      remind -= ret;
    }
  }
  return length - remind;
}

int SocketIO::readHttp(void *buf) {
  // length created by buf
  // last must be '\0'
  char *pstr = static_cast<char *>(buf);
  ssize_t ret;

  ret = recv(_netSock.getFd(), buf, 65535, MSG_PEEK);
  if (ret == -1 && errno == EINTR) {
    cerr << "EINTR\n";
  } else if (ret == -1) {
    perror("recv in readLine");
    return -1;
  } else if (ret == 0) {
    cout << "disconnected by peer" << endl;
    return -1;
  }
  // cerr << pstr;
  readn(pstr, ret);
  pstr += ret;
  *pstr = '\0';
  return 0;
}

int SocketIO::fd() const { return _netSock.getFd(); }
