CC = g++
CFILES = $(shell find ./ -name "*.cpp")
OBJS = $(CFILES:.cpp=.o)
EXEC = gobang
CXXFLAGS += -std=c++11 -I ./include

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $@
clean:
	-rm -f $(EXEC) *.o