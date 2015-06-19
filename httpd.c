// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.

#include "httpd.h"

int main(int argc, char *argv[])
{
    httpd_init_service(DEFAULT_PORT);
    exit(0);
}
