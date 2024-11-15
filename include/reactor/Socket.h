#ifndef SOCKET_H_
#define SOCKET_H_

class Socket {
 public:
  Socket();
  explicit Socket(int fd);
  ~Socket();
  int getFd() const;

 private:
  int _fd;
};

#endif  // !SOCKET_H_
