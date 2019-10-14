#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "src/basics/noncopyable.h"
#include "src/net/Callbacks.h"
#include "src/net/InetAddress.h"

#include <string>
#include <memory>
#include <map>

namespace netlib
{
namespace net
{

class EventLoop;
class TcpConnection;
class Acceptor;
class EventLoopThreadPool;

class TcpServer: netlib::noncopyable {
 public:
  TcpServer(EventLoop* loop,
         std::string name,
         InetAddress listenAddress);
  
  ~TcpServer();

  void setThreadNums(int nums);

  void start();

  void setConnectionCallBack(const ConnectionCallBack& cb)
  { connectionCallBack_ = cb; }
  void setMessageCallBack(const MessageCallBack& cb)
  { messageCallBack_ = cb; }
  void setWriteCompleteCallBack(const WriteCompleteCallBack& cb)
  { writeCompleteCallBack_ = cb; }

 private:
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

  void newConnection(int connFd, const InetAddress& peerAddress);
  void removeConnection(const TcpConnectionPtr& conn);
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

  EventLoop* loop_;
  const std::string name_;
  const InetAddress listenAddress_;
  std::unique_ptr<Acceptor> acceptor_;
  bool start_;
  int nextConnId_;
  ConnectionMap connections_;
  std::unique_ptr<EventLoopThreadPool> threadPool_;    // thread pool

  ConnectionCallBack        connectionCallBack_;
  MessageCallBack           messageCallBack_;
  WriteCompleteCallBack     writeCompleteCallBack_;
};

} // net
} // netlib

#endif  /* TCPSERVER_H */
