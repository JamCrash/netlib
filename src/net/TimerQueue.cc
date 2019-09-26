
#include "src/basics/Logging.h"
#include "src/net/TimerQueue.h"
#include "src/net/EventLoop.h"
#include "src/net/Channel.h"

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
  }
  return timerFd;
}

} // namespace

TimerQueue::TimerQueue(EventLoop* loop)
: ownerLoop_(loop),
  timerFd_(createTimerFd()),
  timerChannel_(new Channel(ownerLoop_, timerFd_)),
  cancel_(false)
{
  timerChannel_->setReadCallBack(
    std::bind(&TimerQueue::handleRead, this));
  timerChannel_->setEnableReading();
}

TimerQueue::~TimerQueue()
{

}

void TimerQueue::handleRead()
{
  ownerLoop_->assertInLoopThread();
  
}
