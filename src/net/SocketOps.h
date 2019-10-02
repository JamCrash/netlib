#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <arpa/inet.h>

namespace netlib
{
namespace socket
{

inline uint16_t networkToHost16(uint16_t net16)
{
  return ntohs(net16);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htons(host16);
}

inline uint32_t networkToHost32(uint32_t net32)
{
  return ntohl(net32);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
  return htonl(host32);
}

int createNonblockingOrAbort();

void bindOrAbort(int sockFd, const struct sockaddr_in& addr);
void listenOrAbort(int sockFd);
int connect(int sockFd, const struct sockaddr_in& addr);
int accept(int sockFd, struct sockaddr_in* addr);
void close(int sockFd);
void shutdownWrite(int sockFd);

void toHostPort(char* buf, size_t size, const struct sockaddr_in& addr);
void fromHostPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

struct sockaddr_in getPeerAddress(int sockFd);
struct sockaddr_in getLocalAddress(int sockFd);

} // socket
} // netlib

#endif  /* SOCKOPS_H */
