#ifndef _RECORD_H
#define _RECORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>

extern int record_init(char *record_filename);
extern int record_data(char *format, ...);
extern int record_destroy();

#endif