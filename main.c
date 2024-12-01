#include "include/main.h"

struct GlobalConfigurationProperties globalConfigurationProperties;

int main() {
    globalConfigurationProperties = initGlobalConfigurationPropertiesDefault();

    char *buffer = 0;
    long length;
    cJSON *json;
    FILE *f = fopen("./cherokee-config.json", "rb");

    if (f) {
        logs("Config file found");
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    if (buffer) {
        json = cJSON_Parse(buffer);
        if (json) {
            cJSON *path = cJSON_GetObjectItem(json, "path");
            cJSON *debug = cJSON_GetObjectItem(json, "debug");
            if (path) {
                logsLevel(path->valuestring, DEBUG_LOGGING_LEVEL);
                strcpy(globalConfigurationProperties.path, path->valuestring);
            }
            if (debug) {
                logsLevel(debug->valuestring, DEBUG_LOGGING_LEVEL);
                globalConfigurationProperties.debugMode = debug->valueint;
            }
            logsLevel(cJSON_Print(json), DEBUG_LOGGING_LEVEL);
            cJSON_Delete(json);
        }
    }

    return runServer();
}