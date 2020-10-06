#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "run_task.h"

#include "log.h"

int main(int argc, char *argv[]) {
    int opt;
    uint64_t task_number = 10, begin_index = 0,process_number = 2, time_limit = 10000, loop_arg = 1;
    char *optstring = "c:b:s:p:t:h";
    char *run_program;
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt)
        {
        case 'b':   
            begin_index = atol(optarg);
            break;
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
            printf("Usage: \t -h 打印帮助\n\r");
            printf("\t -b 设置开始（包含）\n\r");
            printf("\t -s 设置结束（包含）\n\r");
            printf("\t -p 设置进程数\n\r");
            printf("\t -t 设置时间限制\n\r");
            printf("\t -c 运行的指令\n\r");
            return 0;
            break;
        case 'c':
            run_program = optarg;
            break;
        default:
            break;
        }
    }


    log_init(stderr);

    int program_cmd_number = 1;
    char *program_v_p = (char *)malloc(sizeof(char) * (strlen(run_program) + 1));
    memcpy(program_v_p, run_program, sizeof(char) * (strlen(run_program) + 1));
    for (int loop_i = 0; loop_i < strlen(run_program); ++loop_i) {
        if (program_v_p[loop_i] == ' ') {
            program_v_p[loop_i] = 0;
            program_cmd_number++;
        }
    }

    char **program_v = (char **)malloc(sizeof(char *) * (program_cmd_number + 1));

    program_v[0] = program_v_p;
    int temp_index = 1;
    for (int loop_i = 0; loop_i < strlen(run_program); ++loop_i) {
        if (program_v_p[loop_i] == 0) {
            program_v[temp_index] = program_v_p  + loop_i + 1;
            temp_index++;
        }
    }
    
    for (int loop_i = 0; loop_i < program_cmd_number; ++loop_i) {
        if (strcmp(program_v[loop_i], "loop_arg") == 0) {
            loop_arg = loop_i;
            break;
        }
    }
    run_task_init(begin_index, task_number, process_number, time_limit, program_v, program_cmd_number, loop_arg);
    run_task_start();
    run_task_destroy();
    log_destroy();
    return 0;
}
