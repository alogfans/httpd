// Simple HTTP Server
// Copyright 2015 Alogfans, all rights reserved.
//
// Description: General header file

#ifndef __HTTPD_H__
#define __HTTPD_H__

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <execinfo.h>

#ifdef CONFIG_PTHREAD_SUPPORT
#include <pthread.h>
#endif // CONFIG_PTHREAD_SUPPORT

#define DEFAULT_PORT    		8080
#define REQUEST_BUFFER_LENGTH   2048
#define REPLY_BUFFER_LENGTH     2048
#define PROPERTY_LENGTH         256
#define BACKTRACE_BUFFER_LENGTH	256
#define URI_BUFFER_LENGTH       256
#define MIME_LENGTH             16
#define EXT_LENGTH              8
#define BACKLOGS        		5
#define WWWROOT 		  		"wwwroot/"
#define SERVER_AGENT    		"Tiny HTTP Server"

#define UNKNOWN_CONTENT         0
#define STATIC_CONTENT          1
#define DYNAMIC_CONTENT         2

#define NOT_IMPLEMENTED         501
#define NOT_FOUND               404

#define TERMINATE_PROGRAM() do { \
    printf("\nPanic! This program will be terminated due to an unexpected exception.\n"); \
    printf("Stop location: %s() [%s:%d]\n", __func__, __FILE__, __LINE__); \
    perror("System error code"); \
	printf("--- Function backtrace ---\n"); \
	httpd_dump_backtrace(); \
	exit(-1); \
} while(0)

#define ALERT_PROGRAM() do { \
    printf("\nAlert! This program was encountered with abnormal condition.\n"); \
    printf("Associated location: %s() [%s:%d]\n", __func__, __FILE__, __LINE__); \
    perror("System error code"); \
	printf("--- Function backtrace ---\n"); \
	httpd_dump_backtrace(); \
    return ; \
} while(0)

struct mime_t
{
    char ext[EXT_LENGTH];
    char mime[MIME_LENGTH];
};

struct request_t
{
    int type;
    char uri[URI_BUFFER_LENGTH];
};

// backtrace.c
void httpd_dump_backtrace();

// workflow.c
int httpd_init_service(ushort port);
void httpd_accept_connection(int server_sock);
void httpd_process_request(int client_sock);

// packet.c
void httpd_read_request(int client_sock, struct request_t * request);

// static.c
void httpd_transfer_static_content(int client_sock, struct request_t * request);

// dynamic.c
void httpd_transfer_dynamic_content(int client_sock, struct request_t * request);

// error.c
void httpd_transfer_error_content(int client_sock, int error_type);

// filetype.c
const char * httpd_filetype(const char * filename);

// utility.c
ssize_t httpd_write(int fd, char * buffer, size_t size);
ssize_t httpd_read(int fd, char * buffer, size_t size);
ssize_t httpd_readline(int fd, char * buffer, size_t size);

#endif // __HTTPD_H__