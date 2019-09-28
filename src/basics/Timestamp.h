#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "copyable.h"

#include <stdint.h>

namespace netlib
{

class Timestamp: netlib::copyable {
 public:
  Timestamp();
  explicit Timestamp(int64_t microSecondsFromEpoch);

  bool valid() const { return microSecondsFromEpoch_ > 0; }

  int64_t microSecondsFromEpoch() const { return microSecondsFromEpoch_; }

  static Timestamp now();

  static Timestamp invalid();

  static const int64_t microSecondsPerSecond = 1000 * 1000;
 private:
  int64_t microSecondsFromEpoch_;
};

inline Timestamp addTime(Timestamp timestamp, double offsetTime)
{
  //MIX ME
  //check timestamp valid
  int64_t microSeconds = static_cast<int64_t>(offsetTime * Timestamp::microSecondsPerSecond);
  return Timestamp(timestamp.microSecondsFromEpoch() + microSeconds);
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsFromEpoch() == rhs.microSecondsFromEpoch();
}

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsFromEpoch() < rhs.microSecondsFromEpoch();
}

} // netlib

#endif  /* TIMESTAMP_H */
