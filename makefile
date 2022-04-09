all:
	gcc compilador.c general.c ./lexico/lexico.c -o run

debug:
	gcc -g compilador.c general.c ./lexico/lexico.c -o run

clear:
	rm run
