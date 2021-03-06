#include "log.h"

static FILE * log_fd;
static pthread_mutex_t log_mutex;

extern int log_init(FILE *plog_fd) {
#if DEBUG_LOG == 1
    log_fd = plog_fd;
    pthread_mutex_init(&log_mutex, NULL);
#endif
    return 0;
}

extern int log_data(char *level, char *format, ...) {
#if DEBUG_LOG == 1
    char *info = (char *)malloc(sizeof(char) * 1024);
    va_list arg_list;
    va_start(arg_list, format);
    pthread_mutex_lock(&log_mutex);
    sprintf(info, "[%s] ", level);
    vsprintf(info + strlen(info), format, arg_list);
    termview_addinfo(info);
    // fprintf(log_fd, "[%s] ", level);
    // vfprintf(log_fd, format, arg_list);
    // fprintf(log_fd, "\n");
    // fflush(log_fd);
    va_end(arg_list);
    pthread_mutex_unlock(&log_mutex);
#endif
    return 0;
}

extern int log_destroy() {
#if DEBUG_LOG == 1
    pthread_mutex_destroy(&log_mutex);
#endif
    return 0;
}