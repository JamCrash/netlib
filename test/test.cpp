#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

using namespace std;

void* fn(void* arg)
{
  cout << static_cast<pid_t>(syscall(SYS_gettid)) << endl;
}
int main()
{
  cout << static_cast<pid_t>(syscall(SYS_gettid)) << endl;
  cout << getpid() << endl;
  pthread_t tid;
  pthread_create(&tid, NULL, fn, NULL);
  pthread_join(tid, NULL);
}