CC = gcc
CFLAGS = -std=c11 -pthread -g -I include
SRCSERVER = $(wildcard src/*.c)
OBJSERVER = $(SRCSERVER:.c=.o)
EXEC = server

all: $(EXEC)

server : $(OBJSERVER)
	$(CC) -o $@ $(OBJSERVER) $(CFLAGS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean : 
	rm -f src/*.o $(EXEC)