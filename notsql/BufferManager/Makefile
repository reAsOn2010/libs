SRC=src
SRCFILE=$(SRC)/*
BIN=bin
TARGET=NotsqlBufferManager
OBJ=$(TARGET).o
INCPATH=-I./include

vpath %.cpp $(SRC)
vpath %.o   $(BIN)

all:$(TARGET)

$(TARGET):$(OBJ)
	g++ -o $(TARGET) -g $(BIN)/$(OBJ)

$(OBJ):$(SRCFILE)
	g++ -c -g $(SRCFILE) $(INCPATH)
	mv $(OBJ) $(BIN)/

clean:
	rm -vf $(TARGET)
	rm -vf $(BIN)/*
