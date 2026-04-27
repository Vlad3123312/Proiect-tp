CC = gcc
CFLAGS = -Wall -Wextra -g
SERVER_OBJ = server.o logic.o astar.o
CLIENT_OBJ = client.o

all: server client

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c game.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o server client