
#include "src/net/SocketOps.h"
#include "src/net/Socket.h"

using namespace netlib;
using namespace net;

Socket::~Socket()
{
  socket::close(sockFd_);
}

void Socket::bindAddress(const InetAddress& localAddr)
{

}

void Socket::listen()
{
  socket::listenOrAbort(sockFd_);
}

int Socket::accept(InetAddress* peerAddr)
{

}
