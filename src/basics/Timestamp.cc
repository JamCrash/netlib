
#include "Timestamp.h"

#include <sys/time.h>
#include <stddef.h>

using namespace netlib;

Timestamp::Timestamp()
: microSecondsFromEpoch_(0)
{
}

Timestamp::Timestamp(int64_t microSecondsFromEpoch)
: microSecondsFromEpoch_(microSecondsFromEpoch)
{
}

Timestamp Timestamp::now()
{
  struct timeval tm;
  gettimeofday(&tm, NULL);
  Timestamp timestamp(tm.tv_sec * microSecondsPerSecond + tm.tv_usec);
  return timestamp;
}

Timestamp Timestamp::invalid()
{
  return Timestamp();
}
