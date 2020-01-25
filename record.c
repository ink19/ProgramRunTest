#include "record.h"

static FILE * record_fd;
static pthread_mutex_t record_mutex;

extern int record_init(char *filename) {
    record_fd = fopen(filename, "wb+");
    pthread_mutex_init(&record_mutex, NULL);
    return 0;
}

extern int record_data(char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    pthread_mutex_lock(&record_mutex);
    vfprintf(record_fd, format, arg_list);
    fprintf(record_fd, "\n");
    fflush(record_fd);
    va_end(arg_list);
    pthread_mutex_unlock(&record_mutex);
    return 0;
}

extern int record_destroy() {
    pthread_mutex_destroy(&record_mutex);
    return 0;
}