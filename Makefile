build: battleship.c
	gcc -Wall -pedantic -o battleship battleship.c -lcurses
clean: battleship
	rm battleship
run: battleship
	./battleship
