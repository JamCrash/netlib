
#include "src/net/InetAddress.h"
#include "src/net/SocketOps.h"

#include <strings.h>

using namespace netlib;
using namespace net;

InetAddress::InetAddress(uint16_t port)
{
  bzero(&addr_, sizeof addr_);
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = socket::hostToNetwork32(INADDR_ANY);
  addr_.sin_port = socket::hostToNetwork16(port);
}

InetAddress::InetAddress(const char* ip, uint16_t port)
{
  bzero(&addr_, sizeof addr_);
  socket::fromHostPort(ip, port, &addr_);
}

std::string InetAddress::toHostPort() const 
{
  char buf[32];
  socket::toHostPort(buf, sizeof buf, addr_);
  return buf;
}
