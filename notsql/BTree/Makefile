SRC=src
SRCFILE=$(SRC)/*
BIN=bin
TARGET=NotsqlBTree
OBJ=$(TARGET).o
INCPATH=-I./include

vpath %.cpp $(SRC)
vpath %.o $(BIN)

all:$(TARGET)

$(TARGET):$(OBJ)
	g++ -o NotsqlBTree -g $(BIN)/$(OBJ) 

$(OBJ):$(SRCFILE)
	g++ -c -g $(SRCFILE)  $(INCPATH)
	mv $(OBJ) $(BIN)/

clean:
	rm -f $(TARGET)
	rm -f $(BIN)/*
