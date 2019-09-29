
#include <unistd.h>
#include <stdint.h>
#include <strings.h>
#include <stdio.h>
#include <sys/timerfd.h>

int main()
{
  int fd = timerfd_create(CLOCK_MONOTONIC, 0);
  struct itimerspec spec;
  bzero(&spec, sizeof spec);
  spec.it_value.tv_sec = 1;
  timerfd_settime(fd, 0, &spec, 0);
  uint64_t val;
  ssize_t n = read(fd, &val, sizeof val);
  if(n == sizeof val)
  {
    printf("ok\n");
  }
}