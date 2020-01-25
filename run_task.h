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
} mtask_t;

static uv_loop_t *loop;
static mtask_t *task;
//启动进程的idle循环
static uv_idle_t start_thread;
static task_queue_t task_queue;
static int start_thread_flag = 0;

//刷新UI
static uv_timer_t ui_refresh;

static u_int64_t argv_loop_n = 2;
static u_int64_t task_number = 3;
static u_int64_t task_sum = 100;
static u_int64_t now_task_id = 1;
static u_int64_t limit_time = 20000;
static u_int64_t over_task = 0;

int run_task_init(u_int64_t sum, u_int64_t thread_number, u_int64_t plimit_time, char *program_argv[], int64_t program_arg_length, u_int64_t _argv_loop_n);
int start_task(u_int64_t number);
int run_task_start();
int run_task_destroy();

void timer_on_exit(uv_timer_t* handle);
void process_on_exit(uv_process_t *req, int64_t exit_status, int term_signal);
void start_task_loop(uv_idle_t *handle);
void file_open_cb(uv_fs_t* req);
void file_close_cb(uv_fs_t* req);

#endif
