#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "src/basics/noncopyable.h"
#include "src/basics/Timestamp.h"

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
 public:
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

 private:
  typedef std::pair<Timestamp, Timer*> TimerEntry;
  typedef std::set<TimerEntry> TimerSet;
  typedef std::pair<Timer*, int64_t> CancelTimerEntry;
  typedef std::set<CancelTimerEntry> CancelTimerSet;

  void handleRead();


  EventLoop* ownerLoop_;
  const int timerFd_;
  std::unique_ptr<Channel> timerChannel_;
  bool cancel_;
  CancelTimerSet cancelTimers_;
  TimerSet timers_;
};

} // net
} // netlib

#endif  /* TIMERQUEUE_H */
