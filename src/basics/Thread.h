#ifndef THREAD_H
#define THREAD_H

#include <unistd.h>

#include <functional>

#include "noncopyable.h"

namespace netlib 
{

class Thread: noncopyable {
  typedef std::function<void ()> ThreadFunc;
 public:
  explicit Thread(const ThreadFunc&);

  ~Thread();

  void start();
  int join();
  
  bool started() { return started_; }
  bool joined() { return joined_; }
  pid_t tid() { return tid_; }
  
 private:
  ThreadFunc func_;
  bool started_;
  bool joined_;
  pthread_t threadId_;
  pid_t tid_;
};

namespace CurrentThread
{
  pid_t tid();
} //CurrentThread

} // netlib

#endif  /* THREAD_H */
