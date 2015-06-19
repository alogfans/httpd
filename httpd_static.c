// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Static content management

#include "httpd.h"


void httpd_transfer_static_content(int client_sock, struct request_t * request)
{
    int length;
    char filename[REPLY_BUFFER_LENGTH];
    sprintf(filename, WWWROOT "%s", request->uri);
    if (strcmp(request->uri, "/") == 0)
    	strcat(filename, "index.html");
    
    char buffer[REPLY_BUFFER_LENGTH];
    struct stat sbuf;

    if (stat(filename, &sbuf) < 0)
    {
        httpd_transfer_error_content(client_sock, NOT_FOUND);
        return;
    }

    length = sbuf.st_size;

    FILE * fd = fopen(filename, "r");
    if (fd == NULL)
    {
        httpd_transfer_error_content(client_sock, NOT_FOUND);
        return;
    }

    sprintf(buffer, "HTTP/1.0 %d %s\r\n", 200, "OK");
    sprintf(buffer, "%s%s\r\n", buffer, SERVER_AGENT);
    sprintf(buffer, "%sContent-length: %d\r\n", buffer, length);
    sprintf(buffer, "%sContent-type: %s\r\n", buffer, httpd_filetype(filename));
    sprintf(buffer, "%s\r\n", buffer);
    write(client_sock, buffer, strlen(buffer));

    int remain = length;

    while (remain > REPLY_BUFFER_LENGTH)
    {
        fread(buffer, REPLY_BUFFER_LENGTH, 1, fd);
        write(client_sock, buffer, REPLY_BUFFER_LENGTH);
        remain -= REPLY_BUFFER_LENGTH;
    }

    fread(buffer, remain, 1, fd);
    write(client_sock, buffer, remain);
}
