CC = gcc
CFLAGS  = -g -Wall

stepone: main.c lexer.c better_string.c
	cc *.c $(CFLAGS) -o stepone 

clean:
	-rm stepone

install:
	cp nxs /usr/local/bin/

uninstall:
	-rm /usr/local/bin/stepone
