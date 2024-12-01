/*
** ETNA PROJECT, 02/01/2023 by meritt_j, vignan_q
** logging.c
** File description:
**      Logging handling file
*/

#include "../include/logging.h"

extern struct GlobalConfigurationProperties globalConfigurationProperties;

void logs(char *str) {
    if(strlen(str) == 0) {
        return;
    }
    else {
        fprintf(stderr, "\n[INFO] - %s", str);
    }
}

void logsLevel(char *str, enum LOGGING level) {
    if (level == 0) {
        logs(str);
    } else if (level == 1) {
        if (globalConfigurationProperties.debugMode) {
            fprintf(stderr, "\n[DEBUG] - %s", str);
        }

    } else if (level == 2) {
        fprintf(stderr, "\n[WARNING] - %s", str);
    } else if (level == 3) {
        fprintf(stderr, "\n[ERROR] - %s", str);
    } else {
        logsLevel("Wrong Logging level, exit application...", ERROR_LOGGING_LEVEL);
        exit(0);
    }
}

void blocLog(char *str, enum LOGGING level, char *start, char *end) {
    logsLevel(start, level);
    logsLevel(str, level);
    logsLevel(end, level);
}