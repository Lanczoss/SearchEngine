#include "ProtocolParser.h"
string ProtocolParser::_method;
string ProtocolParser::_url;
string ProtocolParser::_status;
string ProtocolParser::_body;
ProtocolParser* ProtocolParser::_pInstance = getInstance();
pthread_once_t ProtocolParser::_once = PTHREAD_ONCE_INIT;

ProtocolParser::ProtocolParser() : _parser(), _settings() {
  llhttp_init(&_parser, HTTP_BOTH, &_settings);

  // _settings.on_message_begin = on_message_begin;
  _settings.on_status = on_status;
  _settings.on_method = on_method;
  _settings.on_url = on_url;
  // _settings.on_header_field = on_header_field;
  // _settings.on_header_value = on_header_value;
  // _settings.on_headers_complete = on_headers_complete;
  _settings.on_body = on_body;
  // _settings.on_message_complete = on_message_complete;
}

ProtocolParser::~ProtocolParser() {}

ProtocolParser* ProtocolParser::getInstance() {
  pthread_once(&_once, initReady);
  return _pInstance;
}

void ProtocolParser::initReady() {
  _pInstance = new ProtocolParser();
  atexit(destory);
}

int ProtocolParser::parse(const std::string& data) {
  enum llhttp_errno err = llhttp_execute(&_parser, data.c_str(), data.size());
  if (err != HPE_OK) {
    fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
            _parser.reason);
    return -1;
  }
  llhttp_init(&_parser, HTTP_BOTH, &_settings);
  return 0;
}

void ProtocolParser::destory() {
  if (_pInstance) {
    delete _pInstance;
    _pInstance = nullptr;
  }
}
