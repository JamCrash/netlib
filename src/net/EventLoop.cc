
#include "src/basics/Logging.h"
#include "src/net/EventLoop.h"
#include "src/net/Channel.h"
#include "src/net/Poller.h"

#include <assert.h>
#include <sys/eventfd.h>

namespace netlib
{
namespace
{

int createEventFd()
{
  int fd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
  if(fd < 0)
  {
    LOG << "create event fd failed\n";
    abort();
  }
  return fd;
}

}
} // netlib

using namespace netlib;
using namespace netlib::net;

__thread EventLoop* currentThreadLoop = 0;
const int timeOutMs = 10000;

EventLoop::EventLoop()
: looping_(false),
  quit_(false),
  loopThreadId_(netlib::CurrentThread::tid()),
  poller_(new Poller(this)),
  wakeupFd_(createEventFd()),
  wakeupChannel_(new Channel(this, wakeupFd_))
{
  if(currentThreadLoop != NULL)
  {
    LOG << "already exist EvnetLoop in current Thread\n";
    abort();
  }
  currentThreadLoop = this;

  wakeupChannel_->setReadCallBack(
    std::bind(&EventLoop::handleRead, this));
  wakeupChannel_->setEnableReading();
}

EventLoop::~EventLoop()
{
  assert(quit_);
}

void EventLoop::loop()
{
  assertInLoopThread();
  assert(!looping_);
  quit_ = false;
  looping_ = true;

  while(!quit_)
  {
    activeChannels_.clear();
    poller_->poll(timeOutMs, &activeChannels_);
    for(ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
      (*it)->handleEvent();
    }
  }

  LOG << "eventloop stop looping\n";
  looping_ = false;
}

void EventLoop::quit()
{
  quit_ = true;
  wakeup();
}

void EventLoop::wakeup()
{
  uint64_t val = 1;
  ssize_t n = write(wakeupFd_, &val, sizeof val);
  if(n != sizeof val)
  {
    LOG << "EventLoop wakeup failed\n";
  }
}

void EventLoop::handleRead()
{
  uint64_t val;
  ssize_t n = read(wakeupFd_, &val, sizeof val);
  if(n != sizeof val)
  {
    LOG << "EventLoop wakeup failed\n";
  }
}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->removeChannel(channel);
}

void EventLoop::abortNotInLoop()
{
  LOG << "ThreadLoop abort\n";
  abort();
}
