#ifndef PROTOCOL_PARSER_H_
#define PROTOCOL_PARSER_H_

#include <pthread.h>

#include <iostream>
#include <string>
using std::string;

#include "llhttp/llhttp.h"
using std::cerr;

class ProtocolParser {
 public:
  static ProtocolParser* getInstance();
  // void init();

  // 解析 HTTP 数据
  int parse(const std::string& data);

 private:
  ProtocolParser();
  ~ProtocolParser();
  ProtocolParser(const ProtocolParser& rhs) = delete;
  ProtocolParser& operator=(const ProtocolParser& rhs) = delete;

  static void destory();
  static void initReady();

 private:
  // 回调函数
  static int on_method(llhttp_t* parser, const char* at, size_t length) {
    // std::cout << "Method: " << std::string(at, length) << "\n";
    _method = string(at, length);
    return 0;
  }

  static int on_status(llhttp_t* parser, const char* at, size_t length) {
    _status = string(at, length);
    return 0;
  }

  static int on_url(llhttp_t* parser, const char* at, size_t length) {
    _url = string(at, length);
    return 0;
  }

  static int on_body(llhttp_t* parser, const char* at, size_t length) {
    _body = string(at, length);
    return 0;
  }

 public:
  static string _method;
  static string _url;
  static string _status;
  static string _body;

 private:
  llhttp_t _parser;
  llhttp_settings_t _settings;

  static ProtocolParser* _pInstance;
  static pthread_once_t _once;
};

#endif  // !PROTOCOL_PARSER_H_
