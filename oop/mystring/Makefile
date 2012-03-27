test = test/letters.txt
csrc = src
obj = obj

vpath %.cpp src
vpath %.o obj
vpath %.txt test

string: main.o
	cp $(test) ./
	g++ -o string $(obj)/*.o
main.o: main.cpp
	mkdir -p obj
	g++ -c $(csrc)/main.cpp
	mv main.o obj
clean:
	rm -rf obj/
	rm -f string
	rm -f letters.txt
