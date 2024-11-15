#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
using std::function;
using std::unique_lock;

// BO_threadPool
using Task = function<void()>;

class TaskQueue {
 public:
  // Task is the elementtype in queue
  using ElemType = Task;

  TaskQueue(int);
  ~TaskQueue();
  void push(ElemType&&);
  ElemType pop();
  bool empty();
  bool full();
  void wakeup();

 private:
  std::queue<ElemType> _queue;
  std::mutex _mutex;
  std::condition_variable _notEmpty;
  std::condition_variable _notFull;
  int _capacity;
  bool _flag;
};

#endif  // !TASKQUEUE_H_
