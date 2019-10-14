
#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include "src/basics/noncopyable.h"
#include "src/basics/Mutex.h"
#include "src/basics/Condition.h"

namespace netlib
{
namespace net
{

class EventLoop;

class EventLoopThread: netlib::noncopyable {
 public:
  EventLoopThread();
  ~EventLoopThread();

  EventLoop* startLoop();

 private:
  EventLoop* loop_;
  bool exiting_;
  Mutex
};

} // net
} // netlib

#endif  /* EVENTLOOPTHREAD_H */
