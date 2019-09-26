#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "src/basics/noncopyable.h"
#include "src/basics/Thread.h"

#include <vector>
#include <memory>

namespace netlib
{
namespace net
{

class Channel;
class Poller;

class EventLoop: netlib::noncopyable {
 public:
  typedef std::vector<Channel*> ChannelList;

  EventLoop();
  ~EventLoop();

  void loop();
  void quit();

  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  bool isInLoopThread() { return loopThreadId_ == CurrentThread::tid(); }
  void assertInLoopThread()
  {
    if(!isInLoopThread())
    {
      abortNotInLoop();
    }
  }

 private:
  typedef std::vector<Channel*> ChannelList;

  void abortNotInLoop();

  // handle read event of wakeupFd_
  void handleRead();
  // write to wakeupFd_ to wakeup EventLoop;
  void wakeup();

  bool looping_;
  bool quit_;
  const pid_t loopThreadId_;
  std::unique_ptr<Poller> poller_;
  const int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;
  ChannelList activeChannels_;
};

} // net
} // netlib

#endif  /* EVENTLOOP_H */
