#ifndef CONDITION_H
#define CONDITION_H

#include "src/basics/noncopyable.h"
#include "src/basics/Mutex.h"

#include <pthread.h>

namespace netlib
{

class Condition: netlib::noncopyable {
 public:
  explicit Condition(MutexLock& lock)
  : lock_(lock)
  {
    pthread_cond_init(&cond_, NULL);
  }

  ~Condition()
  {
    pthread_cond_destroy(&cond_);
  }

  void wait()
  {
    pthread_cond_wait(&cond_, lock_.getMutexLock());
  }

  void notify()
  {
    pthread_cond_signal(&cond_);
  }

  void notifyAll()
  {
    pthread_cond_broadcast(&cond_);
  }

 private:
  MutexLock& lock_;
  pthread_cond_t cond_;
};

}

#endif  /* CONDITION_H */
