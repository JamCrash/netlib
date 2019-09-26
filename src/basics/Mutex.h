#ifndef MUTEX_H
#define MUTEX_H

#include "src/basics/noncopyable.h"
#include "src/basics/Thread.h"

#include <assert.h>
#include <pthread.h>

namespace netlib
{

class MutexLock: netlib::noncopyable {
 public:
  MutexLock()
  : holder(0)
  {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock()
  {
    assert(holder == 0);
    pthread_mutex_destroy(&mutex_);
  }

  bool isLockedByCurrentThread()
  { return holder == CurrentThread::tid(); }

  void assertLockedByCurrentThread()
  { assert(isLockedByCurrentThread()); }

  void lock()
  {
    holder = CurrentThread::tid();
    pthread_mutex_lock(&mutex_);
  }

  void unlock()
  {
    holder = 0;
    pthread_mutex_unlock(&mutex_);
  }

 private:
  pthread_mutex_t mutex_;
  pid_t holder;
};

class MutexLockGuard: netlib::noncopyable {
 public:
  explicit MutexLockGuard(MutexLock& mutexLock)
  : mutexLock_(mutexLock)
  {
    mutexLock_.lock();
  }

  ~MutexLockGuard()
  {
    mutexLock_.unlock();
  }

 private:
  MutexLock& mutexLock_;
};

#define MutexLockGuard(x) error "Missing guard object name"

} // netlib

#endif  /* MUTEX_H */
