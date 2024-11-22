#include "ProtocolParser.h"
string ProtocolParser::_method;
string ProtocolParser::_url;
string ProtocolParser::_status;
string ProtocolParser::_body;

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

int ProtocolParser::parse(const std::string& data) {
  enum llhttp_errno err = llhttp_execute(&_parser, data.c_str(), data.size());
  if (err != HPE_OK) {
    fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
            _parser.reason);
    return 0;
  }
  return 1;
}
