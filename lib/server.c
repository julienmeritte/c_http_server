/*
** ETNA PROJECT, 02/01/2023 by meritt_j, vignan_q
** examplelib.c
** File description:
**      Example file for Makefile
*/

#include "../include/server.h"

int epoll_mask = EPOLLONESHOT | EPOLLET;

struct epoll_event current_ev, ev, events[MAX_EVENTS];
struct sockaddr_in saddr, caddr;

socklen_t saddrlen, caddrlen;

char reply[4096];
char buf[4096];

int acc, soc, epfd, nfd, on;

extern struct GlobalConfigurationProperties globalConfigurationProperties;


int runServer() {

    logsLevel("Server initializing", DEBUG_LOGGING_LEVEL);

    initServer();

    logs("SERVER STARTED...");

    while (1) {

        nfd = epoll_wait(epfd, events, MAX_EVENTS, -1);

        if (nfd == -1) {
            logsLevel("Epoll file descriptor failed.", ERROR_LOGGING_LEVEL);
        } else if (nfd == 0) {
            logsLevel("Epoll file descriptor timed out", ERROR_LOGGING_LEVEL);
        } else {
            if (manageRequest() == -1) {
                logsLevel("Manage request failed.", ERROR_LOGGING_LEVEL);
            }
        }
        fflush(stdout);
        fflush(stderr);
        fflush(stdin);
    }

    close(soc);
    return 0;

}

int initServer() {
    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        logsLevel("Can't make socket", ERROR_LOGGING_LEVEL);
        return 1;
    }

    int on = 1;

    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    saddrlen = sizeof(saddr);
    caddrlen = sizeof(caddr);

    if (bind(soc, (struct sockaddr *) &saddr, saddrlen) == -1) {
        logsLevel("Can't bind socket.", ERROR_LOGGING_LEVEL);
        return 1;
    }

    if (listen(soc, CONNECTION) == -1) {
        logsLevel("Can't listen socket.", ERROR_LOGGING_LEVEL);
        return 1;
    }

    if ((epfd = epoll_create1(0)) == -1) {
        logsLevel("Can't create epoll.", ERROR_LOGGING_LEVEL);
    }

    ev.data.fd = soc;
    ev.events = EPOLLIN;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1) {
        logsLevel("Can't add to epoll.", ERROR_LOGGING_LEVEL);
    }
    return 0;
}

int manageRequest() {
    for (int i = 0; i < nfd; i++) {
        current_ev = events[i];
        if (current_ev.data.fd == soc) {
            logsLevel("Managing entering request...", DEBUG_LOGGING_LEVEL);
            if (acceptRequest() == -1) {
                return -1;
            }
            logsLevel("Request accepted.", DEBUG_LOGGING_LEVEL);
        } else if (current_ev.events & EPOLLIN) {
            logsLevel("Reading request.", DEBUG_LOGGING_LEVEL);
            if (readRequest() == -1) {
                return -1;
            }
            logsLevel("Request read.", DEBUG_LOGGING_LEVEL);
        } else if (current_ev.events & EPOLLOUT) {
            logsLevel("Sending response.", DEBUG_LOGGING_LEVEL);
            if (sendResponse() == -1) {
                return -1;
            }
            logsLevel("Response sent.", DEBUG_LOGGING_LEVEL);
        }
    }
    return 0;
}

int acceptRequest() {
    if ((acc = accept(soc, (struct sockaddr *) &caddr, &caddrlen)) == -1) {
        logsLevel("Failed to accept request.", ERROR_LOGGING_LEVEL);
        return -1;
    }

    current_ev.data.fd = acc;
    current_ev.events = EPOLLIN | epoll_mask;

    epoll_ctl(epfd, EPOLL_CTL_ADD, acc, &current_ev);
    return 0;
}

int readRequest() {
    memset(buf, 0, sizeof buf);
    read(current_ev.data.fd, buf, sizeof(buf));
    if (strcmp("abort", buf) == 0) {
        epoll_ctl(epfd, EPOLL_CTL_DEL, current_ev.data.fd, NULL);
        close(current_ev.data.fd);
    } else {
        memset(reply, 0, sizeof reply);
        processResponse(buf, reply);
        current_ev.events = EPOLLOUT | epoll_mask;
        epoll_ctl(epfd, EPOLL_CTL_MOD, current_ev.data.fd, &current_ev);
    }
    return 0;
}

