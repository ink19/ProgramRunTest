all: TaskRunLimit

CC=gcc

CFLAGS+=-g

TaskRunLimit: main.o run_task.o log.o record.o
	gcc $^ -o $@ -luv -lc

main.o: main.c run_task.h

record.o: record.h record.c

run_task.o: run_task.c run_task.h record.h

log.o: log.h log.c

clean:
	rm -rf TaskRunLimit *.o

clean_log:
	rm -rf err/* out/*