#include "reactor/EventLoop.h"

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <iostream>
using std::lock_guard;

#include "reactor/Acceptor.h"
#include "reactor/TcpConnection.h"

using std::cout;
using std::endl;
using std::string;

// get socket_fd to do work
EventLoop::EventLoop(Acceptor &acc)
    : _epfd(createEpollFd()),
      // vector initialized by size 1024
      _evtList(1024),
      _isLooping(false),
      _acceptor(acc),
      _conns(),
      _mutex(),
      _pendings(10),
      _eventFd(createEventFd()) {
  // add socket_fd into epoll
  addEpollReadFd(acc.getSocketFd());

  addEpollReadFd(_eventFd);
}

EventLoop::~EventLoop() {}

void EventLoop::loop() {
  _isLooping = true;
  while (_isLooping) {
    waitEpollFd();
  }
}

void EventLoop::unloop() { _isLooping = false; }

void EventLoop::addEpollReadFd(int newFd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = newFd;
  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, newFd, &event);
  if (-1 == ret) {
    perror("epoll_ctl in addEpollReadFd");
    return;
  }
}

void EventLoop::delEpollReadFd(int delFd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = delFd;
  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, delFd, &event);
  if (-1 == ret) {
    perror("epoll_ctl in delEpollReadFd");
    return;
  }
}

void EventLoop::waitEpollFd() {
  int nready = 0;
  // when interrupts by other signal, we need restart epoll_wait
  do {
    nready = ::epoll_wait(_epfd, _evtList.data(), _evtList.size(), 3000);

  } while (-1 == nready && errno == EINTR);

  if (-1 == nready) {
    perror("epoll_wait error in waitEpollFd");
    return;
  } else if (0 == nready) {
    cout << ">>epoll wait timeout once!" << endl;
  } else {
    // main logic
    // manual resize
    if (nready == _evtList.size()) {
      _evtList.resize(2 * nready);
    }
    for (size_t idx = 0; idx < nready; ++idx) {
      int readyFd = _evtList[idx].data.fd;
      if (_acceptor.getSocketFd() == readyFd) {
        handleNewConnecion();
      } else if (_eventFd == readyFd) {
        // read something to clear the counter
        handleEventFd();

        // and now we can handleFunctors
        handleFunctors();
      } else {
        // existed connection
        handleMessage(readyFd);
      }
    }
  }
}

int EventLoop::createEpollFd() {
  int epollFd = ::epoll_create1(0);
  if (-1 == epollFd) {
    perror("epoll_create1 in createEpollFd");
    return -1;
  }
  return epollFd;
}

// when a new connection is coming
// add fd into epoll
// add TcpConnection into map
void EventLoop::handleNewConnecion() {
  int netFd = _acceptor.accept();
  // On success, these system calls return a file descriptor for the accepted
  // socket (a nonnegative integer). On error, -1 is returned, errno is set
  // to indicate the error, and addrlen is left unchanged.
  if (netFd < 0) {
    perror("accpet in handleNewConnecion");
    return;
  }

  addEpollReadFd(netFd);

  TcpConnectionPtr con(new TcpConnection(netFd, this));

  // register three functions
  con->setNewConnectionCallback(_onNewConnection);
  con->setMessageCallback(_onMessage);
  con->setCloseCallback(_onClose);

  _conns.insert(std::make_pair(netFd, shared_ptr<TcpConnection>(con)));

  con->handleNewConnectionCallback();
}
void EventLoop::handleMessage(int netFd) {
  auto it = _conns.find(netFd);
  if (it != _conns.end()) {
    // connection is existed and not closed
    if (it->second->isClosed()) {
      it->second->handleCloseCallback();
      delEpollReadFd(netFd);
      _conns.erase(it);
    } else {
      // execute the callback function here

      it->second->handleMessageCallback();
    }
  } else {
    cout << ">>connection doesn't existed!" << endl;
    return;
  }
}

// connected event
void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb) {
  _onNewConnection = std::move(cb);
}
// Message arrived event
void EventLoop::setMessageCallback(TcpConnectionCallback &&cb) {
  _onMessage = std::move(cb);
}
// disconnected event
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb) {
  _onClose = std::move(cb);
}

void EventLoop::runInLoop(functor &&f) {
  // save the functor
  {
    lock_guard<mutex> lg(_mutex);
    _pendings.push_back(std::move(f));
  }

  // inform epoll_wait
  wakeup();
}

int EventLoop::createEventFd() {
  int eventFd = eventfd(0, 0);
  if (eventFd == -1) {
    perror("eventfd in createEventFd");
    return -1;
  }
  return eventFd;
}

void EventLoop::wakeup() {
  // write something into eventfd
  uint64_t one = 1;
  ssize_t ret = write(_eventFd, &one, sizeof(uint64_t));
  if (ret != sizeof(uint64_t)) {
    perror("write in wakeup");
    return;
  }
}

void EventLoop::handleEventFd() {
  // read something from eventfd
  uint64_t one = 1;
  ssize_t ret = read(_eventFd, &one, sizeof(uint64_t));
  if (ret != sizeof(uint64_t)) {
    perror("read in handleEventFd");
    return;
  }
}

void EventLoop::handleFunctors() {
  vector<functor> temp;
  {
    lock_guard<mutex> lg(_mutex);
    temp.swap(_pendings);
    _pendings.clear();
  }
  for (auto &f : temp) {
    // this->send(msg)
    if (f) {
      f();
    }
  }
}
