#include "reactor/TaskQueue.h"

TaskQueue::TaskQueue(int capacity)
    : _queue(),
      _mutex(),
      _notEmpty(),
      _notFull(),
      _capacity(capacity),
      _flag(true) {}

TaskQueue::~TaskQueue() {}

void TaskQueue::push(ElemType &&value) {
  // get lock
  unique_lock<std::mutex> m(_mutex);
  while (full()) {
    _notFull.wait(m);
  }
  _queue.push(std::move(value));
  // notify who waits for queue
  _notEmpty.notify_one();
}

TaskQueue::ElemType TaskQueue::pop() {
  unique_lock<std::mutex> m(_mutex);
  while (empty() && _flag) {
    _notEmpty.wait(m);
  }
  if (_flag) {
    ElemType temp = _queue.front();
    _queue.pop();
    _notFull.notify_one();
    return temp;
  } else {
    return nullptr;
  }
}

bool TaskQueue::empty() { return 0 == _queue.size(); }

bool TaskQueue::full() { return _capacity == _queue.size(); }

void TaskQueue::wakeup() {
  _flag = false;
  _notEmpty.notify_all();
}
