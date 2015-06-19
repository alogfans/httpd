// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Packet management

#include "httpd.h"

void httpd_read_request(int client_sock, struct request_t * request)
{
	char line_buffer[REQUEST_BUFFER_LENGTH];
	char type[PROPERTY_LENGTH], uri[PROPERTY_LENGTH];
	httpd_readline(client_sock, line_buffer, REQUEST_BUFFER_LENGTH);

	int i, j;
	for (i = 0, j = 0; i < strlen(line_buffer) && j < PROPERTY_LENGTH; i++, j++)
	{
		type[j] = line_buffer[i];
		if (type[j] == ' ')
		{
			type[j] = 0;
			break;
		}
	}

	for (i++, j = 0; i < strlen(line_buffer) && j < PROPERTY_LENGTH; i++, j++)
	{
		uri[j] = line_buffer[i];
		if (uri[j] == ' ')
		{
			uri[j] = 0;
			break;
		}		
	}

	if (strcasecmp(type, "GET") == 0)
	{
		if (!strstr(uri, "cgi-bin"))
		{
			strcpy(request->filename, WWWROOT);
			strcat(request->filename, uri);
			if (uri[strlen(uri) - 1] == '/')
				strcat(request->filename, "index.html");
			request->type = STATIC_CONTENT;
		}
		else
		{
			// dynamic content
			strcpy(request->filename, WWWROOT);

			int k;
			for (k = 0; k < strlen(uri); k++)
				if (k == '?')
					break;

			if (k != strlen(uri))
			{
				strcpy(request->cgiargs, uri + k + 1);
				* (uri + k) = 0;
				strcat(request->filename, uri);
			}
			else
				strcat(request->filename, uri);

			request->type = DYNAMIC_CONTENT;
		}
	}
	else
		request->type = UNKNOWN_CONTENT;

    // read the rest of headers, which will be ignored.
    while (strcasecmp(line_buffer, "\r\n") != 0)
        httpd_readline(client_sock, line_buffer, REQUEST_BUFFER_LENGTH);
}
