// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Static content management

#include "httpd.h"

extern char ** environ;

void httpd_transfer_dynamic_content(int client_sock, struct request_t * request)
{
	printf("DYN: filename = %s, cgiargs = [%s]\n", request->filename, request->cgiargs);

	struct stat sbuf;
    if (stat(request->filename, &sbuf) < 0 || S_ISDIR(sbuf.st_mode))
    {
        httpd_transfer_error_content(client_sock, NOT_FOUND);
        return;
    }

    char buffer[REPLY_BUFFER_LENGTH], * emptylist[] = { NULL };
    sprintf(buffer, "HTTP/1.0 %d %s\r\n", 200, "OK");
    sprintf(buffer, "%s%s\r\n", buffer, SERVER_AGENT);
    httpd_write(client_sock, buffer, strlen(buffer));

    if (fork() == 0) // child
    {
        // bug: is not thread-safe!!!
    	setenv("QUERY_STRING", request->cgiargs, 1);
    	dup2(client_sock, STDOUT_FILENO);
    	execve(request->filename, emptylist, environ);
    }
    // parent: waiting
    wait(NULL); 

}
