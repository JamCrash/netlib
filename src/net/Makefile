
INCLUDE_PATH := /home/han/Desktop/VScode/netlib

CXXFLAGS := -g	\
			-Wall	\
			-I$(INCLUDE_PATH)

objects := Buffer.o	Channel.o EventLoop.o Poller.o Timer.o TimerQueue.o

.PHONY: all
all: $(objects)

Buffer.o: 
	g++ $(CXXFLAGS)	-c Buffer.cc -o $@ -lphread
Channel.o:
	g++ $(CXXFLAGS) -c Channel.cc -o $@ -lphread
EventLoop.o:
	g++ $(CXXFLAGS) -c EventLoop.cc -o $@ -lpthread
Poller.o:
	g++ $(CXXFLAGS) -c Poller.cc -o $@ -lpthread
Timer.o:
	g++ $(CXXFLAGS) -c Timer.cc -o $@ -lpthread
TimerQueue.o:
	g++ $(CXXFLAGS) -c TimerQueue.cc -o $@ -lpthread

.PHONY: clean 
clean: 
	rm -f $(objects)
