CC = gcc
CFLAGS = -O2 -Wall -I .. -rdynamic -DCONFIG_THREADING

all: httpd

httpd: httpd.c
	$(CC) $(CFLAGS) -o httpd *.c *.h -lpthread

clean:
	rm -f httpd
