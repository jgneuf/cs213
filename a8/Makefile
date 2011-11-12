all: uthread ping_pong

uthread:
		gcc -c -o uthread.o uthread.c

ping_pong:
		gcc -o ping_pong ping_pong.c uthread.o -lpthread

clean:
		@rm -f uthread.o ping_pong

