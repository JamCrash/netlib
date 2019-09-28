#ifndef TIMERID_H
#define TIMERID_H

#include "src/basics/copyable.h"
#include "src/basics/Timestamp.h"
#include "src/net/Timer.h"

namespace netlib
{
namespace net
{

class TimerId: netlib::copyable {
  friend class TimerQueue;
 public:
  TimerId(Timer* timer)
  : timer_(timer),
    sequence_(timer->sequence())
  {
  }

 private:
  Timer* timer_;
  int64_t sequence_;
};

} // net
} //netlib

#endif  /* TIMERID_H */
