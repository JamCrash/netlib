#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "src/basics/noncopyable.h"
#include "src/net/InetAddress.h"
#include "src/net/Callbacks.h"

#include <memory>

namespace netlib
{
namespace net
{

class Socket;
class Channel;
class EventLoop;

class TcpConnection: netlib::noncopyable {
 public:
  
 private:
  EventLoop* loop_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  InetAddress peerAddr_;
  InetAddress localAddr_;  

  ConnectionCallBack connectionCallBack_;
  MessageCallBack    messageCallBack_;
  ErrorCallBack      errorCallBack_;
  WriteCompleteCallBack writeCompleteCallBack_;
};

} // net
} // netlib

#endif  /* TCPCONNECTION_H */
