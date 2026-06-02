#include "logger.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

static const char* __get_timestamp(void)
{
    static char timestamp[20];
    time_t now;
    struct tm* tm_info;

    now = time(NULL);
    tm_info = localtime(&now);

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    return timestamp;
}

void logger_log(log_severity severity, const char* message, const char* file, int line)
{
    switch (severity)
    {
        case LOGGER_SEVERITY_INFO:
            fprintf(stdout, "[%s][INFO]: %s\n", __get_timestamp(), message);
            break;

        case LOGGER_SEVERITY_WARNING:
            fprintf(stderr, "[%s][WARNING] %s:%d: %s\n", __get_timestamp(), file, line, message);
            break;

        case LOGGER_SEVERITY_ERROR:
            fprintf(stderr, "[%s][ERROR] %s:%d: %s\n", __get_timestamp(), file, line, message);
            break;
    }
}
