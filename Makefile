compile: riverCrossing.c
	gcc -std=c99 -pthread riverCrossing.c -o riverCrossing.out -lm
run: ./riverCrossing.out
	./riverCrossing.out
