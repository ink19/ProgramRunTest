#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_task.h"

#include "log.h"

int main() {
    log_init(stderr);
    char *program_v[] = {
        "./TestProgram.bin",
        "",
        NULL
    };
    run_task_init(20, 2, 10000, program_v, 3, 1);
    run_task_start();
    run_task_destroy();
    log_destroy();
    return 0;
}
