#include "reactor/MyTask.h"

#include <stdlib.h>

// #include <iostream>

#include "reactor/TcpConnection.h"
// using std::cout;
// using std::endl;

MyTask::MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg), _con(con) {}

MyTask::~MyTask() {}

void MyTask::process() {
  // mission main logic!!!!!!!!!!

  // thread(threadPool) informs EventLoop that msg processed
  _con->sendToLoop(_msg);
}
