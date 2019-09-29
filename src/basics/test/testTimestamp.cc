
#include "src/basics/Timestamp.h"

#include <assert.h>
#include <stdio.h>

using namespace netlib;

int main()
{
  Timestamp stamp;
  assert(!stamp.valid());
  Timestamp stamp2 = Timestamp::invalid();
  assert(!stamp2.valid());
  stamp = Timestamp::now();
  stamp2 = Timestamp(stamp.microSecondsFromEpoch());
  assert(stamp == stamp2);
  Timestamp stamp3(Timestamp().microSecondsFromEpoch() + stamp2.microSecondsFromEpoch());
  assert(stamp == stamp3);
  printf("%lu\n", stamp2.microSecondsFromEpoch()/Timestamp::microSecondsPerSecond);
}