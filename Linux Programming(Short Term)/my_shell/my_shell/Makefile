all:my_shell ls grep

my_shell:my_shell.c
	gcc -o my_shell my_shell.c

ls:ls.c
	gcc -o ls ls.c
	mv ls bin/

grep:grep.c
	gcc -o grep grep.c
	mv grep bin/

clean:
	rm -f my_shell
	rm -f bin/*
