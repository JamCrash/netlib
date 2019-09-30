
#include "src/net/InetAddress.h"
#include "src/net/Acceptor.h"
#include "src/net/EventLoop.h"

#include <iostream>
#include <assert.h>

using namespace netlib;
using namespace net;
using namespace std;

EventLoop loop;

void callBack(int connFd, const InetAddress& peerAddr)
{
  cout << "connFd=" << connFd << ":" << peerAddr.toHostPort() << endl;
  loop.quit();
}

int main()
{
  InetAddress listenAddr(3000);
  Acceptor accpetor(&loop,listenAddr);
  accpetor.setNewConnectionCallBack(callBack);
  assert(!accpetor.listening());
  accpetor.listen();
  loop.loop();
}
