CC = gcc
CFLAGS = -O2 -Wall -I ..

all: httpd

httpd: httpd.c
	$(CC) $(CFLAGS) -o httpd *.c *.h

clean:
	rm -f httpd
