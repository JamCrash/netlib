
INCLUDE_PATH := /home/han/Desktop/VScode/netlib

CXXFLAGS := -g	\
						-Wall 	\
						-I$(INCLUDE_PATH)

objects := ../Channel.cc ../EventLoop.cc ../Poller.cc	\
					 ../Timer.cc ../TimerQueue.cc	\
					 ../InetAddress.cc	../Acceptor.cc	\
					 ../Socket.cc	../SocketOps.cc	\
					 ../../basics/Timestamp.cc ../../basics/Thread.cc

test:	test.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testTimer: testTimer.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testEventLoop: testEventLoop.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testPoller: testPoller.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testAcceptor: testAcceptor.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

TESTS := test testTimer testEventLoop testPoller testAcceptor

.PHONY: clean
clean:
	rm -f $(TESTS)
