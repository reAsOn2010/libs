SRC=src
SRCFILE=$(SRC)/*
BIN=bin
BINFILE=$(BIN)/*
TARGET=NotsqlIndexManager
OBJ = NotsqlIndexManager.o NotsqlBTree.o NotsqlBufferManager.o NotsqlTable.o
INCLUDE=include
INCPATH=-I./include

vpath %.cpp $(SRC)
vpath %.o $(BIN)
vpath %.h $(INCLUDE)

All : $(OBJ)
	g++ -o $(TARGET) $(BINFILE)

NotsqlTable.o : NotsqlTable.h
	g++ -c -g $(SRC)/NotsqlTable.cpp $(INCPATH) -Wno-write-strings
	mv NotsqlTable.o $(BIN)

NotsqlBTree.o : NotsqlBTree.h
	g++ -c -g $(SRC)/NotsqlBTree.cpp $(INCPATH) -Wno-write-strings
	mv NotsqlBTree.o $(BIN)

NotsqlBufferManager.o : NotsqlBufferManager.h
	g++ -c -g $(SRC)/NotsqlBufferManager.cpp $(INCPATH) -Wno-write-strings
	mv NotsqlBufferManager.o $(BIN)

NotsqlIndexManager.o : NotsqlBufferManager.h NotsqlBTree.h NotsqlIndexManager.h
	g++ -c -g $(SRC)/NotsqlIndexManager.cpp $(INCPATH) -Wno-write-strings
	mv NotsqlIndexManager.o $(BIN)


clean:
	rm -vf $(TARGET)
	rm -vf $(BINFILE)
