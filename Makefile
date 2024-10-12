CC := cc
CFLAGS := -Wall -Wextra
CINCLUDES := -I./includes

SRCS := $(wildcard src/*.c)
HEADERS := $(wildcard includes/*.h)

server: $(SRCS) $(HEADERS)
	$(CC) $(SRCS) $(CINCLUDES) $(CFLAGS) -o server