int sendResponse() {

    write(current_ev.data.fd, reply, strlen(reply));

    current_ev.events = EPOLLIN | epoll_mask;
    epoll_ctl(epfd, EPOLL_CTL_MOD, current_ev.data.fd, &current_ev);

    close(current_ev.data.fd);
    logsLevel("Request closed", DEBUG_LOGGING_LEVEL);
    return 0;
}

int findCharacterIndex(char *string, char ch) {
    int i = 0;
    while (string[i]) {
        if (string[i] == ch) {
            return i;
        }
        i++;
    }
    return -1;
}

char *parseHeaders(char *header, char *body) {
    int indexBodyStart = body - header;
    char *headersOnly = malloc((indexBodyStart) * sizeof(char));
    memcpy(headersOnly, header, indexBodyStart);
    headersOnly[indexBodyStart - 4] = '\0';
    return headersOnly;
}

char *parseRequestLine(char *request, char *requestLine) {
    int endRequestLineIndex = findCharacterIndex(request, '\r');
    if (endRequestLineIndex == -1) {
        return NULL;
    }
    //requestLine = malloc(endRequestLineIndex * sizeof(char));
    memset(requestLine, 0, sizeof &requestLine);
    strncpy(requestLine, request, endRequestLineIndex + 1);
    logs(requestLine);
    return requestLine;
}

enum METHOD_TYPE getMethodType(char *requestLine) {
    char *method = strtok(requestLine, " ");
    if (strcasecmp(method, "GET") == 0) {
        logsLevel("GET METHOD DETECTED", DEBUG_LOGGING_LEVEL);
        return METHOD_TYPE_GET;
    } else if (strcasecmp(method, "POST") == 0) {
        logsLevel("POST METHOD DETECTED", DEBUG_LOGGING_LEVEL);
        return METHOD_TYPE_POST;
    } else if (strcasecmp(method, "HEAD") == 0) {
        logsLevel("HEAD METHOD DETECTED", DEBUG_LOGGING_LEVEL);
        return METHOD_TYPE_HEAD;
    } else if (strcasecmp(method, "PUT") == 0) {
        logsLevel("PUT METHOD DETECTED", DEBUG_LOGGING_LEVEL);
        return METHOD_TYPE_PUT;
    } else if (strcasecmp(method, "DELETE") == 0) {
        logsLevel("DELETE METHOD DETECTED", DEBUG_LOGGING_LEVEL);
        return METHOD_TYPE_DELETE;
    }
    logsLevel("UNKNOWN METHOD DETECTED", DEBUG_LOGGING_LEVEL);
    return METHOD_TYPE_UNKNOWN;
}

enum PATH_TYPE getPathType(char *path) {
    char *localPath = strtok(NULL, " ");
    if (strcasecmp(localPath, "/info") == 0) {
        logsLevel("/info DETECTED", DEBUG_LOGGING_LEVEL);
        return PATH_TYPE_INFO;
    } else if (strcasecmp(localPath, "/swagger") == 0) {
        logsLevel("/config DETECTED", DEBUG_LOGGING_LEVEL);
        return PATH_TYPE_ENDPOINTS;
    } else if (strcasecmp(localPath, ""))
    strcpy(path, globalConfigurationProperties.path);
    strcat(path, localPath);
    return PATH_TYPE_PATH;
}

int isValidProtocol() {
    char *protocol = strtok(NULL, "\r");
    if (strcasecmp(protocol, PROTOCOL) == 0) {
        return 0;
    }
    return -1;
}


