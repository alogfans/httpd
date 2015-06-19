// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Packet management

#include "httpd.h"

ssize_t httpd_write(int fd, char * buffer, size_t size)
{
	while (1)
	{
		int nbytes = write(fd, buffer, size);
		if (nbytes == -1 && errno == EINTR)
			continue;
		return nbytes;
	}
}

ssize_t httpd_read(int fd, char * buffer, size_t size)
{
	while (1)
	{
		int nbytes = read(fd, buffer, size);
		if (nbytes == -1 && errno == EINTR)
			continue;
		return nbytes;
	}
}

ssize_t httpd_recv_peek(int fd, char * buffer, size_t size)
{
	while (1)
	{
		int nbytes = recv(fd, buffer, size, MSG_PEEK);
		if (nbytes == -1 && errno == EINTR)
			continue;
		return nbytes;
	}
}

ssize_t httpd_readline(int fd, char * buffer, size_t size)
{
    ssize_t n, rc;
    char c, * ptr = buffer;

    for (n = 1; n < size; n++)
    {
    again:
    	if ( (rc = read(fd, &c, 1)) == 1)
    	{
    		*ptr++ = c;
    		if (c == '\n')		// new line is stored
    			break;
    	} 
    	else if (rc == 0) 		// eof
    	{
    		*ptr = 0;
    		return n - 1;
    	}
    	else
    	{
    		if (errno == EINTR)
    			goto again;
    		TERMINATE_PROGRAM();
    	}
    }

    *ptr = 0;
    return n;
}