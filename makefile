all: TaskRunLimit

CFLAGS+=-g

TaskRunLimit: main.o run_task.o
	gcc main.o run_task.o -o TaskRunLimit -luv -lc

main.o: main.c run_task.h

run_task.o: run_task.c run_task.h

clean:
	rm -rf TaskRunLimit *.o

clean_log:
	rm -rf err/* out/*