#ifndef __TERMVIEW_H
#define __TERMVIEW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

typedef struct {
    int task_id;
    int run_time;
} termview_process_t;

extern int termview_init(int task_number, int process_number);
extern int termview_update_task(int over_task);
extern int termview_update_process(int process_id, int run_id, int run_time);
extern int termview_refresh();
extern int termview_destroy();


#endif