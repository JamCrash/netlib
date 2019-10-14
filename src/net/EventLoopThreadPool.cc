
#include "src/net/EventLoop.h"
#include "src/net/EventLoopThread.h"
#include "src/net/EventLoopThreadPool.h"

#include <assert.h>

using namespace netlib;
using namespace net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
: baseLoop_(baseLoop),
  threadNums_(0),
  next_(0),
  started_(false);
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start()
{
  assert(!started_);
  baseLoop_->assertInLoopThread();

  started_ = true;
  for(int i = 0; i < threadNums_; ++i)
  {
    EventLoopThread* loopThread = new EventLoopThread();
    EventLoop* newLoop_ = loopThread->startLoop();
    threads_.push_back(std::unique_ptr(loopThread));
    loops_.push_back(newLoop_);
  }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  baseLoop_->assertInLoopThread();
  EventLoop* loop = baseLoop_;

  if(!loops_.empty())
  {
    loop = loops_[next_];
    next_++;
    if(next_ >= threadNums_)
      next_ = 0;
  }

  return loop;
}
