#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
int main(int argc, char *argv[]) {
    int opt;
    uint64_t task_number, process_number, time_limit;
    char *optstring = "s:p:t:";
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt)
        {
        case 's':
            task_number = atol(optarg);
            printf("Task %ld\n", task_number);
            break;
        case 'p':
            process_number = atol(optarg);
            printf("Process %ld\n", process_number);
            break;
        case 't':
            time_limit = atol(optarg);
            printf("Time_limit: %ld\n", time_limit);
            break;
        default:
            break;
        }
    }
    printf("%d %d\n", argc, optind);
    
    
    return 0;
}