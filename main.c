#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_task.h"

#include "log.h"

int main() {
    log_init(stderr);
    char *program_v[] = {
        "java",
        "-jar",
        "MAPFRScala.jar",
        "",
        "1",
        "1",
        NULL
    };
    run_task_init(1800, 2, 600000, program_v, 7, 3);
    run_task_start();
    run_task_destroy();
    log_destroy();
    return 0;
}