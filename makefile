all: play.c start.c
	gcc -o play.o play.c -lcurses;gcc -o start.o start.c -lcurses

play: play.c
	gcc -o play.o play.c -lcurses

start: start.c
	gcc -o start.o start.c -lcurses