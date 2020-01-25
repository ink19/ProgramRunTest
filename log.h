#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include "termview.h"

#define DEBUG_LOG 1

extern int log_init(FILE *log_fd);
extern int log_data(char *level, char *format, ...);
extern int log_destroy();

#endif