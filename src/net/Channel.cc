
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
  index_(-1)
{

}

Channel::~Channel()
{

}

void Channel::update()
{
  loop_->updateChannel(this);
}

void Channel::handleEvent()
{

}
