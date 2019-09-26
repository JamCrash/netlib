#ifndef ATOMIC_H
#define ATOMIC_H

#include "src/basics/noncopyable.h"
#include "src/basics/Mutex.h"

#include <stdint.h>

namespace netlib
{

template<typename T>
class AtomicInteger: netlib::noncopyable {
 public:
  AtomicInteger()
  : value_(0)
  {
  {

  T get() const { return value_; }

  T getAndAdd(T x)
  {
    MutexLockGuard(lock_);
    T tmpval = value_;
    value_ = value_ + x;
    return tmpval;
  }

  T addAndGet(T x)
  {
    return getAndAdd(x) + x;
  }

  T incrementAndGet()
  {
    return addAndGet(1);
  }

 private:
  volatile T value_;
  MutexLock lock_;
};

typedef AtomicInteger<uint64_t> AtomicInteger64;
typedef AtomicInteger<uint32_t> AtomicInteger32;

} // netlib

#endif  /* ATOMIC_H */
