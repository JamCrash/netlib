
INCLUDE_PATH := /home/han/Desktop/VScode/netlib

CXXFLAG := -g	\
					 -Wall	\
					 -I$(INCLUDE_PATH)

testTimestamp: testTimestamp.cc
	g++ $(CXXFLAG) $< ../Timestamp.cc -o $@

.PHONY:	clean
clean:
	rm -f testTimestamp