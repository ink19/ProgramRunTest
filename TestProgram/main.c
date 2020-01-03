#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int arg, char *argv[]) {
    if (arg < 2) {
        fprintf(stderr, "参数不足\n");
        return 1;
    }

    int sleep_time = atoi(argv[1]);
    fprintf(stdout, "Sleep Time %d.\n", sleep_time);
    fprintf(stderr, "Errer Test\n");
    sleep(sleep_time);
    fprintf(stdout, "Sleep Over.\n");
    return 0;
}