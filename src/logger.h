#ifndef LOGGER_H
#define LOGGER_H

typedef enum
{
    LOGGER_SEVERITY_INFO = 0,
    LOGGER_SEVERITY_WARNING,
    LOGGER_SEVERITY_ERROR
} log_severity;

/**
 * @brief Log a message.
 *
 * @param severity
 * @param message
 * @param file
 * @param line
 */
void logger_log(log_severity severity, const char* message, const char* file, int line);

#ifdef LOGGER_ENABLED
#define __LOG(severity, message) logger_log((severity), (message), __FILE__, __LINE__)
#else
#define __LOG(severity, message) ((void)0)
#endif

#define LOG_INFO(message) __LOG(LOGGER_SEVERITY_INFO, message)
#define LOG_WARNING(message) __LOG(LOGGER_SEVERITY_WARNING, message)
#define LOG_ERROR(message) __LOG(LOGGER_SEVERITY_ERROR, message)

#endif /* LOGGER_H */