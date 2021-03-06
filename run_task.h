#ifndef _RUN_TASK_H
#define _RUN_TASK_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include "log.h"
#include "record.h"
#include "termview.h"
#define ERRDIR "./err/"
#define OUTDIR "./out/"

typedef struct {
    u_int64_t *data;
    u_int64_t head;
    u_int64_t tail;
    u_int32_t length;
} task_queue_t;

typedef struct {
    u_int64_t begin_time;
    uv_timer_t timer;
    uv_process_t process;
    uv_process_options_t option;
    char **program_argv;
    uv_file out;
    uv_file err;
    uv_fs_t file_req;
    u_int64_t task_id;
    char filename[100];
} run_process_t;

int run_task_init(u_int64_t begin, u_int64_t sum, u_int64_t thread_number, u_int64_t plimit_time, char *program_argv[], int64_t program_arg_length, u_int64_t _argv_loop_n);
int start_task(u_int64_t number);
int run_task_start();
int run_task_destroy();

void timer_on_exit(uv_timer_t* handle);
void process_on_exit(uv_process_t *req, int64_t exit_status, int term_signal);
void start_task_loop(uv_idle_t *handle);
void file_open_cb(uv_fs_t* req);
void file_close_cb(uv_fs_t* req);

#endif
