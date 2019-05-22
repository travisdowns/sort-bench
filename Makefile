# CXX=g++-8
# CC=gcc-8

ENABLE_TIMER := 0
COMMONFLAGS := -O3 -g -march=haswell -DENABLE_TIMER=$(ENABLE_TIMER) -Wall -Wextra -Wno-unused-parameter
CXXFLAGS    := $(COMMONFLAGS) $(EFLAGS) -std=c++11
CFLAGS      := $(COMMONFLAGS) $(EFLAGS)

HEADERS := $(wildcard *.h*)
SRCS    := $(wildcard *.cpp *.c)
OBJS    := $(SRCS:%.cpp=%.o)
OBJS    := $(OBJS:%.c=%.o)

# $(info "OBJS = $(OBJS)")

bench: $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS)    $(CPPFLAGS) -c $<

clean:
	rm -f bench *.o
