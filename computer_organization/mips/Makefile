vpath %.c src
vpath %.o obj

src = src
obj = obj

mips.out: mips.o
	gcc -g -o mips.out $(obj)/*.o
	cp $(src)/instructions.txt ./
mips.o: mips.c
	mkdir -p $(obj)
	gcc -c $(src)/mips.c
	mv mips.o $(obj)
clean:
	rm -rf $(obj)
	rm mips.out
	rm instructions.txt
	rm out
