#include "termview.h"

static int task_number = 0;
static int process_number = 0;
static int now_task = 0;
static termview_process_t *process_data;


extern int termview_init(int _task_number, int _process_number) {
    task_number = _task_number;
    process_number = _process_number;
    process_data = (termview_process_t *)malloc(sizeof(termview_process_t) * process_number);
    for (int loop_i = 0; loop_i < process_number; ++loop_i) {
        process_data[loop_i].run_time = 0;
        process_data[loop_i].task_id = -1;
    }
    initscr();
    termview_refresh();
    return 0;
}

extern int termview_update_task(int over_task) {
    now_task = over_task;
    return 0;
}
extern int termview_update_process(int process_id, int run_id, int run_time) {
    process_data[process_id].task_id = run_id;
    process_data[process_id].run_time = run_time;
    return 0;
}

extern int termview_refresh() {
    int scrh, scrw;
    int linebuff[1024];
    int shape_number = 0;
    clear();
    getmaxyx(stdscr, scrh, scrw);
    sprintf(linebuff, "%d/%d", now_task, task_number);
    shape_number = (scrw - strlen(linebuff) - 4) * now_task / task_number;
    addch('[');
    while(shape_number--) {
        addch('#');
    }
    addch(']');
    mvprintw(0, scrw - strlen(linebuff) - 1, linebuff);
    printw("\n");
    printw("process number: %d\n", process_number);
    for (int loop_i = 0; loop_i < process_number; ++loop_i) {
        int run_id = process_data[loop_i].task_id;
        int run_time = process_data[loop_i].run_time;
        printw("%d. %d", loop_i, run_id);
        sprintf(linebuff, "%d", run_time);
        mvprintw(loop_i + 2, scrw - strlen(linebuff) - 1, linebuff);
        printw("\n");
    }
    refresh();
    return 0;
}

extern int termview_destroy() {
    free(process_data);
    endwin();
    return 0;
}
