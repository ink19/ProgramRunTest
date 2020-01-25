#include "termview.h"

static u_int64_t task_number = 0;
static u_int64_t process_number = 0;
static u_int64_t now_task = 0;
static termview_process_t *process_data;
static char *info_list[INFO_LIST_LENGTH];
static char info_begin_index = 0;

//初始化终端视图
extern int termview_init(u_int64_t _task_number, u_int64_t _process_number) {
    task_number = _task_number;
    process_number = _process_number;
    process_data = (termview_process_t *)malloc(sizeof(termview_process_t) * process_number);
    for (u_int64_t loop_i = 0; loop_i < process_number; ++loop_i) {
        process_data[loop_i].run_time = 0;
        process_data[loop_i].task_id = 0;
    }
    for (int loop_i = 0; loop_i < 10; ++loop_i) {
        info_list[loop_i] = NULL;
    }
    initscr();
    termview_refresh();
    return 0;
}

//添加信息
extern int termview_addinfo(char *info) {
    // int loop_index = (info_begin_index + 9) % 10;
    // while (info_list[loop_index] != NULL) {

    // }
    if (info_list[info_begin_index] != NULL) {
        free(info_list[info_begin_index]);
    } 
    info_list[info_begin_index] = info;
    info_begin_index += 1;
    info_begin_index %= INFO_LIST_LENGTH;
    return 0;
}

//更新完成任务数量
extern int termview_update_task(u_int64_t over_task) {
    now_task = over_task;
    return 0;
}

//更新进程进度
extern int termview_update_process(u_int64_t process_id, u_int64_t run_id, u_int64_t run_time) {
    process_data[process_id].task_id = run_id;
    process_data[process_id].run_time = run_time;
    return 0;
}

//刷新
extern int termview_refresh() {
    int scrh, scrw;
    char linebuff[1024];
    int shape_number = 0;
    clear();
    getmaxyx(stdscr, scrh, scrw);

    //打印任务进度条
    sprintf(linebuff, "%ld/%ld", now_task, task_number);
    shape_number = (scrw - strlen(linebuff) - 4) * now_task / task_number;
    addch('[');
    for(int loop_i = 0; loop_i < shape_number; ++loop_i) {
        addch('#');
    }
    for (int loop_i = 0; loop_i < scrw - strlen(linebuff) - 4 - shape_number; ++loop_i) {
        addch('-');
    }
    addch(']');
    //打印进程信息
    mvprintw(0, scrw - strlen(linebuff) - 1, linebuff);
    printw("\n");
    printw("process number: %ld\n", process_number);
    for (int loop_i = 0; loop_i < process_number; ++loop_i) {
        int run_id = process_data[loop_i].task_id;
        int run_time = process_data[loop_i].run_time;
        printw("%ld. %ld", loop_i, run_id);
        sprintf(linebuff, "%ld", run_time);
        mvprintw(loop_i + 2, scrw - strlen(linebuff) - 1, linebuff);
        printw("\n");
    }
    //打印信息
    printw("----------------------------\n");
    int loop_index = (info_begin_index + INFO_LIST_LENGTH - 1)%INFO_LIST_LENGTH;
    while (getcury(stdscr) < getmaxy(stdscr) - 1) {
        if (info_list[loop_index] == NULL) break;
        printw("%s\n", info_list[loop_index], getcury(stdscr), getmaxy(stdscr));
        loop_index = (loop_index + INFO_LIST_LENGTH - 1)%INFO_LIST_LENGTH;
        if (loop_index == info_begin_index) break;
    }
    refresh();
    return 0;
}

//销毁
extern int termview_destroy() {
    free(process_data);
    for (int loop_i = 0; loop_i < 10; ++loop_i) {
        free(info_list[loop_i]);
    }
    endwin();
    return 0;
}
