#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "src/basics/noncopyable.h"
#include "src/basics/Thread.h"
#include "src/basics/Mutex.h"
#include "src/net/TimerId.h"

#include <vector>
#include <memory>

namespace netlib
{
namespace net
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop: netlib::noncopyable {
 public:
  typedef std::vector<Channel*> ChannelList;
  typedef std::function<void()> Functor;
  typedef Functor TimerCallBack;

  EventLoop();
  ~EventLoop();

  void loop();
  void quit();

  void runInLoop(const Functor& func);
  void queueInLoop(const Functor& func);

  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  // timer setting functions
  TimerId runAt(const Timestamp& when, const TimerCallBack& cb);
  TimerId runAfter(double delay, const TimerCallBack& cb);
  TimerId runEvery(double interval, const TimerCallBack& cb);

  bool isInLoopThread() { return loopThreadId_ == CurrentThread::tid(); }
  void assertInLoopThread()
  {
    if(!isInLoopThread())
    {
      abortNotInLoop();
    }
  }

 private:

  void abortNotInLoop();

  // handle read event of wakeupFd_
  void handleRead();
  // write to wakeupFd_ to wakeup EventLoop;
  void wakeup();

  void doFunctors();

  bool looping_;
  bool quit_;
  const pid_t loopThreadId_;
  std::unique_ptr<Poller> poller_;
  const int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;
  ChannelList activeChannels_;

  MutexLock mutexLock_;
  std::vector<Functor> pendingFunctors_;
  bool callingPendingFunctor_;

  std::unique_ptr<TimerQueue> timers_;
};

} // net
} // netlib

#endif  /* EVENTLOOP_H */
