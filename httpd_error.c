// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: When error happens, we should display error message
// e.g. 404

#include "httpd.h"

const char error_reply_template[] = 
	"HTTP/1.0 %d %s\r\n"		// the 3rd and 4th arg in sprintf
	SERVER_AGENT " \r\n"
	"Content-Type: text/html\r\n"
	"\r\n"
	"<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY><P>%s</BODY></HTML>\r\n";
								// the 5rd and 6th arg in sprintf

void httpd_transfer_error_content(int client_sock, int error_type)
{
	char short_text[PROPERTY_LENGTH];
	char detailed_text[PROPERTY_LENGTH];	
	char transfer_buffer[REPLY_BUFFER_LENGTH];

	switch (error_type)
	{
	case NOT_IMPLEMENTED:
		strcpy(short_text, "Not Implemented");
		strcpy(detailed_text, "HTTP request method not supported.");
		break;

	case NOT_FOUND:
		strcpy(short_text, "Not Found");
		strcpy(detailed_text, "The resource specified is unavailable or nonexistent.");
		break;
	}

	sprintf(transfer_buffer, error_reply_template, 
		error_type, short_text, short_text, detailed_text);

	httpd_write(client_sock, transfer_buffer, strlen(transfer_buffer));
}
