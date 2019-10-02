#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "src/basics/noncopyable.h"
#include "src/net/Callbacks.h"
#include "src/net/Buffer.h"
#include "src/net/InetAddress.h"

#include <string>
#include <memory>

namespace netlib
{
namespace net
{

class EventLoop;
class Socket;
class Channel;

class TcpConnection: netlib::noncopyable,
                     public std::enable_shared_from_this<TcpConnection>
{
 public:
  TcpConnection(EventLoop* loop,
                int sockFd,
                std::string name,
                const InetAddress& localAddress,
                const InetAddress& peerAddress);

  ~TcpConnection();

  std::string name() const { return name_; }

  void connectionEstablished();
  void connectionDestroyed();

  void shutdown();

  void send(const std::string& message);

  void setConnectionCallBack(const ConnectionCallBack& cb)
  { connectionCallBack_ = cb; }
  void setMessageCallBack(const MessageCallBack& cb)
  { messageCallBack_ = cb; }
  void setCloseCallBack(const CloseCallBack& cb)
  { closeCallBack_ = cb; }
  void errorCallBack(const ErrorCallBack& cb)
  { errorCallBack_ = cb; }
  void setWriteCompleteCallBack(const WriteCompleteCallBack& cb)
  { writeCompleteCallBack_ = cb; }

 private:
  enum { CONNECTING, CONNECTED, DISCONNECTING, DISCONNECTED };

  void handleRead();
  void handleWrite();
  void handleError();
  void handleClose();

  void sendInLoop(const std::string& message);

  void shutdownInLoop();

  EventLoop* loop_;
  std::string name_;
  const InetAddress localAddress_;
  const InetAddress peerAddress_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  int state_;

  Buffer inputBuffer;
  Buffer outputBuffer;

  ConnectionCallBack connectionCallBack_;
  MessageCallBack    messageCallBack_;
  CloseCallBack      closeCallBack_;
  ErrorCallBack      errorCallBack_;
  WriteCompleteCallBack writeCompleteCallBack_;
};

} // net
} // netlib

#endif  /* TCPCONNECTION_H */
