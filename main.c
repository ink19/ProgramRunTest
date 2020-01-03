#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

uv_loop_t *loop;
uv_process_t child_req;
u_int64_t begin_time;
uv_timer_t timer;
char *program = "./TestProgram.bin";
char *program_argv[] = {
    "./TestProgram.bin",
    "",
    NULL
};

void pon_exit(uv_process_t *req, int64_t exit_status, int term_signal)  {
    printf("Exit.\n");
    uv_timer_stop(&timer);
    uv_close((uv_handle_t *)req, NULL);
}

void ton_stop(uv_timer_t* handle) {
    uv_kill(child_req.pid, SIGKILL);
    printf("%ld\n", uv_now(loop) - begin_time);
    uv_timer_stop(handle);
    uv_close((uv_handle_t *)handle, NULL);
}

int main() {
    loop = uv_default_loop();
    begin_time = uv_now(loop);
    char sleep_time[100];
    sprintf(sleep_time, "%d", 7);
    program_argv[1] = sleep_time;
    
    uv_process_options_t options = {0};
    options.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];
    child_stdio[0].flags = UV_IGNORE;
    child_stdio[1].flags = UV_INHERIT_FD;
    child_stdio[1].data.fd = 1;
    child_stdio[2].flags = UV_INHERIT_FD;
    child_stdio[2].data.fd = 2;
    options.stdio = child_stdio;
    options.file = program;
    options.args = program_argv;
    options.exit_cb = pon_exit;
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        printf("%s\n", uv_strerror(r));
    } 
    
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, ton_stop, 5000, 0);
    return uv_run(loop, UV_RUN_DEFAULT);
}