
#include "src/net/InetAddress.h"
#include "src/net/SocketOps.h"
#include "src/net/Socket.h"

#include <strings.h>

using namespace netlib;
using namespace net;

Socket::~Socket()
{
  socket::close(sockFd_);
}

void Socket::bindAddress(const InetAddress& listenAddr)
{
  struct sockaddr_in addr = listenAddr.getAddr();
  socket::bindOrAbort(sockFd_, addr);
}

void Socket::listen()
{
  socket::listenOrAbort(sockFd_);
}

int Socket::accept(InetAddress* peerAddr)
{
  struct sockaddr_in addr;
  bzero(&addr, sizeof addr);
  int connFd = socket::accept(sockFd_, &addr);
  if(connFd >= 0) 
  {
    peerAddr->setSockAddress(addr);
  }
  return connFd;
}
