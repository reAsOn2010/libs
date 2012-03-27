vpath %.c src
vpath %.o obj

src = src
obj = obj

true_form.out: true_form.o
	gcc -o true_form.out $(obj)/*.o
	cp $(src)/test.txt ./
true_form.o: true_form.c
	mkdir -p $(obj)
	gcc -c $(src)/true_form.c
	mv true_form.o $(obj)
clean:
	rm -rf $(obj)
	rm true_form.out
	rm test.txt
