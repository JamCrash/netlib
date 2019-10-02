
#include "src/basics/Logging.h"
#include "src/net/SocketOps.h"
#include "src/net/TcpServer.h"
#include "src/net/EventLoop.h"
#include "src/net/Channel.h"
#include "src/net/Acceptor.h"
#include "src/net/TcpConnection.h"

#include <assert.h>
#include <stdio.h>

using namespace netlib;
using namespace netlib::net;

using std::placeholders::_1;
using std::placeholders::_2;

TcpServer::TcpServer(EventLoop* loop,
                     std::string name, 
                     InetAddress listenAddress)
: loop_(loop),
  name_(name),
  listenAddress_(listenAddress),
  acceptor_(new Acceptor(loop_, listenAddress_)),
  start_(false),
  nextConnId_(1)
{
  acceptor_->setNewConnectionCallBack(
    std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
  assert(!start_);
  start_ = true;

  loop_->runInLoop(
    std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::newConnection(int connFd, const InetAddress& peerAddress)
{
  loop_->assertInLoopThread();

  char buf[32];
  snprintf(buf, sizeof buf, "#%d", nextConnId_);
  nextConnId_++;
  std::string connName = name_ + buf;

  InetAddress localAddress(socket::getLocalAddress(connFd));

  TcpConnectionPtr newConn
    = std::make_shared<TcpConnection>(this,
                                      connFd,
                                      connName,
                                      localAddress,
                                      peerAddress);
  connections_[connName] = newConn;
  newConn->setConnectionCallBack(connectionCallBack_);
  newConn->setMessageCallBack(messageCallBack_);
  newConn->setWriteCompleteCallBack(writeCompleteCallBack_);
  newConn->setCloseCallBack(
    std::bind(&TcpServer::removeConnection, this, _1));
  loop_->runInLoop(
    std::bind(&TcpConnection::connectionEstablished, newConn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
  loop_->runInLoop(
    std::bind(TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
  loop_->assertInLoopThread();
  LOG << "TcpServer connection " << conn->name() << " destroyed\n";
  size_t n = connections_.erase(conn->name());
  assert(n==1);
  loop_->runInLoop(
    std::bind(&TcpConnection::connectionDestroyed, conn));
}
