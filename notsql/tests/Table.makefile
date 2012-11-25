CURDIR := $(shell pwd)

vpath %.h $(CURDIR)/../Interpreter/lib
vpath %.h $(CURDIR)/../utils/lib
vpath %.h $(CURDIR)/../BTree/include
vpath %.h $(CURDIR)/../IndexManager/include
vpath %.h $(CURDIR)/../BufferManager/include
vpath %.h $(CURDIR)/../CatalogManager/lib
vpath %.h $(CURDIR)/../RecordManager/lib
vpath %.h $(CURDIR)/../API/lib
vpath %.cpp $(CURDIR)/../Interpreter/src
vpath %.cpp $(CURDIR)/../utils/src
vpath %.cpp $(CURDIR)/../BTree/src
vpath %.cpp $(CURDIR)/../IndexManager/src
vpath %.cpp $(CURDIR)/../BufferManager/src
vpath %.cpp $(CURDIR)/../CatalogManager/src
vpath %.cpp $(CURDIR)/../RecordManager/src
vpath %.cpp $(CURDIR)/../API/src

INCPATH = -I$(CURDIR)/../utils/lib -I$(CURDIR)/../Interpreter/lib -I$(CURDIR)/../API/lib \
		  -I$(CURDIR)/../IndexManager/include -I$(CURDIR)/../BufferManager/include \
		  -I$(CURDIR)/../CatalogManager/lib -I$(CURDIR)/../BTree/include \
		  -I$(CURDIR)/../RecordManager/lib -Wall

testTable: NotsqlTable.o testTable.o
	g++ -o testTable *.o $(INCPATH)

testTable.o : testTable.cpp
	g++ -c -g testTable.cpp $(INCPATH)

NotsqlTable.o : NotsqlTable.h
	g++ -c -g $(CURDIR)/../utils/src/NotsqlTable.cpp $(INCPATH)

clean :
	rm -vf *.o


