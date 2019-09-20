
#include <pthread.h>

#include <exception>
#include <iostream>

#include "Thread.h"

namespace netlib
{

namespace details 
{

struct ThreadData {
  typedef std::function<void()> Func;
  Func func_;
  pid_t tid_;

  ThreadData(Func func, pid_t tid)
  : func_(std::move(func)),
    tid_(tid)
  {

  }

  void startInLoop()
  {
    try
    {
      func_();
    }
    catch(const std::exception& ex)
    {
      LOG_ERROR << "Thread " 
                << tid_
                << " crashed";
      abort();
    }
    catch(...)
    {
      LOG_ERROR << "Unknown exception occur";
      throw;
    }
  }
};

void* startThread(void* arg)
{
  ThreadData* data = static_cast<ThreadData*>(arg);
  data->startInLoop();
  delete data;
  return NULL;
}

} // details

Thread::Thread(const ThreadFunc& func)
  : func_(func),
    started_(false),
    joined_(false),
    tid_(CurrentThread::tid()),
    threadId_(0)
{

}

Thread::~Thread()
{
  if(started_ && !joined_)
  {
    pthread_detach(threadId_);
  }
}

void Thread::start()
{
  assert(!started_);
  started_ = true;

  details::ThreadData* threadData = new details::ThreadData(func_, tid_);
  if(pthread_create(&threadId_, NULL, &details::startThread, threadData))
  {
    started_ = false;
    LOG_SYSERR << "Create new thread failed";
  }
  else
  {
    LOG_INFO << "New thread started, thread id="
             << tid_;
  }
}

int Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(threadId_, NULL);
}

} // netlib
