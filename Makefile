# CXX=g++-8
# CC=gcc-8

ARCH := haswell

ENABLE_TIMER := 0
COMMONFLAGS := -O3 -g -march=$(ARCH) -DENABLE_TIMER=$(ENABLE_TIMER) -Wall -Wextra -Wno-unused-parameter
CXXFLAGS    := $(COMMONFLAGS) $(EFLAGS) -std=c++11
CFLAGS      := $(COMMONFLAGS) $(EFLAGS) -std=c99 -Wno-missing-field-initializers

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
