
#include "src/basics/Thread.h"
#include "src/net/EventLoop.h"
#include "src/net/Channel.h"

#include <unistd.h>
#include <strings.h>
#include <sys/timerfd.h>

#include <iostream>

using namespace std;
using namespace netlib;
using namespace netlib::net;

EventLoop loop;
int fd;
struct itimerspec spec;

void timerCallBack()
{
  cout << "in timerCallBack" << endl;
  loop.quit();
}

void CallBack()
{
  cout << "in CallBack" << endl;
}

void func()
{
  sleep(3);
  cout << "in func" << endl;
  loop.runInLoop(CallBack);
}

int main()
{
  bzero(&spec, sizeof spec);
  spec.it_value.tv_sec = 4;
  fd = timerfd_create(CLOCK_MONOTONIC, 0);
  Channel ch(&loop, fd);
  timerfd_settime(fd, 0, &spec, NULL);
  ch.setReadCallBack(timerCallBack);
  ch.setEnableReading();
  Thread thread(func);
  thread.start();
  loop.loop();
}
