#include "run_task.h"

static uv_loop_t *loop;
static run_process_t *process_list;
//启动进程的idle循环
static uv_idle_t start_thread;
static task_queue_t task_queue;
static int start_thread_flag = 0;

//刷新UI
static int ui_refresh_flags = 0;
static uv_timer_t ui_refresh;

static u_int64_t argv_loop_n = 2;
static u_int64_t process_number = 3;
static u_int64_t task_sum = 100;
static u_int64_t now_task_id = 1;
static u_int64_t limit_time = 20000;
static u_int64_t over_task = 0;

//刷新UI
static void run_task_view_refresh(uv_timer_t * handle) {
    int stop_process = 0;
    for (int loop_i = 0; loop_i < process_number; ++loop_i) {
        if (process_list[loop_i].err == -1) {
            stop_process++;
        }
    }
    if (ui_refresh_flags && stop_process == process_number) {
        uv_timer_stop(&ui_refresh);
        uv_close((uv_handle_t *)&ui_refresh, NULL);
    }
    termview_update_task(over_task);
    for (u_int64_t loop_i = 0; loop_i < process_number; ++loop_i) {
        if (uv_is_closing((uv_handle_t *)&process_list[loop_i].timer)) {
            termview_update_process(loop_i, process_list[loop_i].task_id, 0);
        } else {
            termview_update_process(loop_i, process_list[loop_i].task_id, uv_now(loop) - process_list[loop_i].begin_time);
        }
        termview_update_process(loop_i, process_list[loop_i].task_id, uv_now(loop) - process_list[loop_i].begin_time);
    }
    termview_refresh();
}

//初始化
int run_task_init(u_int64_t begin, u_int64_t sum, u_int64_t _process_number, u_int64_t plimit_time, char *program_v[], int64_t program_arg_length, u_int64_t _argv_loop_n) {
    //初始化运行参数
    record_init("runtime.data");
    
    now_task_id = begin;
    if (begin == 0) {
        over_task = 0;
        termview_init(sum + 1, _process_number);
    } else {
        over_task = begin - 1;
        termview_init(sum, _process_number);
    }
    
    task_sum = sum;
    argv_loop_n = _argv_loop_n;
    process_number = _process_number;
    limit_time = plimit_time;
    loop = uv_default_loop();
    process_list = (run_process_t *)malloc(sizeof(run_process_t) * process_number);
    
    //初始化队列
    task_queue.head = 0;
    task_queue.tail = 0;
    task_queue.length = process_number * 2;
    task_queue.data = (u_int64_t *)malloc(sizeof(int64_t) * process_number * 2);

    uv_idle_init(loop, &start_thread);

    //初始化每一个进程
    for (int i = 0; i < process_number; i++) {
        //初始化参数
        (process_list + i)->program_argv = (char **)malloc(sizeof(char *) * (program_arg_length + 1));
        for (int j = 0; j < program_arg_length; j++) {
            (process_list + i)->program_argv[j] = program_v[j];
        }
        (process_list + i)->program_argv[argv_loop_n] = (char *)malloc(sizeof(char) * 100);
        (process_list + i)->program_argv[program_arg_length] = NULL;
        memset(&((process_list + i)->option), sizeof(uv_process_t), 0);

        (process_list + i)->option.stdio_count = 3;
        (process_list + i)->option.stdio = (uv_stdio_container_t *)malloc(sizeof(uv_stdio_container_t) * 3);
        (process_list + i)->option.file = (process_list + i)->program_argv[0];
        (process_list + i)->option.stdio[0].flags = UV_IGNORE;
        (process_list + i)->option.stdio[1].flags = UV_INHERIT_FD;
        (process_list + i)->option.stdio[2].flags = UV_INHERIT_FD;
        (process_list + i)->option.exit_cb = process_on_exit;
        (process_list + i)->option.args = (process_list + i)->program_argv;
        (process_list + i)->process.data = (void *)i;
        (process_list + i)->timer.data = (void *)i;
        (process_list + i)->file_req.data = (void *)i;
        (process_list + i)->err = -1;
        (process_list + i)->out = -1;
        task_queue.data[task_queue.head++] = i;
        task_queue.head %= task_queue.length;
    }
    uv_idle_init(loop, &start_thread);
    uv_idle_start(&start_thread, start_task_loop);
    uv_timer_init(loop, &ui_refresh);
    uv_timer_start(&ui_refresh, run_task_view_refresh, 10, 1000);
    start_thread_flag = 1;
}

int run_task_destroy() {
    record_destroy();
    termview_destroy();
    
    free(task_queue.data);
    for (int i = 0; i < process_number; i++) {
        free(process_list[i].program_argv[argv_loop_n]);
        free(process_list[i].program_argv);
        free((process_list + i)->option.stdio);
        uv_fs_req_cleanup(&process_list[i].file_req);
    }
    free(process_list);
    return 0;
}

