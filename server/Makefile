CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wstrict-aliasing -I include
SRCSERVER = $(wildcard src/*.c)
OBJSERVER = $(SRCSERVER:.c=.o)
EXEC = server

all: $(EXEC)

server : $(OBJSERVER)
	$(CC) -o $@ $(OBJSERVER)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean : 
	rm -f src/*.o $(EXEC)