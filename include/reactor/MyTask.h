#ifndef MYTASK_H_
#define MYTASK_H_

#include <memory>
#include <string>
using std::string;

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class MyTask {
 public:
  MyTask(const string &msg, const TcpConnectionPtr &);
  ~MyTask();
  void process();
  string urlDecode(const string &);

 private:
  void responseIndex(const string &);
  void responseCss(const string &);
  void responseJs(const string &);
  void responseRecommand(const string &);
  void responseError();
  void responseCandidate(const string &);

 private:
  string _msg;
  TcpConnectionPtr _con;
};

#endif  // !MYTASK_H_
