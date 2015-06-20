// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Packet management

#include "httpd.h"

ssize_t httpd_write(int fd, char * buffer, size_t size)
{
    int nbytes;
    
again:
    nbytes = write(fd, buffer, size);
	if (nbytes == -1 && errno == EINTR)
		goto again;

    if (nbytes < 0)
        TERMINATE_PROGRAM();

	return nbytes;
}

ssize_t httpd_read(int fd, char * buffer, size_t size)
{
    int nbytes;

again:
    nbytes = read(fd, buffer, size);
    if (nbytes == -1 && errno == EINTR)
        goto again;

    if (nbytes < 0)
        TERMINATE_PROGRAM();

    return nbytes;
}

__thread char   line_buf[REQUEST_BUFFER_LENGTH] = { 0 };
__thread int    read_cnt = 0;
__thread char * read_ptr = NULL;

ssize_t httpd_readline_helper(int fd, char * ptr)
{
    if (read_cnt <= 0)
    {
again:
        if ( (read_cnt = read(fd, line_buf, sizeof(line_buf))) < 0)
        {
            if (errno == EINTR)
                goto again;
            return -1;
        }
        else if (read_cnt == 0)
            return 0;
        read_ptr = line_buf;
    }
    read_cnt--;
    *ptr = *read_ptr++;
    return 1;
}

ssize_t httpd_readline(int fd, char * buffer, size_t size)
{
    ssize_t n, rc;
    char c, * ptr = buffer;

    for (n = 1; n < size; n++)
    {
    	if ( (rc = httpd_readline_helper(fd, &c)) == 1)
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
    		TERMINATE_PROGRAM();
    }

    *ptr = 0;
    return n;
}