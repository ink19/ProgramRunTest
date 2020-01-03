#ifndef _RUN_TASK_H
#define _RUN_TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

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
    uv_pipe_t out;
    uv_pipe_t err;
    u_int64_t task_id;
} task_t;

uv_loop_t *loop;
task_t *task;
//启动进程的idle循环
uv_idle_t start_thread;
task_queue_t task_queue;

u_int64_t task_number = 3;
u_int64_t task_sum = 100;
u_int64_t now_task_id = 0;
u_int64_t limit_time;

int run_task_init(u_int64_t sum, u_int64_t thread_number, u_int64_t plimit_time, char *program_argv[], int64_t program_arg_length);
int start_task(u_int64_t number);
int run_task_start();
int run_task_destroy();

void timer_on_exit(uv_timer_t* handle);
void process_on_exit(uv_process_t *req, int64_t exit_status, int term_signal);
void start_task_loop(uv_idle_t *handle);

#endif