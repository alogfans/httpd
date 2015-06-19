// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Static content management

#include "httpd.h"

void httpd_transfer_static_content(int client_sock, struct request_t * request)
{
    printf("STA: filename = %s\n", request->filename);

    char buffer[REPLY_BUFFER_LENGTH];
    struct stat sbuf;

    if (stat(request->filename, &sbuf) < 0)
    {
        httpd_transfer_error_content(client_sock, NOT_FOUND);
        return;
    }

    int length = sbuf.st_size;
    int local_fd = open(request->filename, O_RDONLY);
    if (local_fd < 0)
        TERMINATE_PROGRAM();

    sprintf(buffer, "HTTP/1.0 %d %s\r\n", 200, "OK");
    sprintf(buffer, "%s%s\r\n", buffer, SERVER_AGENT);
    sprintf(buffer, "%sContent-length: %d\r\n", buffer, length);
    sprintf(buffer, "%sContent-type: %s\r\n", buffer, httpd_filetype(request->filename));
    sprintf(buffer, "%s\r\n", buffer);
    httpd_write(client_sock, buffer, strlen(buffer));

    int remain = length;

    while (remain > REPLY_BUFFER_LENGTH)
    {
        httpd_read(local_fd, buffer, REPLY_BUFFER_LENGTH);
        httpd_write(client_sock, buffer, REPLY_BUFFER_LENGTH);
        remain -= REPLY_BUFFER_LENGTH;
    }

    httpd_read(local_fd, buffer, remain);
    httpd_write(client_sock, buffer, remain);
    close(local_fd);
}
