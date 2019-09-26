
CXXFLAGS := -g	\
						-Wall \
						-O0

LKFLAGS	:= -lpthread

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
