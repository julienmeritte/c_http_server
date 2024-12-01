#include "../include/logging.h"
#include <criterion/criterion.h>
#include <criterion/logging.h>

struct GlobalConfigurationProperties globalConfigurationProperties;


Test(logsLevel_function , simple_defined) {
    cr_log_warn("logsLevel - Function | testing existing function");
    globalConfigurationProperties = initGlobalConfigurationPropertiesDefault();
    logsLevel("" , DEBUG_LOGGING_LEVEL);
}

Test(logs_function , simple_defined) {
    cr_log_warn("logs - Function | testing existing function");
    globalConfigurationProperties = initGlobalConfigurationPropertiesDefault();
    logs("");
}

Test(blocLog_function , simple_defined) {
    cr_log_warn("blocLog - Function | testing existing function");
    globalConfigurationProperties = initGlobalConfigurationPropertiesDefault();
    blocLog("test blocLog" , DEBUG_LOGGING_LEVEL , "t" , "t");
}