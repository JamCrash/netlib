#ifndef INETADDRESS_H
#define INETADDRESS_H

#include "src/basics/copyable.h"

#include <arpa/inet.h>

#include <string>

namespace netlib
{
namespace net
{

class InetAddress: netlib::copyable {
 public:
  explicit InetAddress(uint16_t port);
  InetAddress(const char* ip, uint16_t port);
  InetAddress(const sockaddr_in& addr)
  : addr_(addr) { }

  void setSockAddress(const struct sockaddr_in& addr) { addr_ = addr; }

  std::string toHostPort() const;

  const sockaddr_in& getAddr() const { return addr_; }
  
 private:
  struct sockaddr_in addr_;
};

} // net
} // netlib

#endif  /* INETADDRESS_H */
