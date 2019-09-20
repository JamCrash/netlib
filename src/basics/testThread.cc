
#include "Thread.h"

#include <stdio.h>

using namespace netlib;

void fn()
{
  printf("in fn\n");
}

int main()
{
  Thread thread(fn);
  thread.start();
  printf("in main\n");
}