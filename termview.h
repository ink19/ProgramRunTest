#ifndef __TERMVIEW_H
#define __TERMVIEW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>

#define INFO_LIST_LENGTH 100

typedef struct {
    u_int64_t task_id;
    u_int64_t run_time;
} termview_process_t;

extern int termview_init(u_int64_t task_number, u_int64_t process_number);
extern int termview_update_task(u_int64_t over_task);
extern int termview_update_process(u_int64_t process_id, u_int64_t run_id, u_int64_t run_time);
extern int termview_addinfo(char *info);
extern int termview_refresh();
extern int termview_destroy();

#endif