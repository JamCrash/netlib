#ifndef BUFFER_H
#define BUFFER_H

#include "../basics/copyable.h"

#include <vector>
#include <string>
#include <algorithm>

#include <assert.h>

namespace netlib
{
namespace net
{

class Buffer: public netlib::copyable {
 public:
  static const size_t prependSize = 8;
  static const size_t initialSize = 1024;

  explicit Buffer(size_t InitSize = initialSize)
  : buffer_(prependSize + InitSize),
    readerIndex_(prependSize),
    writerIndex_(prependSize)
  {
    
  }

  size_t readableBytes() const 
  { return writerIndex_ - readerIndex_; }

  size_t writeableBytes() const 
  { return buffer_.size() - writerIndex_; }

  size_t prependableBytes() const 
  { return readerIndex_; }

  const char* peek() const
  { return begin() + readerIndex_; }

  void retrieve(size_t len)
  {
    assert(len <= readableBytes());
    if(len < readableBytes())
    {
      readerIndex_ += len;
    }
    else
    {
      retrieveAll();
    }
  }

  void retrieveAll()
  {
    readerIndex_ = writerIndex_ = prependSize;
  }

  std::string retrieveAllAsString()
  { return retrieveAsString(readableBytes()); }

  std::string retrieveAsString(size_t len)
  { 
    assert(len <= readableBytes());
    std::string ret(peek(), len);
    retrieve(len);
    return ret;
  }

  void append(const char* data, size_t len)
  {
    ensureReadableBytes(len);
    std::copy(data, data+len, beginWrite());
    hasWritten(len);
  }

  void append(const void* data, size_t len)
  {
    append(static_cast<const char*>(data), len);
  }

  void ensureReadableBytes(size_t len)
  {
    if(writeableBytes() < len)
    {
      makeSpace(len);
    }
    assert(writeableBytes() >= len);
  }

  char* beginWrite()
  { return begin() + writerIndex_; }

  void hasWritten(size_t len)
  {
    assert(len <= writeableBytes());
    writerIndex_ += len;
  }

  void prepend(const void* data, size_t len)
  {
    assert(len <= prependableBytes());
    const char* d = static_cast<const char*>(data);
    readerIndex_ -= len;
    std::copy(d, d+len, begin()+readerIndex_);
  }

  ssize_t readFd(int fd, int* savedErrno);

 private:
  char* begin()
  { return buffer_.data(); }

  const char* begin() const 
  { return buffer_.data(); }

  void makeSpace(size_t len)
  {
    if(writeableBytes() + prependableBytes() < len + prependSize)
    {
      buffer_.resize(writerIndex_ + len);
    }
    else 
    {
      assert(prependSize < readerIndex_);
      const size_t readable = readableBytes();
      std::copy(begin() + readerIndex_,
                begin() + writerIndex_,
                begin() + prependSize);
      readerIndex_ = prependSize;
      writerIndex_ = prependSize + readable;
      assert(readableBytes() == readable);
    }
  }
 
 private:
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;
};

} // net
} // netlib

#endif  /* BUFFER_H */
