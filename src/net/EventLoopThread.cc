
#include "src/net/EventLoopThread.h"
#include "src/net/EventLoop.h"

#include <assert.h>

using namespace netlib;
using namespace netlib::net;

EventLoopThread::EventLoopThread()
: loop_(NULL),
  exiting_(false),
  thread_(std::bind(EventLoopThread::threadFunc, this)),
  lock_(),
  cond_(lock_)
{}

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  loop_->quit();
  thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  EventLoop* loop;
  thread_.start();

  {
    MutexLockGuard  guard(lock_);
    while(loop_ == NULL)
    {
      cond_.wait();
    }
  }

  return loop;
}

void EventLoopThread::threadFunc()
{
  EventLoop loop;

  {
    MutexLockGuard guard(lock_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
}
