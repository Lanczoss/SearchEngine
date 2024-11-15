#include "reactor/InetAddress.h"

InetAddress::InetAddress(const string &ip, const string &port) {
  _addr.sin_family = AF_INET;
  _addr.sin_port = htons(atoi(port.c_str()));
  _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in &addr) : _addr(addr) {}

InetAddress::~InetAddress() {}

string InetAddress::ip() { return string(inet_ntoa(_addr.sin_addr)); }

string InetAddress::port() { return std::to_string(ntohs(_addr.sin_port)); }

struct sockaddr_in *InetAddress::getInetAddress() { return &_addr; }
