CC = g++

RM = rm -rf

SRCS_OFFLINE = $(wildcard src/offline/*.cc)
SRCS_ONLINE = $(wildcard src/online/*.cc)
SRCS_MYTASK = $(wildcard src/online/myTask/*.cc)

BIN_OFFLINE = bin/dictionary
BIN_ONLINE = bin/server
BINS = $(BIN_OFFLINE) $(BIN_ONLINE)

FLAGS =-lredis++ -lhiredis -lpthread -ljson -g -Wall

all: $(BINS)

$(BIN_OFFLINE): $(SRCS_OFFLINE)
	$(CC) $^ -o $@ $(FLAGS)

$(BIN_ONLINE): $(SRCS_ONLINE) $(SRCS_MYTASK)
	$(CC) -std=c++11 $^ -o $@ $(FLAGS)

.PHONY: clean
clean:
	$(RM) $(BIN_OFFLINE)
	$(RM) $(BIN_ONLINE)
