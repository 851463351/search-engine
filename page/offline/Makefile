CC = g++

RM = rm -rf

SRCS_OFFLINE = $(wildcard src/*.cc)

BIN_ONLINE = bin/test
BINS = $(BIN_ONLINE)

FLAGS = 

all: $(BINS)

$(BIN_ONLINE): $(SRCS_OFFLINE)
	$(CC)  $^ -o $@ $(FLAGS)

.PHONY: clean
clean:
	$(RM) $(BIN_OFFLINE)
