all: TaskRunLimit

CC=gcc

CFLAGS+=-g

LDFLAGS+=-L/usr/lib64 -luv -lcurses

TaskRunLimit: main.o run_task.o log.o record.o termview.o
	gcc $^ -o $@ $(LDFLAGS)

main.o: main.c run_task.h log.h

termview.o: termview.c termview.h

record.o: record.h record.c

run_task.o: run_task.c run_task.h record.h termview.h

log.o: log.h log.c termview.h

test: test.c
	gcc $^ -o $@ $(LDFLAGS)
	
#test.o: test.c

clean:
	rm -rf TaskRunLimit *.o test a.out runtime.data

clean_log:
	rm -rf err/* out/*
	