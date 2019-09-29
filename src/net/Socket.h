#ifndef SOCKET_H
#define SOCKET_H

#include "src/basics/copyable.h"

namespace netlib
{
namespace net
{

class InetAddress;

class Socket: netlib::copyable {
 public:
  explicit Socket(int sockFd)
  : sockFd_(sockFd)
  { }

  ~Socket();

  int fd() const { return sockFd_; }

  void bindAddress(const InetAddress& localAddress);
  void listen();

  int accept(InetAddress* peerAddress);

 private:
  const int sockFd_;
};

} // net
} // netlib

#endif  /* SOCKET_H */
