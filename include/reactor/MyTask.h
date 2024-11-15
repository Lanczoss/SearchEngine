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

 private:
  string _msg;
  TcpConnectionPtr _con;
};

#endif  // !MYTASK_H_
