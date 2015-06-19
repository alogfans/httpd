// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Handling errors in httpd

#include "httpd.h"

// attain bacetrace information from system and print it to screen.
void httpd_dump_backtrace()
{
	void * buffer[BACKTRACE_BUFFER_LENGTH];
	int nptrs = backtrace(buffer, BACKTRACE_BUFFER_LENGTH);
	char ** strings = backtrace_symbols(buffer, nptrs);
	int i;
	if (strings)
		for (i = 0; i < nptrs; ++i)
			printf("  [%d] %s\n", i, strings[i]);
	free(strings);
}

