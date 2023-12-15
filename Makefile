CC = g++
FLAGS = -pthread

BIN = main

all:
	$(CC) velha.cpp -o $(BIN) $(FLAGS) 

clean: 
	rm $(BIN)

run:
	./main