int processResponse(char *request, char *originResponse) {

    char *response = originResponse;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response, &bufferSize);

    blocLog(request, DEBUG_LOGGING_LEVEL, "[REQUEST-VALUE-START]", "[REQUEST-VALUE-END]");

    char requestLine[4096];

    parseRequestLine(request, requestLine);

    blocLog(requestLine, DEBUG_LOGGING_LEVEL, "[REQUEST-LINE-START]", "[REQUEST-LINE-END]");

    char *header = trimStart(strstr(request, "\r\n"));

    if (header == NULL) {
        logsLevel("HEADER NULL", ERROR_LOGGING_LEVEL);
        generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Headers invalid", responseStream);
        fclose(responseStream);
        strcat(originResponse, response);
        free(response);
        return -1;
    } else {
        logsLevel("HEADER NOT NULL", DEBUG_LOGGING_LEVEL);
    }

    char *body = trimStart(strstr(header, "\r\n\r\n"));

    if (body == NULL) {
        logsLevel("Body NULL", ERROR_LOGGING_LEVEL);
        generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Body invalid", responseStream);
        fclose(responseStream);
        strcat(originResponse, response);
        free(response);
        return -1;
    } else {
        logsLevel("BODY NOT NULL", DEBUG_LOGGING_LEVEL);
    }

    char *headers = parseHeaders(header, body);

    blocLog(headers, DEBUG_LOGGING_LEVEL, "[HEADERS-START]", "[HEADERS-END]");


    if (strlen(trim(body)) == 0) {
        blocLog("Body vide\n", DEBUG_LOGGING_LEVEL, "[BODY-START]", "[BODY-END]");
    } else {
        blocLog(body, DEBUG_LOGGING_LEVEL, "[BODY-START]", "[BODY-END]");
    }

    enum METHOD_TYPE method = getMethodType(requestLine);

    char path[128];

    enum PATH_TYPE pathType = getPathType(path);

    int validProtocolStatus = isValidProtocol();

    if (method == METHOD_TYPE_UNKNOWN) {
        logsLevel("\nMethod Unknown", ERROR_LOGGING_LEVEL);
        generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Http Request invalid", responseStream);
        fclose(responseStream);
        strcat(originResponse, response);
        free(response);

        free(headers);
        return -1;
    }

    if (strlen(trim(body)) == 0) {
        if (method == METHOD_TYPE_POST || method == METHOD_TYPE_PUT) {
            logsLevel("Empty body with POST/PUT method", ERROR_LOGGING_LEVEL);
            generateResponseError(response, HTTP_STATUS_NOT_FOUND, "Empty body with POST/PUT method", responseStream);
            fclose(responseStream);
            strcat(originResponse, response);
            free(response);

            free(headers);
            return -1;
        }
    }

    if (validProtocolStatus != 0) {
        logsLevel("Http protocol version not supported", ERROR_LOGGING_LEVEL);
        generateResponseError(response, HTTP_STATUS_NOT_SUPPORTED, "HTTP Version Not Supported", responseStream);
        fclose(responseStream);
        strcat(originResponse, response);
        free(response);

        free(headers);
        return -1;
    }

    struct stat statbuf;

    if (pathType == PATH_TYPE_PATH) {
        if (method == METHOD_TYPE_GET || method == METHOD_TYPE_HEAD) {
            if (stat(path, &statbuf) < 0) {
                logsLevel("PATH - GET or HEAD - File not found", ERROR_LOGGING_LEVEL);
                generateResponseError(response, HTTP_STATUS_NOT_FOUND, "File not found", responseStream);
                fclose(responseStream);
                strcat(originResponse, response);
                free(response);

                free(headers);
                return -1;
            } else if (S_ISDIR(statbuf.st_mode)) {
                logsLevel("PATH - GET or HEAD - Directory", DEBUG_LOGGING_LEVEL);
                int getDirectoryStatus = getDirectory(responseStream, response, path, statbuf, method);
                if (getDirectoryStatus == -1) {
                    logsLevel("PATH - GET or HEAD - Directory not found", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_NOT_FOUND, "File not found",
                                          responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                } else if (getDirectoryStatus == -2) {
                    logsLevel("PATH - GET or HEAD - Directory malformation url", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Directories should en with backslash",
                                          responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                }
                fclose(responseStream);
                logsLevel("PATH - GET or HEAD - Directory success", DEBUG_LOGGING_LEVEL);
            } else {
                logsLevel("PATH - GET or HEAD - File", DEBUG_LOGGING_LEVEL);
                if (getFile(responseStream, response, path, method) == -1) {
                    logsLevel("PATH - GET or HEAD - File forbidden access", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_FORBIDDEN, "Forbidden access", responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                }
                fclose(responseStream);
                logsLevel("PATH - GET or HEAD - File success", DEBUG_LOGGING_LEVEL);
            }
        } else if (method == METHOD_TYPE_POST) {
            if (stat(path, &statbuf) < 0) {
                logsLevel("PATH - POST - File", DEBUG_LOGGING_LEVEL);
                if (addFile(responseStream, response, path, method, body, HTTP_STATUS_CREATED) == -1) {
                    logsLevel("PATH - POST - File failed to create", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_INTERNAL_SERVER_ERROR, "Internal Server Error",
                                          responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                }
                fclose(responseStream);
                logsLevel("PATH - POST - File created success", DEBUG_LOGGING_LEVEL);
            } else if (S_ISDIR(statbuf.st_mode)) {
                logsLevel("PATH - POST - Directory", ERROR_LOGGING_LEVEL);
                generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Path is a diretory on POST", responseStream);
                fclose(responseStream);
                strcat(originResponse, response);
                free(response);

                free(headers);
                return -1;
            } else {
                logsLevel("PATH - POST - File exists", ERROR_LOGGING_LEVEL);
                generateResponseError(response, HTTP_STATUS_CONFLICT, "File already exist", responseStream);
                fclose(responseStream);
                strcat(originResponse, response);
                free(response);

                free(headers);
                return -1;
            }
        } else if (method == METHOD_TYPE_PUT) {
            if (stat(path, &statbuf) < 0) {
                logsLevel("PATH - PUT - File", DEBUG_LOGGING_LEVEL);
                if (addFile(responseStream, response, path, method, body, HTTP_STATUS_CREATED) == -1) {
                    logsLevel("PATH - PUT - File failed to create", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_INTERNAL_SERVER_ERROR, "Internal Server Error",
                                          responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                }
                fclose(responseStream);
                logsLevel("PATH - PUT - File created success", DEBUG_LOGGING_LEVEL);
            } else if (S_ISDIR(statbuf.st_mode)) {
                logsLevel("PATH - PUT - Directory", ERROR_LOGGING_LEVEL);
                generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Path is a directory on PUT", responseStream);
                fclose(responseStream);
                strcat(originResponse, response);
                free(response);

                free(headers);
                return -1;
            } else {
                logsLevel("PATH - PUT - File replace case", DEBUG_LOGGING_LEVEL);
                if (addFile(responseStream, response, path, method, body, HTTP_STATUS_OK) == -1) {
                    logsLevel("PATH - PUT - File failed to replace", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_INTERNAL_SERVER_ERROR, "Internal Server Error",
                                          responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                }
                fclose(responseStream);
                logsLevel("PATH - PUT - File replaced success", DEBUG_LOGGING_LEVEL);
            }
        } else if (method == METHOD_TYPE_DELETE) {
            if (stat(path, &statbuf) < 0) {
                logsLevel("PATH - DELETE - File does not exists", ERROR_LOGGING_LEVEL);
                generateResponseError(response, HTTP_STATUS_NO_CONTENT, "File does not exist", responseStream);
                fclose(responseStream);
                strcat(originResponse, response);
                free(response);

                free(headers);
                return -1;
            } else if (S_ISDIR(statbuf.st_mode)) {
                logsLevel("PATH - DELETE - Directory", ERROR_LOGGING_LEVEL);
                generateResponseError(response, HTTP_STATUS_BAD_REQUEST, "Path is a directory on DELETE",
                                      responseStream);
                fclose(responseStream);
                strcat(originResponse, response);
                free(response);

                free(headers);
                return -1;
            } else {
                logsLevel("PATH - DELETE - File", DEBUG_LOGGING_LEVEL);
                if (deleteFile(responseStream, response, path, method) == -1) {
                    logsLevel("PATH - DELETE - File delete failed", ERROR_LOGGING_LEVEL);
                    generateResponseError(response, HTTP_STATUS_INTERNAL_SERVER_ERROR, "Internal Server Error",
                                          responseStream);
                    fclose(responseStream);
                    strcat(originResponse, response);
                    free(response);

                    free(headers);
                    return -1;
                }
                fclose(responseStream);
                logsLevel("PATH - DELETE - File deleted success", DEBUG_LOGGING_LEVEL);
            }
        } else {
            logsLevel("PATH - NOT IMPLEMENTED", ERROR_LOGGING_LEVEL);
            generateResponseError(response, HTTP_STATUS_NOT_IMPLEMENTED, "Not implemented case", responseStream);
            fclose(responseStream);
            strcat(originResponse, response);
            free(response);

            free(headers);
            return -1;
        }
    } else if (pathType == PATH_TYPE_INFO) {
        fprintf(stderr, "RESPONSE /INFO BEFORE");
        generateInfo(responseStream, response, method);
        fclose(responseStream);
        fprintf(stderr, "RESPONSE /INFO -> %s", response);
    } else if (pathType == PATH_TYPE_ENDPOINTS) {
        fprintf(stderr, "RESPONSE /ENDPOINTS BEFORE");
        generateSwagger(responseStream, response, method);
        fclose(responseStream);
        fprintf(stderr, "RESPONSE /ENDPOINTS -> %s", response);
    } else {
        logsLevel("Unknown use case", ERROR_LOGGING_LEVEL);
        generateResponseError(response, HTTP_STATUS_FORBIDDEN, "Forbidden access", responseStream);
        fclose(responseStream);
        strcat(originResponse, response);
        free(response);

        free(headers);
        return -1;
    }


    strcat(originResponse, response);

    free(response);

    free(headers);

    return 0;
}

void generateInfo(FILE *responseStream, char* response, int methodType) {
    char *body = NULL;
    size_t bufferSize = 0;
    FILE *bodyStream = open_memstream(&body, &bufferSize);

    fprintf(bodyStream, "<HTML><HEAD><TITLE>Server informations</TITLE></HEAD>\r\n<BODY>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>Server:</span> %s<br/><br/>", SERVER);
    fprintf(bodyStream, "<span style='font-weight: bold;'>Protocol HTTP:</span> %s<br/><br/>", PROTOCOL);
    fprintf(bodyStream, "<span style='font-weight: bold;'>Max connection:</span> %d<br/><br/>", CONNECTION);

    time_t now = time(0);
    char timebuf[128];

    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));

    fprintf(bodyStream, "<span style='font-weight: bold;'>Current time:</span> %s<br/><br/>", timebuf);

    fprintf(bodyStream, "<span style='font-weight: bold;'>Configuration debug:</span> %s<br/><br/>", convertBooleanToString(globalConfigurationProperties.debugMode));

    fprintf(bodyStream, "<span style='font-weight: bold;'>Configuration path:</span> %s<br/><br/>", globalConfigurationProperties.path);

    fprintf(bodyStream, "</BODY></HTML>");

    fclose(bodyStream);

    generateResponse(response, HTTP_STATUS_OK, body, responseStream, "text/html", methodType, NULL);

    free(body);
}

void generateSwagger(FILE *responseStream, char* response, int methodType) {
    char *body = NULL;
    size_t bufferSize = 0;
    FILE *bodyStream = open_memstream(&body, &bufferSize);

    fprintf(bodyStream, "<HTML><HEAD><TITLE>Endpoints</TITLE></HEAD>\r\n<BODY>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>GET:</span> %%/folder/%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>GET:</span> %%/file%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>HEAD:</span> %%/folder/%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>HEAD:</span> %%/file%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>POST:</span> %%/file%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>PUT:</span> %%/file%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>DELETE:</span> %%/file%%</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>%%PARAM%%:</span> /info</span><br/><br/>");
    fprintf(bodyStream, "<span style='font-weight: bold;'>%%PARAM%%:</span> /swagger</span><br/><br/>");

    fprintf(bodyStream, "</BODY></HTML>");

    fclose(bodyStream);

    generateResponse(response, HTTP_STATUS_OK, body, responseStream, "text/html", methodType, NULL);

    free(body);
}

const char *convertBooleanToString(int number) {
    if (number == 0) {
        return "false";
    } else {
        return "true";
    }
}

int deleteFile(FILE *responseStream, char *response, char *path, int methodType) {
    int result = remove(path);

    if (result != 0) {
        return -1;
    }

    generateResponse(response, HTTP_STATUS_OK, NULL, responseStream, "text/plain", methodType, NULL);
    return 0;
}

int addFile(FILE *responseStream, char *response, char *path, int methodType, char *bodyRequest, int httpStatus) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    int result = fputs(bodyRequest, file);
    if (result == EOF) {
        fprintf(stderr, "ADDFILE FAILED");
    }
    fclose(file);

    char extraHeader[4096];
    memset(extraHeader, 0, sizeof extraHeader);
    strcat(extraHeader, "Location: ");
    strcat(extraHeader, path);
    strcat(extraHeader, "\r\n");

    generateResponse(response, httpStatus, NULL, responseStream, getMimeType(path), methodType, extraHeader);

    return 0;
}

char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;
    if (!*s)
        return s;
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

char *trimStart(char *s) {
    while (isspace(s[0])) {
        s = s + 1;
    }
    return s;
}

int getDirectory(FILE *responseStream, char *response, char *path, struct stat statbuf, int methodType) {
    char pathbuf[4096];
    int len = strlen(path);
    if (len == 0 || path[len - 1] != '/') {
        return -2;
    } else {
        snprintf(pathbuf, sizeof(pathbuf), "%sindex.html", path);
        if (stat(pathbuf, &statbuf) >= 0) {
            if (getFile(responseStream, response, pathbuf, methodType) == -1) {
                return -1;
            }
        } else {
            DIR *dir;
            struct dirent *de;

            char *body = NULL;
            size_t bufferSize = 0;
            FILE *bodyStream = open_memstream(&body, &bufferSize);

            fprintf(bodyStream, "<HTML><HEAD><TITLE>Index of %s</TITLE></HEAD>\r\n<BODY>", path);
            fprintf(bodyStream, "<H4>Index of %s</H4>\r\n<PRE>\n", path);
            fprintf(bodyStream, "Name                             Last Modified              Size\r\n");
            fprintf(bodyStream, "<HR>\r\n");
            if (len > 1)
                fprintf(bodyStream, "<A HREF=\"..\">..</A>\r\n");

            dir = opendir(path);
            while ((de = readdir(dir)) != NULL) {
                char timebuf[32];
                struct tm *tm;

                strcpy(pathbuf, path);
                strcat(pathbuf, de->d_name);

                stat(pathbuf, &statbuf);
                tm = gmtime(&statbuf.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%d-%b-%Y %H:%M:%S", tm);

                fprintf(bodyStream, "<A HREF=\"%s%s\">", de->d_name, S_ISDIR(statbuf.st_mode) ? "/" : "");
                fprintf(bodyStream, "%s%s", de->d_name, S_ISDIR(statbuf.st_mode) ? "/</A>" : "</A> ");
                if (strlen(de->d_name) < 32)
                    fprintf(bodyStream, "%s", "test");
                if (S_ISDIR(statbuf.st_mode)) {
                    fprintf(bodyStream, "%s\r\n", timebuf);
                } else {
                    fprintf(bodyStream, "%s %10ld\r\n", timebuf, statbuf.st_size);
                }
            }
            closedir(dir);

            fprintf(bodyStream, "</PRE>\r\n<HR>\r\n<ADDRESS>%s</ADDRESS>\r\n</BODY></HTML>\r\n", SERVER);
            fclose(bodyStream);
            generateResponse(response, HTTP_STATUS_OK, body, responseStream, "text/html", methodType, NULL);
            free(body);
        }
    }
    return 0;
}

int getFile(FILE *responseStream, char *response, char *path, int methodType) {
    char data[4096];
    int n;

    char *body = NULL;
    size_t bufferSize = 0;
    FILE *bodyStream = open_memstream(&body, &bufferSize);

    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    } else {
        while ((n = fread(data, 1, sizeof(data), file)) > 0) {
            fwrite(data, 1, n, bodyStream);
        }
        fclose(file);
        fclose(bodyStream);
        generateResponse(response, HTTP_STATUS_OK, body, responseStream, getMimeType(path), methodType, NULL);
        free(body);
    }
    return 0;
}

char *
generateResponse(char *response, int httpStatus, const char *bodyResponse, FILE *responseStream, char *contentType,
                 int methodType, char *extraHeader) {
    blocLog(response, DEBUG_LOGGING_LEVEL, "[GENERATE-RESPONSE-BEFORE-START]", "[GENERATE-RESPONSE-BEFORE-END]");
    size_t bodyLength;
    if (bodyResponse == NULL) {
        bodyLength = 0;
    } else {
        bodyLength = strlen(bodyResponse);
    }
    generateResponseStatusLine(response, httpStatus, responseStream);
    generateResponseHeaders(response, contentType, bodyLength, responseStream, extraHeader);
    if (methodType == METHOD_TYPE_HEAD || bodyResponse == NULL) {
        fprintf(responseStream, "\r\n");
    } else {
        fprintf(responseStream, "\r\n%s", bodyResponse);
    }
    blocLog(response, DEBUG_LOGGING_LEVEL, "[GENERATE-RESPONSE-START]", "[GENERATE-RESPONSE-END]");
}

char *generateResponseError(char *response, int httpStatus, const char *bodyResponse, FILE *responseStream) {
    blocLog(response, DEBUG_LOGGING_LEVEL, "[ERROR-BEFORE-START]", "[ERROR-BEFORE-END]");
    generateResponseStatusLine(response, httpStatus, responseStream);
    generateResponseHeaders(response, "text/plain", strlen(bodyResponse), responseStream, NULL);
    fprintf(responseStream, "\r\n%s", bodyResponse);
    blocLog(response, DEBUG_LOGGING_LEVEL, "[ERROR-START]", "[ERROR-END]");
}

char *generateResponseStatusLine(char *response, int httpStatusEnum, FILE *responseStream) {
    switch (httpStatusEnum) {
        case HTTP_STATUS_OK:
            fprintf(responseStream, "%s 200 OK\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_CREATED:
            fprintf(responseStream, "%s 201 CREATED\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_NO_CONTENT:
            fprintf(responseStream, "%s 204 NO CONTENT\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_BAD_REQUEST:
            fprintf(responseStream, "%s 400 BAD REQUEST\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_FORBIDDEN:
            fprintf(responseStream, "%s 403 FORBIDDEN\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_NOT_FOUND:
            fprintf(responseStream, "%s 404 NOT FOUND\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_CONFLICT:
            fprintf(responseStream, "%s 409 CONFLICT\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_NOT_IMPLEMENTED:
            fprintf(responseStream, "%s 501 NOT IMPLEMENTED\r\n", PROTOCOL);
            break;
        case HTTP_STATUS_NOT_SUPPORTED:
            fprintf(responseStream, "%s 505 NOT SUPPORTED\r\n", PROTOCOL);
            break;
        default:
            fprintf(responseStream, "%s 500 INTERNAL SERVER ERROR\r\n", PROTOCOL);
            break;
    }
    return response;
}

char *
generateResponseHeaders(char *response, char *contentType, size_t bodyLength, FILE *responseStream, char *extraHeader) {
    time_t now = time(0);
    char timebuf[128];
    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));

    fprintf(responseStream, "Date: %s\r\n", timebuf);

    fprintf(responseStream, "Server: %s\r\n", SERVER);

    fprintf(responseStream, "Content-Type: %s\r\n", contentType);

    fprintf(responseStream, "Content-Length: %zu\r\n", bodyLength);

    if (extraHeader) {
        fprintf(responseStream, "%s", extraHeader);
    }

    return response;
}

char *getMimeType(char *name) {
    char *extension = strrchr(name, '.');
    if (!extension)
        return NULL;
    if (strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0)
        return "text/html";
    if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(extension, ".png") == 0)
        return "image/png";
    return "text/plain";
}