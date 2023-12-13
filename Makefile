CC = g++
FLAGS = -g -pthread

BIN = main

all: $(OBJECTS)
	$(CC) -o $(BIN) $(FLAGS) 

clean: 
	rm $(BIN)

run:
	./$(BIN)