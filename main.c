#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_task.h"

int main() {
    char *program_v[] = {
        "./TestProgram.bin",
        "",
        NULL
    };
    run_task_init(100, 5, 2000, program_v, 3);
    run_task_start();
    run_task_destroy();
    return 0;
}