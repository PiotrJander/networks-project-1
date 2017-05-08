TARGET: bin/server bin/client

CC	= gcc
CFLAGS	= -Wall -O2
LFLAGS	= -Wall

C_FILES=$(wildcard src/*.c)
OBJ_FILES=$(addprefix bin/,$(notdir $(C_FILES:.c=.o)))

bin:
	mkdir bin

bin/%.o: bin src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

bin/server: bin/server.o
	$(CC) $(LFLAGS) $^ -o $@

bin/client: bin/client.o
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: server
server: bin/server
	bin/server 10001 foo.txt

.PHONY: client
client: bin/client
	bin/client 1234 a localhost 10001

.PHONY: clean TARGET
clean:
	rm -f bin/*
