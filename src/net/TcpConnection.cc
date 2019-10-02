
#include "src/basics/Logging.h"
#include "src/net/Socket.h"
#include "src/net/SocketOps.h"
#include "src/net/EventLoop.h"
#include "src/net/TcpConnection.h"
#include "src/net/Channel.h"

#include <assert.h>

using namespace netlib;
using namespace net;

TcpConnection::TcpConnection(EventLoop* loop,
                             int sockFd,
                             std::string name,
                             const InetAddress& localAddress,
                             const InetAddress& peerAddress)
: loop_(loop),
  name_(name),
  localAddress_(localAddress),
  peerAddress_(peerAddress),
  socket_(new Socket(sockFd)),
  channel_(new Channel(loop_, sockFd)),
  state_(CONNECTING)
{
  channel_->setReadCallBack(
    std::bind(&TcpConnection::handleRead, this));
  channel_->setWriteCallBack(
    std::bind(&TcpConnection::handleWrite, this));
  channel_->setErrorCallBack(
    std::bind(&TcpConnection::handleError, this));
  channel_->setCloseCallBack(
    std::bind(&TcpConnection::handleClose,this));
}

TcpConnection::~TcpConnection()
{
  LOG << "TcpConnection dtor\n";
}

void TcpConnection::connectionEstablished()
{
  loop_->assertInLoopThread();
  assert(CONNECTING);
  state_ = CONNECTED;
  channel_->setEnableReading();
  connectionCallBack_(shared_from_this());
}

void TcpConnection::connectionDestroyed()
{
  loop_->assertInLoopThread();
  assert(state_ == CONNECTED || state_ == DISCONNECTING);
  state_ = DISCONNECTED;
  channel_->disableAll();
  connectionCallBack_(shared_from_this());
  loop_->removeChannel(channel_.get());
}

void TcpConnection::handleRead()
{
  int savedErrno = 0;
  ssize_t n = inputBuffer.readFd(socket_->fd(), &savedErrno);
  if(n > 0)
  {
    if(messageCallBack_)
      messageCallBack_(shared_from_this(), &inputBuffer, Timestamp::now());
  }
  else if(n == 0)
  {
    if(closeCallBack_)
      closeCallBack_(shared_from_this());
  }
  else 
  {
    errno = savedErrno;
    LOG << "Connection error\n";
    if(errorCallBack_)
      errorCallBack_(shared_from_this());
  }
}

void TcpConnection::send(const std::string& message)
{
  if(loop_->isInLoopThread()) {
    sendInLoop(message);
  }
  else {
    loop_->runInLoop(
      std::bind(&TcpConnection::sendInLoop, this, message));
  }
}

void TcpConnection::sendInLoop(const std::string& message)
{
  loop_->assertInLoopThread();

  ssize_t n = 0;
  if(!channel_->isWriting() && outputBuffer.readableBytes() == 0)
  {
    n = ::write(channel_->fd(), message.data(), message.size());
    if(n >= 0)
    {
      if(static_cast<ssize_t>(message.size()) == n && writeCompleteCallBack_)
      {
        writeCompleteCallBack_(shared_from_this());
      }
    }
    else
    {
      n = 0;
      if(errno != EAGAIN)
        LOG << "TcpConnection write error\n";
    }
  }

  assert(n>=0);
  if(static_cast<ssize_t>(message.size()) > n)
  {
    outputBuffer.append(message.data()+n, message.size() - n);
    if(!channel_->isWriting())
      channel_->setEnableReading();
  }
}

void TcpConnection::handleWrite()
{
  loop_->assertInLoopThread();

  if(channel_->isWriting())
  {
    ssize_t n = ::write(socket_->fd(),
                        outputBuffer.peek(),
                        outputBuffer.readableBytes());
    if(n > 0)
    {
      outputBuffer.retrieve(n);
      if(outputBuffer.readableBytes() == 0)
      {
        channel_->disableWriting();
        if(state_ == DISCONNECTING)
          shutdownInLoop();
      }
    }
    else 
    {
      LOG << "TcpConnection Write error\n";
    }
  }
}

void TcpConnection::shutdown()
{
  if(state_ = CONNECTED)
  {
    state_ = DISCONNECTING;
    loop_->runInLoop(
      std::bind(&TcpConnection::shutdownInLoop, this));
  }
}

void TcpConnection::shutdownInLoop()
{
  assert(state_ = DISCONNECTING);
  if(!channel_->isWriting())
  {
    socket::shutdownWrite(socket_->fd());
  }
}

void TcpConnection::handleError()
{
  if(errorCallBack_)
    errorCallBack_(shared_from_this());
}

void TcpConnection::handleClose()
{
  loop_->assertInLoopThread();
  channel_->disableAll();
  if(closeCallBack_)
    closeCallBack_(shared_from_this());
}
