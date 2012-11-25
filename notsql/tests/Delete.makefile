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

testDelete: testDelete.o NotsqlInterpreter.o NotsqlExpr.o \
	NotsqlBufferManager.o NotsqlBTree.o NotsqlTable.o NotsqlAPI.o\
	NotsqlCatalogDatabase.o NotsqlCatalogManager.o NotsqlRecordManager.o \
	NotsqlIndexManager.o
	g++ -o testDelete *.o $(INCPATH)

testDelete.o : testDelete.cpp NotsqlInterpreter.h NotsqlAPI.h
	g++ -c -g testDelete.cpp $(INCPATH)

NotsqlInterpreter.o : NotsqlInterpreter.cpp NotsqlConstants.h NotsqlBaseException.h NotsqlExpr.h NotsqlAPI.h
	g++ -c -g $(CURDIR)/../Interpreter/src/NotsqlInterpreter.cpp $(INCPATH)

NotsqlAPI.o : NotsqlAPI.cpp
	g++ -c -g $(CURDIR)/../API/src/NotsqlAPI.cpp $(INCPATH)

NotsqlExpr.o : NotsqlExpr.cpp NotsqlConstants.h
	g++ -c -g $(CURDIR)/../utils/src/NotsqlExpr.cpp $(INCPATH)

NotsqlIndexManager.o : NotsqlIndexManager.h NotsqlBTree.h
	g++ -c -g $(CURDIR)/../IndexManager/src/NotsqlIndexManager.cpp $(INCPATH)

NotsqlTable.o : NotsqlTable.h
	g++ -c -g $(CURDIR)/../utils/src/NotsqlTable.cpp $(INCPATH)

NotsqlBTree.o : NotsqlBTree.h
	g++ -c -g $(CURDIR)/../BTree/src/NotsqlBTree.cpp $(INCPATH)

NotsqlCatalogManager.o : NotsqlCatalogManager.h
	g++ -c -g $(CURDIR)/../CatalogManager/src/NotsqlCatalogManager.cpp $(INCPATH)

NotsqlCatalogDatabase.o : NotsqlCatalogDatabase.h
	g++ -c -g $(CURDIR)/../CatalogManager/src/NotsqlCatalogDatabase.cpp $(INCPATH)

NotsqlRecordManager.o : NotsqlRecordManager.h NotsqlAPI.h
	g++ -c -g $(CURDIR)/../RecordManager/src/NotsqlRecordManager.cpp $(INCPATH)

NotsqlBufferManager.o : NotsqlBufferManager.h
	g++ -c -g $(CURDIR)/../BufferManager/src/NotsqlBufferManager.cpp $(INCPATH)

clean :
	rm -vf *.o


