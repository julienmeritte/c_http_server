/*
** ETNA PROJECT, 02/01/2023 by meritt_j, vignan_q
** logging.h
** File description:
**      headers for logging file
*/

// Include networking libraries if Windows or Unix
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32)
/* Windows headers */
#include <windows.h>
#include <io.h>
#include <processthreadsapi.h>
#include <sysinfoapi.h>
#include <winsock2.h>

#else

#include <netinet/in.h>

#endif /* _WIN32 */

#ifndef H_SERVER
#define H_SERVER

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>

#include <sys/epoll.h>

#include <netinet/in.h>

#include <locale.h>

#include "internal.h"
#include "logging.h"
#include "main.h"

#define PORT 8080
#define MAX_EVENTS 1
#define CONNECTION 1000

#define SERVER "cherokee/1.0"
#define PROTOCOL "HTTP/1.1"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define PORT 8080


int runServer();

int initServer();

int manageRequest();

int acceptRequest();

int readRequest();

int sendResponse();

int findCharacterIndex(char *string, char ch);

char *parseHeaders(char *header, char *body);

char *parseRequestLine(char *request, char *requestLine);

enum METHOD_TYPE getMethodType(char *requestLine);

enum PATH_TYPE getPathType(char *path);

int isValidProtocol();

int processResponse(char *request, char *originResponse);

int deleteFile(FILE *responseStream, char *response, char *path, int methodType);

int addFile(FILE *responseStream, char *response, char *path, int methodType, char *bodyRequest, int httpStatus);

char *trim(char *s);

char *trimStart(char *s);

int getDirectory(FILE *responseStream, char *response, char *path, struct stat statbuf, int methodType);

int getFile(FILE *responseStream, char *response, char *path, int methodType);

char *
generateResponse(char *response, int httpStatus, const char *bodyResponse, FILE *responseStream, char *contentType,
                 int methodType, char *extraHeader);

char *generateResponseError(char *response, int httpStatus, const char *bodyResponse, FILE *responseStream);

char *generateResponseStatusLine(char *response, int httpStatusEnum, FILE *responseStream);

char *generateResponseHeaders(char *response, char *contentType, size_t bodyLength, FILE *responseStream, char *extraHeader);

char *getMimeType(char *name);

void generateInfo(FILE *responseStream, char* response, int methodType);

void generateSwagger(FILE *responseStream, char* response, int methodType);

const char *convertBooleanToString(int number);


enum METHOD_TYPE {
    METHOD_TYPE_UNKNOWN, METHOD_TYPE_GET, METHOD_TYPE_HEAD, METHOD_TYPE_POST, METHOD_TYPE_PUT, METHOD_TYPE_DELETE
};

enum PATH_TYPE {
    PATH_TYPE_UNKNOWN, PATH_TYPE_PATH, PATH_TYPE_INFO, PATH_TYPE_ENDPOINTS
};

enum HTTP_STATUS {
    HTTP_STATUS_OK,
    HTTP_STATUS_FORBIDDEN,
    HTTP_STATUS_BAD_REQUEST,
    HTTP_STATUS_NOT_FOUND,
    HTTP_STATUS_NOT_SUPPORTED,
    HTTP_STATUS_CREATED,
    HTTP_STATUS_INTERNAL_SERVER_ERROR,
    HTTP_STATUS_CONFLICT,
    HTTP_STATUS_NO_CONTENT,
    HTTP_STATUS_NOT_IMPLEMENTED
};

#endif
