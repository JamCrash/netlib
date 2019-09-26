
#include "Buffer.h"

#include <sys/uio.h>
#include <errno.h>

namespace netlib
{
namespace net
{

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  char tmpBuffer[65536];
  struct iovec vec[2];
  const size_t writeable = writeableBytes();
  vec[0].iov_base = begin() + writerIndex_;
  vec[0].iov_len = writeable;
  vec[1].iov_base = tmpBuffer;
  vec[1].iov_len = sizeof tmpBuffer;
  
  const int iovcnt = (writeable < sizeof tmpBuffer ? 2 : 1);
  const ssize_t n = readv(fd, vec, iovcnt);
  if(n < 0) 
  {
    *savedErrno = errno;
  }
  else if(static_cast<size_t>(n) <= writeable)
  {
    writerIndex_ += n;
  }
  else
  {
    writerIndex_ = buffer_.size();
    append(tmpBuffer, n-writeable);
  }
}

} // net
} // netlib