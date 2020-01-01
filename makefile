all: TaskRunLimit

TaskRunLimit: main.c
	gcc main.c -o TaskRunLimit -luv

clean:
	rm -rf TaskRunLimit