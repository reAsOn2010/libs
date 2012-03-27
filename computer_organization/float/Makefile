vpath %.c src
vpath %.o obj

src = src
obj = obj

float.out: float.o
	gcc -o float.out $(obj)/*.o
	cp $(src)/test.txt ./
float.o: float.c
	mkdir -p $(obj)
	gcc -c $(src)/float.c
	mv float.o $(obj)
clean:
	rm -rf $(obj)
	rm float.out
	rm test.txt
