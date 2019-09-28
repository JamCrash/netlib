
#include <assert.h>
#include <sys/syscall.h>
#include <unistd.h>


#include <exception>

#include "Thread.h"
#include "Logging.h"

namespace netlib
{

namespace
{
  __thread pid_t CurrentThreadId = 0;

  pid_t gettid()
  {
    return static_cast<pid_t>(syscall(SYS_gettid));
  }
}

pid_t CurrentThread::tid()
{
  if(CurrentThreadId == 0)
  {
    CurrentThreadId = gettid();
  }
  return CurrentThreadId;
}

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
      LOG << "Thread "
          << tid_
          << " crashed\n";
      abort();
    }
    catch(...)
    {
      LOG << "Unknown exception occur\n";
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
    threadId_(0),
    tid_(CurrentThread::tid())
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
    LOG << "Create new thread failed\n";
  }
  else
  {
    LOG << "New thread started, thread id="
             << tid_
             << "\n";
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
