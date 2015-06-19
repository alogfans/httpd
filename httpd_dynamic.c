// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Static content management

#include "httpd.h"


void httpd_transfer_dynamic_content(int client_sock, struct request_t * request)
{
    httpd_transfer_error_content(client_sock, NOT_IMPLEMENTED);
}
