CC := cc
CFLAGS := -Wall -Wextra

server: server.c
	$(CC) server.c $(CFLAGS) -o server