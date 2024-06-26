all:
	cc main.c -o main -lraylib -lm -ggdb
clean:
	rm main
