// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Packet management

#include "httpd.h"

void httpd_read_request(int client_sock, struct request_t * request)
{
	char line_buffer[REQUEST_BUFFER_LENGTH];
	httpd_readline(client_sock, line_buffer, REQUEST_BUFFER_LENGTH);

	char * token_ptr = strtok(line_buffer, " ");
	if (!token_ptr)
		ALERT_PROGRAM();

	if (strcasecmp(token_ptr, "GET") == 0)
	{
		token_ptr = strtok(NULL, " ");

		if (!strstr(token_ptr, "cgi-bin"))
		{
			strcpy(request->filename, WWWROOT);
			strcat(request->filename, token_ptr);
			if (token_ptr[strlen(token_ptr) - 1] == '/')
				strcat(request->filename, "index.html");
			request->type = STATIC_CONTENT;
		}
		else
		{
			// dynamic content
			strcpy(request->filename, WWWROOT);

			char * token_ptr_backup = token_ptr;
			while (* token_ptr != '?' && * token_ptr != 0)
				token_ptr++;

			if (* token_ptr != 0)
			{
				strcpy(request->cgiargs, token_ptr + 1);
				* token_ptr = 0;
				strcat(request->filename, token_ptr_backup);
			}
			else
				strcat(request->filename, token_ptr_backup);

			request->type = DYNAMIC_CONTENT;
		}
	}
	else
		request->type = UNKNOWN_CONTENT;

    // read the rest of headers, which will be ignored.
    while (strcasecmp(line_buffer, "\r\n") != 0)
        httpd_readline(client_sock, line_buffer, REQUEST_BUFFER_LENGTH);
}
