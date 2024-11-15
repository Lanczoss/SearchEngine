#include "ProtocolParser.h"

int main() {
  string data =
      "POST /index.html HTTP/1.1\r\nconnection:close\r\ncontent-length: "
      "1\r\n\r\n1\r\n\r\n";
  ProtocolParser::getInstance()->parse(data);
  cerr << "method = " << ProtocolParser::_method << '\n';
  cerr << "url = " << ProtocolParser::_url << '\n';
  data =
      "HTTP/1.1 200 OK\r\nServer: nginx\r\ncontent-length: "
      "11\r\n\r\nhello:world\r\n\r\n";
  ProtocolParser::getInstance()->parse(data);
  cerr << "body = " << ProtocolParser::_body << '\n';
}
