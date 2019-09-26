
#include "src/net/Timer.h"

using namespace netlib;
using namespace netlib::net;

Timer::Timer(CallBack cb, Timestamp expiration, double interval)
: cb_(std::move(cb)),
  expiration_(expiration),
  interval_(interval),
  repeat_(interval_ > 0.0),
  sequence_(sequenceNumGenerator.incrementAndGet())
{
}

void Timer::restart(Timestamp now)
{
  if(repeat_)
  {
    expiration_ = addTime(now, interval_);
  }
  else
  {
    expiration_ = Timestamp::invalid();
  }
  
}
