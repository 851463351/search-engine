CC = g++

RM = rm -rf

SRCS_MYTASK = $(wildcard src/*.cc)
SRCS_THREADPOOL=$(wildcard src/bo_threadpool/*.cc)
SRCS_REACTOR=$(wildcard src/Reactor/*.cc)

BIN_ONLINE = bin/test
BINS = $(BIN_ONLINE)

FLAGS = -lpthread -ljson -g -Wall

all: $(BINS)

$(BIN_ONLINE): $(SRCS_MYTASK) $(SRCS_THREADPOOL) $(SRCS_REACTOR) 
	$(CC)  $^ -o $@ $(FLAGS)

.PHONY: clean
clean:
	$(RM) $(BIN_OFFLINE)
