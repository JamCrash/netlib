
INCLUDE_PATH := /home/han/Desktop/VScode/netlib

CXXFLAGS := -g	\
						-Wall 	\
						-I$(INCLUDE_PATH)

objects := ../Channel.cc ../EventLoop.cc ../Poller.cc	\
					 ../Timer.cc ../TimerQueue.cc	\
					 ../../basics/Timestamp.cc ../../basics/Thread.cc

test:	test.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testTimer: testTimer.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testEventLoop: testEventLoop.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

testPoller: testPoller.cc
	g++ $(CXXFLAGS) $< $(objects) -o $@ -lpthread

.PHONY: clean
clean:
	rm -f test*
