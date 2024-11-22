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

int SocketIO::readHttp(void *buf, int len) {
  // length created by buf
  // last must be '\0'
  char *pstr = static_cast<char *>(buf);
  int left = len - 1;
  int ret = 0, total = 0;

  while (left > 0) {
    // MSG_PEEK不会将缓冲区中的数据进行清空,只会进行拷贝操作
    ret = recv(_netSock.getFd(), pstr, left, MSG_PEEK);
    if (-1 == ret && errno == EINTR) {
      continue;
    } else if (-1 == ret) {
      perror("readLine error -1");
      return -1;
    } else if (0 == ret) {
      break;
    } else {
      for (int idx = 0; idx < ret; ++idx) {
        ProtocolParser pp;
        if (pstr[idx] == '\r' && pstr[idx + 1] == '\n' && pstr[idx + 2] == '\r' && pstr[idx + 3] == '\n'&& pp.parse(pstr)) {
          int sz = idx + 4;
          readn(pstr, sz);
          pstr += sz;
          *pstr = '\0';  // C风格字符串以'\0'结尾

          return total + sz;
        }
      }

      // readn的底层调用是read，会将数据从内核态拷贝到
      // 用户态，并且数据从内核态移除
      readn(pstr, ret);  // 从内核态拷贝到用户态
      total += ret;
      pstr += ret;
      left -= ret;
    }
  }
  *pstr = '\0';

  return total - left;
}

int SocketIO::fd() const { return _netSock.getFd(); }
