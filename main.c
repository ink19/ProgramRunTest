#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_task.h"

#include "log.h"

int main() {
    log_init(stderr);
    char *program_v[] = {
        "python",
        "CAPGRun.py",
        "",
        NULL
    };
    run_task_init(201, 2, 600000, program_v, 4, 2);
    run_task_start();
    run_task_destroy();
    log_destroy();
    return 0;
}
