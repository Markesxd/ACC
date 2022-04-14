all:
	gcc -Wall compilador.c general.c ./lexico/lexico.c ./sintatico/sintatico.c -o run

debug:
	gcc -g compilador.c general.c ./lexico/lexico.c ./sintatico/sintatico.c -o run

sanitise:
	gcc -g3 -fsanitize=address compilador.c general.c ./lexico/lexico.c ./sintatico/sintatico.c -o run

clear:
	rm run
