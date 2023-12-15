CC = g++
FLAGS = -pthread -g

BIN = velha

all:
	$(CC) $(FLAGS) velha.cpp -o $(BIN)

clean: 
	rm $(BIN)

run:
	./$(BIN)
