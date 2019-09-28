#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "src/basics/noncopyable.h"
#include "src/basics/Timestamp.h"
#include "src/net/TimerId.h"

#include <memory>
#include <set>
#include <vector>

namespace netlib
{
namespace net
{

class EventLoop;
class Timer;
class Channel;

class TimerQueue: netlib::noncopyable {
  typedef std::function<void()> CallBack;
 public:
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

  TimerId addTimer(CallBack cb, Timestamp expiration, double interval);
  void cancel(TimerId timerId);

 private:
  typedef std::pair<Timestamp, Timer*> TimerEntry;
  typedef std::set<TimerEntry> TimerSet;

  void addTimerInLoop(Timer* timer);
  void handleRead();
  std::vector<TimerEntry> getExpired(Timestamp now);
  void reset(const std::vector<TimerEntry>& expiredTimers, Timestamp now);
  bool insert(Timer* timer);
  void cancelInLoop(Timer* timer);

  EventLoop* ownerLoop_;
  const int timerFd_;
  std::unique_ptr<Channel> timerChannel_;
  // to process the case when timer cancel itself in callback function
  bool callingExpiredTimers_;
  TimerSet cancelTimers_;
  TimerSet timers_;
};

} // net
} // netlib

#endif  /* TIMERQUEUE_H */
