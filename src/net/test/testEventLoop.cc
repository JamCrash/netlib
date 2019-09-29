
#include "src/basics/Mutex.h"
#include "src/net/Channel.h"
#include "src/net/EventLoop.h"

#include <iostream>

#include <assert.h>

using namespace std;
using namespace netlib;
using namespace netlib::net;

EventLoop loop;
int a = 1;

void callback()
{
  sleep(1);
  a++;
  cout << "in callback" << endl;
  if(a == 4) {
    a = 1;
    loop.quit();
  }
}

void wakeup()
{
  cout << "in wakeup" << endl;
}

void wakeupLoop()
{
  sleep(2);
  loop.runInLoop(wakeup);
}

void timerCallBack1()
{
  cout << "timeout CallBack1" << endl;
}

void timerCallBack2()
{
  cout << "timeout CallBack2" << endl;
  a++;
  if(a == 5)
  {
    a = 1;
    loop.quit();
  }
}

int main()
{
/*   Channel channel(&loop, 0);
  channel.setWriteCallBack(callback);
  channel.setEnableWriting();

  Thread thread(wakeupLoop);
  thread.start();
  loop.loop();
  thread.join();
  if(!thread.joined())
    assert(0);
 */
  loop.runAfter(3.0, timerCallBack1);
  loop.runEvery(1.0, timerCallBack2);
  loop.loop();
}
