#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "src/basics/noncopyable.h"
#include "src/net/Socket.h"
#include "src/net/Channel.h"
#include "src/net/InetAddress.h"

namespace netlib
{
namespace net
{

class EventLoop;
class TcpConnection;

class Acceptor: netlib::noncopyable {
 public:
  typedef 
    std::function<void(int connFd, const InetAddress& peerAddr)> 
      NewConnectionCallBack;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr);

  void listen();
  bool listening() const { return listening_; }

  void setNewConnectionCallBack(const NewConnectionCallBack& cb)
  { newConnectionCallBack_ = cb; }

 private:
  void handleRead();

  EventLoop* loop_;
  Socket listenSocket_;
  Channel channel_;
  bool listening_;
  NewConnectionCallBack newConnectionCallBack_;
};

}
}

#endif  /* ACCEPTOR_H */
