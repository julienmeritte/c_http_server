/*
** ETNA PROJECT, 02/01/2023 by meritt_j, vignan_q
** logging.h
** File description:
**      headers for logging file
*/

#ifndef H_PROPERTIES
#define H_PROPERTIES

#include <stdbool.h>

struct GlobalConfigurationProperties {
    int debugMode;
    char path[4096];
};

struct GlobalConfigurationProperties
initGlobalConfigurationPropertiesFromFile(struct GlobalConfigurationProperties defaultConfigurationProperties);

struct GlobalConfigurationProperties initGlobalConfigurationPropertiesDefault();

#endif