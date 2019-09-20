#ifndef THREAD_H
#define THREAD_H

#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

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
  static void startFunc();

  bool started_;
  bool joined_;
  pthread_t threadId_;
  pid_t tid_;
  ThreadFunc func_;
};

namespace CurrentThread
{
  __thread pid_t CurrentThreadID;

  inline pid_t tid()
  {
    if(CurrentThreadID == 0) 
    {
      CurrentThreadID = ::syscall(SYS_gettid);
    }
    return CurrentThreadID;
  }
} // CurrentThread

} // netlib

#endif  /* THREAD_H */
