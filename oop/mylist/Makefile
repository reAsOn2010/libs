test = test/letters.txt
src = src
obj = obj

vpath %.cpp src
vpath %.o obj
vpath %.txt test

main: main.o
	g++ -g -o main $(obj)/*.o
main.o: main.cpp
	mkdir -p obj
	g++ -c $(src)/main.cpp
	mv main.o obj
clean:
	rm -rf obj/
	rm -f main
