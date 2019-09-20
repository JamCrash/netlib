
MUDUO_PATH := /home/han/Desktop/VScode/build/release-install-cpp11
INCLUDE_PATH := $(MUDUO_PATH)/include
LIBRARY_PATH := $(MUDUO_PATH)/lib

CXXFLAGS := -g	\
						-Wall \
						-O0	\
						-I$(INCLUDE_PATH)
LKFLAGS	:= -L$(LIBRARY_PATH) -lpthread -lmuduo_base

objects := Thread.o

all: $(objects)

Thread.o:	noncopyable.h Thread.h
	g++ $(CXXFLAGS) -c Thread.cc $(LKFLAGS)

.PHONY:	clean
clean:
	rm -f $(objects)


testObj := testThread

.PHONY: allTest
allTest:	$(testObj)

testThread: testThread.cc Thread.o
	g++ $(CXXFLAGS) -o $@ $^ $(LKFLAGS)

.PHONY:	cleanAllTests
cleanAllTests:
	rm -f $(testObj)
