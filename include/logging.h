/*
** ETNA PROJECT, 02/01/2023 by meritt_j, vignan_q
** logging.h
** File description:
**      headers for logging file
*/

#ifndef H_LOGGING
#define H_LOGGING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

enum LOGGING {
    INFO_LOGGING_LEVEL, DEBUG_LOGGING_LEVEL, WARNING_LOGGING_LEVEL, ERROR_LOGGING_LEVEL
};

struct LoggingLevelStruct {
    char *info;
    char *debug;
    char *warning;
    char *error;
};

void logs(char *str);

void logsLevel(char *str, enum LOGGING level);

void blocLog(char *str, enum LOGGING level, char *start, char *end);

#endif