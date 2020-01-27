#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_task.h"

#include "log.h"

int main(int argc, char *argv[]) {
    int opt;
    uint64_t task_number, process_number, time_limit, loop_arg;
    char *optstring = "s:p:t:h";
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt)
        {
        case 's':
            task_number = atol(optarg);
            break;
        case 'p':
            process_number = atol(optarg);
            break;
        case 't':
            time_limit = atol(optarg);
            break;

        case 'h':
            printf("Usage: \t -h 打印帮助\n");
            printf("\t -s 设置总数\n");
            printf("\t -p 设置进程数\n");
            printf("\t -t 设置时间限制\n");
            return;
            break;
        default:
            break;
        }
    }


    log_init(stderr);
    char **program_v = argv + optind;
    
    for (int loop_i = 0; loop_i < argc - optind; ++loop_i) {
        if (strcmp(program_v[loop_i], "loop_arg") == 0) {
            loop_arg = loop_i;
            break;
        }
    }
    run_task_init(task_number, process_number, time_limit, program_v, argc - optind, loop_arg);
    run_task_start();
    run_task_destroy();
    log_destroy();
    return 0;
}
