// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: Implementation of workflow componment.

#include "httpd.h"

int httpd_init_service(ushort port)
{
    // blocking SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    int server_sock; 
    struct sockaddr_in server_addr;

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        TERMINATE_PROGRAM();

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        TERMINATE_PROGRAM();

    if (listen(server_sock, BACKLOGS) < 0)
        TERMINATE_PROGRAM();

    // It will loop forever unless user aborted it. Note that server_sock will close
    // by OS.
    for ( ; ; )
    	httpd_accept_connection(server_sock);
}

void httpd_accept_connection(int server_sock)
{
    int client_sock = accept(server_sock, NULL, NULL);
    if (client_sock < 0)
        TERMINATE_PROGRAM();

#ifdef CONFIG_THREADING
    pthread_t pid;
    int * args = malloc(sizeof(int));
    * args = client_sock;
    if ((errno = pthread_create(&pid, NULL, httpd_process_request_threading, args)) != 0)
        TERMINATE_PROGRAM();
#else
    httpd_process_request(client_sock);
#endif // CONFIG_THREADING
    
}

#ifdef CONFIG_THREADING
void * httpd_process_request_threading(void * args)
{
    int client_sock = * (int *) args;
    free(args);

    // to release its resourse
    if ((errno = pthread_detach(pthread_self())) != 0)
        TERMINATE_PROGRAM();

    httpd_process_request(client_sock);
    return NULL;
}
#endif // CONFIG_THREADING

void httpd_process_request(int client_sock)
{
	struct request_t request;
    memset(&request, 0, sizeof(request));
    httpd_read_request(client_sock, &request);

    switch (request.type)
    {
    case STATIC_CONTENT:
        httpd_transfer_static_content(client_sock, &request);
        break;
    case DYNAMIC_CONTENT:
        httpd_transfer_dynamic_content(client_sock, &request);
        break;
    case UNKNOWN_CONTENT:
        httpd_transfer_error_content(client_sock, NOT_IMPLEMENTED);
        break;
    }
    close(client_sock);
    return;
}