void start_task_loop(uv_idle_t *handle) {
    if (now_task_id > task_sum) {
        uv_idle_stop(handle);
        ui_refresh_flags = 1;
        return;
    }

    //开启新的进程，首先开启文件读写
    while (task_queue.head != task_queue.tail) {
        uint64_t start_id = task_queue.data[task_queue.tail++];
        task_queue.tail %= task_queue.length;
        log_data("INFO", "OUT %d,%d,%d", task_queue.head, task_queue.tail, start_id);
        start_task(start_id);
    }

    //停止循环，节省CPU
    uv_idle_stop(handle);

    //flag置零
    start_thread_flag = 0;
}

//开启任务
int start_task(u_int64_t number) {
    //初始化计时器时间
    uv_timer_init(loop, &((process_list + number)->timer));
    
    //输入参数
    (process_list + number)->task_id = now_task_id;
    sprintf((process_list + number)->program_argv[argv_loop_n], "%d", now_task_id);
    // int loop_i = 0;
    // while ((process_list + number)->program_argv[loop_i] != NULL) {
    //     printf("%s ", (process_list + number)->program_argv[loop_i]);
    //     loop_i++;
    // }
    // printf("\n");
    //打开文件读写
    now_task_id++;
    sprintf((process_list + number)->filename, "%s%d.data", OUTDIR, (process_list + number)->task_id);
    uv_fs_open(loop, &((process_list + number)->file_req), (process_list + number)->filename, O_RDWR|O_CREAT|O_TRUNC, 0644, file_open_cb);
    return 0;
}

void file_open_cb(uv_fs_t* req) {
    int process_id = (int)(req->data);
    int task_id = (process_list + process_id)->task_id;
    //判断是否完成文件打开操作
    if (process_list[process_id].out == -1) {
        process_list[process_id].out = req->result;
        //打开错误输出文件
        sprintf((process_list + process_id)->filename, "%s%d.data", ERRDIR, (process_list + process_id)->task_id);
        uv_fs_open(loop, &((process_list + process_id)->file_req), (process_list + process_id)->filename, O_RDWR|O_CREAT|O_TRUNC, 0644, file_open_cb);
        return;
    } else {
        //完成文件打开
        process_list[process_id].err = req->result;
        //printf("fd:%d,%d\n", task[process_id].out, task[process_id].err);
        //printf("%s\n", req->path);
        (process_list + process_id)->option.stdio[1].data.fd = process_list[process_id].out;
        (process_list + process_id)->option.stdio[2].data.fd = process_list[process_id].err;
        //开启进程
        int return_id = 0;
        if ((return_id = uv_spawn(loop, &((process_list + process_id)->process), &((process_list + process_id)->option)))) {
            fprintf(stderr, "%s\n", uv_strerror(return_id));
        }
        //开启计时器
        (process_list + process_id)->begin_time = uv_now(loop);
        uv_timer_start(&((process_list + process_id)->timer), timer_on_exit, limit_time, 0);
        return;
    }
}

//运行主循环
int run_task_start() {
    return uv_run(loop, UV_RUN_DEFAULT);
}

//计时器退出函数
void timer_on_exit(uv_timer_t * handle) {
    int process_id = (int)handle->data;
    int task_id = (process_list + process_id)->task_id;
    uv_kill(uv_process_get_pid(&((process_list + process_id)->process)), 9);
    uv_timer_stop(handle);
    uv_close((uv_handle_t *)handle, NULL);
}

void file_close_cb(uv_fs_t* req) {
    int process_id = (int)(req->data);
    int task_id = (process_list + process_id)->task_id;
    //判断是否完成进程文件的关闭
    if (process_list[process_id].out != -1) {
        //printf("out_id:%d\n", task[process_id].out);
        process_list[process_id].out = -1;
        uv_fs_close(loop, &(process_list[process_id].file_req), process_list[process_id].err, file_close_cb);
        return;
    } else {
        process_list[process_id].err = -1;

        //压入任务
        task_queue.data[task_queue.head++] = process_id;
        task_queue.head %= task_queue.length;
        log_data("INFO", "IN %d,%d,%d", task_queue.head, task_queue.tail, process_id);
        if (start_thread_flag == 0) {
            uv_idle_start(&start_thread, start_task_loop);
            start_thread_flag = 1;
        }
        return;
    }
}

void process_on_exit(uv_process_t *handle, int64_t exit_status, int term_signal) {
    int process_id = (int)handle->data;
    int task_id = (process_list + process_id)->task_id;
    over_task++;
    //关闭计时器
    uv_timer_stop(&((process_list + process_id)->timer));
    if (!uv_is_closing((uv_handle_t *)&(process_list + process_id)->timer)) {
        uv_close((uv_handle_t *)&((process_list + process_id)->timer), NULL);
    }

    //关闭文件读写
    uv_fs_close(loop, &(process_list[process_id].file_req), process_list[process_id].out, file_close_cb);

    //打印时间
    record_data("%ld,%ld", task_id, uv_now(loop) - (process_list + process_id)->begin_time);
    log_data("INFO", "%ld,%ld", task_id, uv_now(loop) - (process_list + process_id)->begin_time);
    uv_close((uv_handle_t *)handle, NULL);
    
    // task_queue.data[task_queue.head++] = process_id;
    // task_queue.head %= task_queue.length;
}
