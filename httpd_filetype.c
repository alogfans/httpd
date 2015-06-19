// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Packet management

#include "httpd.h"

const struct mime_t mimelist[] = 
{
	{ ".html", "text/html"  },
    { ".gif",  "image/gif"  },
    { ".png",  "image/png"  },
    { ".jpg",  "image/jpeg" },
    { ".txt",  "text/plain" }
}; 

#define ELEMENTS_OF_MIMES (sizeof(mimelist) / sizeof(struct mime_t))

const char * httpd_filetype(const char * filename)
{
	int i;
	for (i = 0; i < ELEMENTS_OF_MIMES; i++)
		if (strstr(filename, mimelist[i].ext))
			return mimelist[i].mime;
		
	return mimelist[i - 1].mime; 	// the last one: text/plain
}