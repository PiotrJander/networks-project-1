TARGET: bin/echo-server bin/echo-client

CC	= gcc
CFLAGS	= -Wall -O2
LFLAGS	= -Wall

C_FILES=$(wildcard src/*.c)
OBJ_FILES=$(addprefix bin/,$(notdir $(C_FILES:.c=.o)))

bin/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

bin/echo-server: bin/echo-server.o bin/err.o bin/socket_wrappers.o
	$(CC) $(LFLAGS) $^ -o $@

bin/echo-client: bin/echo-client.o bin/err.o bin/socket_wrappers.o
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: server
server: bin/echo-server
	bin/echo-server 10001 foo.txt

.PHONY: client
client: bin/echo-client
	bin/echo-client 1234 a localhost 10001

# .PHONY: test
# test: echo-server echo-client
#    bin/echo-server
#    bin/echo-client localhost 10001 hello world

.PHONY: clean TARGET
clean:
	rm -f bin/*
