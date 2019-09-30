
#include "src/net/EventLoop.h"
#include "src/net/Acceptor.h"
#include "src/net/SocketOps.h"

#include <assert.h>

using namespace netlib;
using namespace netlib::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
: loop_(loop),
  listenSocket_(socket::createNonblockingOrAbort()),
  channel_(loop_, listenSocket_.fd()),
  listening_(false)
{
  listenSocket_.bindAddress(listenAddr);
  channel_.setReadCallBack(
    std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen()
{
  assert(!listening_);
  listening_ = true;
  listenSocket_.listen();
  channel_.setEnableReading();
}

void Acceptor::handleRead()
{
  loop_->assertInLoopThread();
  InetAddress peerAddr(0);
  int connFd = listenSocket_.accept(&peerAddr);
  if(connFd >= 0) 
  {
    if(newConnectionCallBack_)
    {
      newConnectionCallBack_(connFd, peerAddr);
    }
    else
    {
      close(connFd);
    }
  }
}
