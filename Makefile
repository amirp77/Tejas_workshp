all:
	gcc main.c graph.c -o main -g -lpthread

clean:
	rm main *.o
