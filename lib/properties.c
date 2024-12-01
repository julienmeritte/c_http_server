#include "../include/properties.h"

struct GlobalConfigurationProperties
initGlobalConfigurationPropertiesFromFile(struct GlobalConfigurationProperties defaultConfigurationProperties) {
    // On fait la récupération depuis fichier json ici
}

struct GlobalConfigurationProperties initGlobalConfigurationPropertiesDefault() {
    const struct GlobalConfigurationProperties localConfigurationDefault = {
            .debugMode = 0,
            .path = "/home/cherokee/public"
    };
    return localConfigurationDefault;
}
