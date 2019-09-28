
#include "src/basics/Logging.h"
#include "src/net/Channel.h"
#include "src/net/EventLoop.h"

#include <poll.h>

using namespace netlib;
using namespace netlib::net;

const int Channel::noEvent = 0;
const int Channel::readEvent = POLLIN | POLLPRI;
const int Channel::writeEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
: loop_(loop),
  fd_(fd),
  events_(0),
  revents_(0),
  index_(-1),
  handlingEvent_(false)
{
}

Channel::~Channel()
{
  assert(!handlingEvent_);
}

void Channel::update()
{
  loop_->updateChannel(this);
}

void Channel::handleEvent()
{
  handlingEvent_ = true;
  if(revents_ & POLLNVAL)
  {
    LOG << "Channel POLLNVAL\n";
  }

  if((revents_ & POLLHUP) && (revents_ & POLLIN))
  {
    if(closeCallBack_)
    {
      closeCallBack_();
    }
  }
  if(revents_ & (POLLERR | POLLNVAL))
  {
    if(errorCallBack_)
    {
      errorCallBack_();
    }
  }
  if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if(readCallBack_)
    {
      readCallBack_();
    }
  }
  if(revents_ & POLLOUT)
  {
    if(writeCallBack_)
    {
      writeCallBack_();
    }
  }
  handlingEvent_ = false;
}
