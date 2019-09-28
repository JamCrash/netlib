
#include "src/basics/Timestamp.h"
#include "src/net/EventLoop.h"

#include <iostream>

using namespace std;
using namespace netlib;
using namespace netlib::net;

EventLoop loop;
int a = 0;
void f()
{
  a++;
  cout << "timeout" << endl;
  if(a == 2)
    loop.quit();
}

int main()
{
  loop.runEvery(3.0, f);
  loop.loop();
  cout << "end" << endl;
}