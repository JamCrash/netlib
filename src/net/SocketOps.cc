
#include "src/basics/Logging.h"
#include "src/net/SocketOps.h"

#include <assert.h>
#include <unistd.h>

using namespace netlib;

namespace 
{
  typedef struct sockaddr SA;
}

int socket::createNonblockingOrAbort()
{
  int socketFd = ::socket(AF_INET, 
                          SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                          IPPROTO_TCP);
  if(socketFd < 0)
  {
    LOG << "create socket failed\n";
    abort();
  }
  return socketFd;
}

void socket::bindOrAbort(int sockFd, const struct sockaddr_in& addr)
{
  if(::bind(sockFd, reinterpret_cast<const SA*>(&addr), sizeof addr) < 0)
  {
    LOG << "bind socket failed\n";
    abort();
  }
}

void socket::listenOrAbort(int sockFd)
{
  if(::listen(sockFd, SOMAXCONN) < 0)
  {
    LOG << "listen socket failed\n";
    abort();
  }
}

int socket::connect(int sockFd, const struct sockaddr_in& addr)
{
  return ::connect(sockFd, reinterpret_cast<const SA*>(&addr), sizeof addr);
}

int socket::accept(int sockFd, struct sockaddr_in* addrPtr)
{
  socklen_t addrLen = sizeof *addrPtr;
  int connFd = accept4(sockFd, 
                       reinterpret_cast<SA*>(addrPtr),
                       &addrLen,
                       SOCK_NONBLOCK | SOCK_CLOEXEC);
  if(connFd < 0)
  {
    int savedErrno = errno;
    LOG << "socket::accept error\n";
    switch(savedErrno) 
    {
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPERM:
        errno = savedErrno;
        break;
      default:
        LOG << "socket::accept fatal error\n";
        abort();
    }
  }
  return connFd;
}

void socket::close(int sockFd)
{
  if(::close(sockFd) < 0)
  {
    LOG << "close socket failed\n";
    abort();
  }
}

void socket::shutdownWrite(int sockFd)
{
  if(::shutdown(sockFd, SHUT_WR) < 0)
  {
    LOG << "shutdown socket writing failed\n";
    abort();
  }
}

void socket::toHostPort(char* buf, size_t size, const struct sockaddr_in& addr)
{
  char host[INET_ADDRSTRLEN] = "UNKNOWN";
  ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
  uint16_t port = socket::networkToHost16(addr.sin_port);
  snprintf(buf, size, "%s:%u", host, port);
}

void socket::fromHostPort(const char* ip, uint16_t port, struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = socket::hostToNetwork16(port);
  if(::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    LOG << "socket::fromHostPort\n";
  }
}
