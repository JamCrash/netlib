#ifndef TIMER_H
#define TIMER_H

#include "src/basics/noncopyable.h"
#include "src/basics/Timestamp.h"
#include "src/basics/Atomic.h"

#include <functional>

namespace netlib
{
namespace net
{

class Timer: netlib::noncopyable {
 public:
  typedef std::function<void()> CallBack;

  Timer(CallBack cb, Timestamp expiration, double interval);

  void run()
  {
    cb_();
  }

  void restart(Timestamp now);

  Timestamp expiration() const
  { return expiration_; }
  bool repeat() const 
  { return repeat_; }
  int64_t sequence() const 
  { return sequence_; }

 private:
  const CallBack cb_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
  int64_t sequence_;

  static AtomicInteger64 sequenceNumGenerator;
};

} // net 
} // netlib

#endif  /* TIMER_H */
