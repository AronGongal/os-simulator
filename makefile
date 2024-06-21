all: main.c cpu.c memory.c disk.c
	gcc -g main.c cpu.c memory.c disk.c scheduler.c smm.c -o main.o
