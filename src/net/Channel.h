#ifndef CHANNEL_H
#define CHANNEL_H

#include "src/basics/noncopyable.h"

#include <functional>

namespace netlib
{
namespace net
{

class EventLoop;

class Channel: netlib::noncopyable {
 public:
  typedef std::function<void()> EventCallBack;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  EventLoop* ownerLoop() const 
  { return loop_; }

  void handleEvent();
  void setReadCallBack(const EventCallBack& cb)
  { readCallBack_ = cb; }
  void setWriteCallBack(const EventCallBack& cb)
  { writeCallBack_ = cb; }
  void setCloseCallBack(const EventCallBack& cb)
  { closeCallBack_ = cb; }
  void setErrorCallBack(const EventCallBack& cb)
  { errorCallBack_ = cb; }

  void setIndex(int index) 
  { index_ = index; }
  int index() const 
  { return index_; }

  int fd() const { return fd_; }
  int events() const { return events_; }
  int revents() const { return revents_; }
  void setRevents(int revents) 
  { revents_ = revents; }

  void update();
  void setEnableReading()
  { events_ |= readEvent; update(); }
  void setEnableWriting()
  { events_ |= writeEvent; update(); }
  void disableWriting()
  { events_ &= ~writeEvent; update(); }
  void disableAll()
  { events_ = noEvent; update(); }
  bool isNoEvents() const 
  { return events_ == noEvent; };
  bool isWriting() const 
  { return events & writeEvent; }

 private:
  EventLoop* loop_;
  int fd_;
  int events_;
  int revents_;
  int index_; //for poller
  bool handlingEvent_;

  static const int noEvent;
  static const int readEvent;
  static const int writeEvent;
  
  EventCallBack readCallBack_;
  EventCallBack writeCallBack_;
  EventCallBack errorCallBack_;
  EventCallBack closeCallBack_;
};

} // net
} // netlib

#endif  /* CHANNEL_H */
