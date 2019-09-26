#ifndef POLLER_H
#define POLLER_H

#include "src/basics/noncopyable.h"

#include <vector>
#include <map>

struct pollfd;

namespace netlib
{
namespace net
{

class EventLoop;
class Channel;

class Poller: netlib::noncopyable {
 public:
  typedef std::vector<Channel*> ChannelList;
  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  Poller(EventLoop* loop);
  ~Poller();

  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  void poll(int timeoutMs, ChannelList* activeChannels);

 private:
  void fillActiveChannels(ChannelList* activeChannels, int activeNums);

  EventLoop* ownerLoop_;
  PollFdList pfds_;
  ChannelMap channels_;
};

} //net
} //netlib

#endif  /* POLLER_H */
