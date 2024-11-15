#ifndef SOCKETIO_H_
#define SOCKETIO_H_

#include "Socket.h"

class SocketIO {
 public:
  explicit SocketIO(int fd);
  ~SocketIO();
  int readn(void *buf, int length);
  int writen(const char *buf, int length);
  int readLine(void *buf, int length);
  int fd() const;

 private:
  Socket _netSock;
};

#endif  // !SOCKETIO_H_
