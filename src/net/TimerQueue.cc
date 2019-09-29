
#include "src/basics/Logging.h"
#include "src/net/TimerQueue.h"
#include "src/net/EventLoop.h"
#include "src/net/Timer.h"
#include "src/net/Channel.h"

#include <strings.h>
#include <sys/timerfd.h>

using namespace netlib;
using namespace netlib::net;

namespace
{

int createTimerFd()
{
  int timerFd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
  if(timerFd < 0)
  {
    LOG << "create timerFd failed\n";
    abort();
  }
  return timerFd;
}

void readTimerFd(int tmFd)
{
  uint64_t val;
  ssize_t n = ::read(tmFd, &val, sizeof val);
  if(n != sizeof val)
  {
    LOG << "read timerFd failed\n";
  }
}

struct timespec expirationInterval(Timestamp when)
{
  int64_t microSecondsInterval 
    = when.microSecondsFromEpoch() - Timestamp::now().microSecondsFromEpoch();
  if(microSecondsInterval < 100) 
    microSecondsInterval = 100;
  struct timespec result;
  result.tv_sec = microSecondsInterval / Timestamp::microSecondsPerSecond;
  result.tv_nsec = (microSecondsInterval % Timestamp::microSecondsPerSecond) * 1000;
  return result;
}

void resetTimerFd(int tmFd, Timestamp when)
{
  struct itimerspec new_value;
  bzero(&new_value, sizeof new_value);
  new_value.it_value = expirationInterval(when);

  if(timerfd_settime(tmFd, 0, &new_value, NULL) != 0)
  {
    LOG << "reset timerFd failed\n";
  }
  else
  {
    LOG << "timer expiration changed\n";
  }
}

} // namespace

TimerQueue::TimerQueue(EventLoop* loop)
: ownerLoop_(loop),
  timerFd_(createTimerFd()),
  timerChannel_(new Channel(ownerLoop_, timerFd_)),
  callingExpiredTimers_(false)
{
  timerChannel_->setReadCallBack(
    std::bind(&TimerQueue::handleRead, this));
  timerChannel_->setEnableReading();
}

TimerQueue::~TimerQueue()
{
  ownerLoop_->assertInLoopThread();
  for(const TimerEntry& entry: timers_)
  {
    delete entry.second;
  }
}

TimerId TimerQueue::addTimer(CallBack cb, Timestamp expiration, double interval)
{
  Timer* timer = new Timer(std::move(cb), expiration, interval);
  ownerLoop_->runInLoop(
    std::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer);
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  ownerLoop_->assertInLoopThread();

  bool expireChanged = insert(timer);
  if(expireChanged)
  {
    LOG << "expiration changed\n";
    resetTimerFd(timerFd_, timer->expiration());
  }
}

void TimerQueue::handleRead()
{
  ownerLoop_->assertInLoopThread();
  Timestamp now = Timestamp::now();
  readTimerFd(timerFd_);

  callingExpiredTimers_ = true;
  cancelTimers_.clear();
  std::vector<TimerEntry> expiredTimers = getExpired(now);
  assert(expiredTimers.size() > 0);
  for(const TimerEntry& entry: expiredTimers)
  {
    entry.second->run();
  }
  callingExpiredTimers_ = false;

  reset(expiredTimers, now);
}

std::vector<TimerQueue::TimerEntry> TimerQueue::getExpired(Timestamp now)
{
  ownerLoop_->assertInLoopThread();
  std::vector<TimerEntry> expiredTimers;
  
  TimerEntry timerEntry
    = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  TimerSet::iterator it = timers_.lower_bound(timerEntry);
  // FIX ME: now <= it->first ?? 
  assert(it == timers_.end() || now < it->first);
  std::copy(timers_.begin(), it, std::back_inserter(expiredTimers));
  timers_.erase(timers_.begin(), it);

  return expiredTimers;
}

void TimerQueue::reset(const std::vector<TimerEntry>& expiredTimers, Timestamp now)
{
  ownerLoop_->assertInLoopThread();
  Timestamp newExpirTime = Timestamp::invalid();

  for(const TimerEntry& entry: expiredTimers)
  {
    if(entry.second->repeat()
       && cancelTimers_.find(entry) == cancelTimers_.end())
    {
      entry.second->restart(now);
      insert(entry.second);
    }
    else
    {
      delete entry.second;
    } 
  }

  if(!timers_.empty())
  {
    newExpirTime = timers_.begin()->first;
  }
  if(newExpirTime.valid())
  {
    resetTimerFd(timerFd_, newExpirTime);
  }
}

bool TimerQueue::insert(Timer* timer)
{
  ownerLoop_->assertInLoopThread();
  bool expiredChanged = false;

  TimerSet::iterator firstTimer = timers_.begin();
  if(firstTimer == timers_.end() || timer->expiration() < firstTimer->first)
  {
    expiredChanged = true;
  }

  {
    std::pair<TimerSet::iterator, bool> result =
      timers_.insert(std::make_pair(timer->expiration(), timer));
    assert(result.second);
  }

  return expiredChanged;
}

void TimerQueue::cancel(TimerId timerId)
{
  ownerLoop_->runInLoop(
    std::bind(&TimerQueue::cancelInLoop, this, timerId.timer_));
}

void TimerQueue::cancelInLoop(Timer* timer)
{
  ownerLoop_->assertInLoopThread();

  TimerEntry timerEntry
    = std::make_pair(timer->expiration(), timer);
  TimerSet::iterator it
    = timers_.find(timerEntry);
  if(it != timers_.end())
  {
    timers_.erase(it);
  }
  else if(callingExpiredTimers_)
  {
    cancelTimers_.insert(timerEntry);
  }
}
