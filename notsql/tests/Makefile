INCPATH = -Icore -I../utils/lib -Itests -Icommon -Wall
testTable: testTable.o NotsqlTable.o 
	g++ -o testTable testTable.o NotsqlTable.o

testTable.o : testTable.cpp ../utils/lib/NotsqlTable.h 
	g++ -c testTable.cpp $(INCPATH)

NotsqlTable.o : ../utils/src/NotsqlTable.cpp ../utils/lib/NotsqlTable.h
	g++ -c ../utils/src/NotsqlTable.cpp $(INCPATH)

clean :
	rm *.o testTable
