#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOLL_H

#include "src/basics/noncopyable.h"
#include "src/net/EventLoopThread.h"

#include <memory>
#include <vector>

namespace netlib
{
namespace net
{

class EventLoop;

class EventLoopThreadPool: netlib::noncopyable {
 public:
  EventLoopThreadPool(EventLoop* baseLoop);
  ~EventLoopThreadPool();

  void setThreadNum(int nums) { threadNums_ = nums; }
  void start();
  EventLoop* getNextLoop();

 private:
  EventLoop* baseLoop_;
  int threadNums_;
  int next_;
  bool started_;
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};

} // net
} // netlib

#endif  /* EVENTLOOPTHREADPOOL_H */
