#ifndef INETADDRESS_H_
#define INETADDRESS_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>
using std::string;

class InetAddress {
 public:
  InetAddress(const string &ip, const string &port);
  InetAddress(const struct sockaddr_in &addr);
  ~InetAddress();
  string ip();
  string port();
  struct sockaddr_in *getInetAddress();

 private:
  struct sockaddr_in _addr;
};

#endif  // !INETADDRESS_H_
