#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <memory>
#include <thread>
#include <vector>

#include "TaskQueue.h"

// BO_threadPool
using Task = function<void()>;

class ThreadPool {
 public:
  using ElemType = Task;

  ThreadPool(size_t, size_t);
  ~ThreadPool();
  void start();
  void stop();
  void addTask(ElemType&&);
  ElemType getTask();
  void doTask();

 private:
  size_t _threadNum;
  std::vector<std::thread> _threads;
  TaskQueue _taskQueue;
  size_t _queueSize;
  bool _isExit;
};

#endif  // !THREADPOOL_H_
