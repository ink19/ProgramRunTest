#include "run_task.h"

int run_task_init(u_int64_t sum, u_int64_t thread_number, u_int64_t plimit_time, char *program_argv[], int64_t program_arg_length) {
    //初始化运行参数
    task_sum = sum;
    task_number = thread_number;
    limit_time = plimit_time;
    loop = uv_default_loop();
    task = (task_t *)malloc(sizeof(task_t) * thread_number);
    
    //初始化队列
    task_queue.head = 0;
    task_queue.tail = 0;
    task_queue.length = thread_number * 2;
    task_queue.data = (u_int64_t *)malloc(sizeof(int64_t) * thread_number * 2);

    uv_idle_init(loop, &start_thread);

    //初始化每一个进程
    for (int i = 0; i < thread_number; i++) {
        //初始化参数
        (task + i)->program_argv = (char **)malloc(sizeof(char *) * program_arg_length);
        for (int j = 0; j < program_arg_length; j++) {
            (task + i)->program_argv[j] = program_argv[j];
        }
        (task + i)->program_argv[1] = (char *)malloc(sizeof(char) * 100);
        
        (task + i)->option.stdio_count = 3;
        (task + i)->option.stdio = (uv_stdio_container_t *)malloc(sizeof(uv_stdio_container_t) * 3);
        (task + i)->option.file = (task + i)->program_argv[0];
        (task + i)->option.stdio[0].flags = UV_IGNORE;
        (task + i)->option.stdio[1].flags = UV_WRITABLE_PIPE;
        (task + i)->option.stdio[1].data.stream = (uv_stream_t *)&((task + i)->out);
        (task + i)->option.stdio[2].flags = UV_WRITABLE_PIPE;
        (task + i)->option.stdio[2].data.stream = (uv_stream_t *)&((task + i)->err);
        (task + i)->option.exit_cb = process_on_exit;
        
        (task + i)->process.data = (void *)i;
        (task + i)->timer.data = (void *)i;
        (task + i)->err.data = (void *)i;
        (task + i)->out.data = (void *)i;
        task_queue.data[task_queue.head++] = i;
    }
    uv_idle_init(loop, &start_thread);
    uv_idle_start(&start_thread, start_task_loop);
}

int run_task_destroy() {
    free(task_queue.data);
    for (int i = 0; i < task_number; i++) {
        free(task[task_number].program_argv[1]);
        free(task[task_number].program_argv);
        free((task + task_number)->option.stdio);
    }
    free(task);
    return 0;
}

void start_task_loop(uv_idle_t *handle) {
    if (now_task_id >= task_sum) {
        uv_idle_stop(handle);
        return;
    }
    while (task_queue.head != task_queue.tail) {
        uint64_t start_id = task_queue.data[task_queue.tail++];
        start_task(start_id);
    }
}

int start_task(u_int64_t number) {
    char filename[100];
    (task + number)->task_id = number;
    uv_timer_init(loop, &((task + number)->timer));
    uv_pipe_init(loop, &((task + number)->err), 1);
    uv_pipe_init(loop, &((task + number)->out), 1);
    (task + number)->begin_time = uv_now(loop);
    sprintf((task + number)->program_argv[1], "%d", now_task_id);

    sprintf(filename, "%s%d.data", OUTDIR, now_task_id);
    fclose(fopen(filename, "w"));
    uv_pipe_bind(&((task + number)->out), filename);

    sprintf(filename, "%s%d.data", ERRDIR, now_task_id);
    fclose(fopen(filename, "w"));
    uv_pipe_bind(&((task + number)->err), filename);

    int return_id = 0;
    if ((return_id = uv_spawn(loop, &((task + number)->process), &((task + number)->option)))) {
        fprintf(stderr, "%s\n", uv_strerror(return_id));
    }

    uv_timer_start(&((task + number)->timer), timer_on_exit, limit_time, 0);
    return 0;
}

int run_task_start() {
    return uv_run(loop, UV_RUN_DEFAULT);
}

void timer_on_exit(uv_timer_t * handle) {
    int process_id = (int)handle->data;
    int task_id = (task + process_id)->task_id;
    uv_kill(uv_process_get_pid(&((task + process_id)->process)), 9);
    uv_timer_stop(handle);
    uv_close((uv_handle_t *)handle, NULL);
}

void process_on_exit(uv_process_t *handle, int64_t exit_status, int term_signal) {
    int process_id = (int)handle->data;
    int task_id = (task + process_id)->task_id;

    uv_timer_stop(&((task + process_id)->timer));
    uv_close((uv_handle_t *)&((task + process_id)->timer), NULL);

    fprintf(stdout, "%ld,%ld", task_id, uv_now(loop) - (task + process_id)->begin_time);
    uv_close((uv_handle_t *)handle, NULL);
    task_queue.data[task_queue.head++] = process_id;
}