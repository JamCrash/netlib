
#include "src/basics/Logging.h"
#include "src/net/EventLoop.h"
#include "src/net/Poller.h"
#include "src/net/Channel.h"

#include <assert.h>
#include <poll.h>

using namespace netlib;
using namespace netlib::net;

Poller::Poller(EventLoop* loop)
: ownerLoop_(loop)
{

}

Poller::~Poller()
{

}

void Poller::updateChannel(Channel* channel)
{
  ownerLoop_->assertInLoopThread();
  if(channel->index() < 0)
  {
    // new one
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    pfd.fd = channel->fd();
    pfds_.push_back(pfd);
    channel->setIndex(static_cast<int>(pfds_.size()) - 1);
    channels_[channel->fd()] = channel;
  }
  else
  {
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    assert(0<= idx && idx < static_cast<int>(pfds_.size()));
    struct pollfd& pfd = pfds_[idx];
    assert(channel->fd() == pfd.fd || channel->fd == -pfd.fd - 1);
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    if(channel->isNoEvents())
    {
      pfd.fd = -channel->fd - 1;
    }
    else
    {
      pfd.fd = channel->fd;
    }
    
  }
}

void Poller::removeChannel(Channel* channel)
{
  ownerLoop_->assertInLoopThread();
  assert(channels_.find(channel->fd()) != channels_.end());
  assert(channels_[channel->fd()] == channel);
  assert(!channel->isNoEvents());
  int idx = channel->index();
  assert(0 <= idx && idx < static_cast<int>(pfds_.size()));
  const struct pollfd& pfd = pfds_[idx];
  assert(pfd.fd == -channel->fd() - 1 && pfd.events == channel->events());
  channels_.erase(channel->fd());
  if(idx == static_cast<int>(pfds_.size() - 1))
  {
    pfds_.pop_back();
  }
  else
  {
    int channelAtEnd = pfds_.back().fd;
    std::iter_swap(pfds_.begin()+idx, pfds_.end()-1);
    if(channelAtEnd < 0)
    {
      channelAtEnd = -channelAtEnd - 1;
    }
    channels_[channelAtEnd] = channel;
    pfds_.pop_back();
  }
}

void Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
  ownerLoop_->assertInLoopThread();
  int activeNums = ::poll(pfds_.data(), pfds_.size(), timeoutMs);
  if(activeNums < 0)
  {
    LOG << "poll error\n";
  }
  else if(activeNums == 0)
  {
    LOG << "nothing happen\n";
  }
  else
  {
    LOG << activeNums << "occur\n";
    fillActiveChannels(activeChannels, activeNums);
  }
}

void Poller::fillActiveChannels(ChannelList* activeChannels, int activeNums)
{
  for(PollFdList::iterator it = pfds_.begin();
      it != pfds_.end() && activeNums > 0; ++it)
  {
    if(it->revents > 0)
    {
      --activeNums;
      ChannelMap::iterator ch = channels_.find(it->fd);
      assert(ch != channels_.end());
      Channel* channel = ch->second;
      assert(channel->fd() == it->fd);
      channel->setRevents(static_cast<int>(it->revents));
      activeChannels->push_back(channel);
    }
  }
}
