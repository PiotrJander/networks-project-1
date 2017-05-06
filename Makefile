TARGET: echo-server echo-client

CC	= gcc
CFLAGS	= -Wall -O2
LFLAGS	= -Wall

C_FILES=$(wildcard src/*.c)
OBJ_FILES=$(addprefix bin/,$(notdir $(C_FILES:.c=.o)))

bin/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

echo-server: bin/echo-server.o bin/err.o bin/socket_wrappers.o
	$(CC) $(LFLAGS) $^ -o bin/$@

echo-client: bin/echo-client.o bin/err.o bin/socket_wrappers.o
	$(CC) $(LFLAGS) $^ -o bin/$@

server: echo-server
	bin/echo-server

client: echo-client
	bin/echo-client localhost 10001 hello world

.PHONY: clean TARGET
clean:
	rm -f bin/*
