// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Packet management

#include "httpd.h"

void httpd_read_request(int client_sock, struct request_t * request)
{
	char line_buffer[REQUEST_BUFFER_LENGTH];
	httpd_readline(client_sock, line_buffer, REQUEST_BUFFER_LENGTH);
	// e.g. GET /index.html HTTP1.1
	char * token_ptr = strtok(line_buffer, " ");
	if (!token_ptr)
		ALERT_PROGRAM();

	if (strcasecmp(token_ptr, "GET") == 0)
		request->type = STATIC_CONTENT;
	else if (strcasecmp(token_ptr, "POST") == 0)
		request->type = DYNAMIC_CONTENT;
	else
		request->type = UNKNOWN_CONTENT;

	token_ptr = strtok(NULL, " ");
	if (!token_ptr)
		ALERT_PROGRAM();

	strcpy(request->uri, token_ptr);

    // read the rest of headers, which will be ignored.
    while (strcasecmp(line_buffer, "\r\n") != 0)
        httpd_readline(client_sock, line_buffer, REQUEST_BUFFER_LENGTH);
}
