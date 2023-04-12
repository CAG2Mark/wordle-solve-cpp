CC = g++
CFLAGS = -g -std=c++17 -Wall -O3
CFLAGS += -Isrc

SRC = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = build

.PHONY: all clean

main: $(OBJ)
	mkdir -p ./$(BIN)
	$(CC) -o $(BIN)/main $^ $(LDFLAGS) $(CFLAGS)

run: main
	$(BIN)/main

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

json: clean
	bear -- make main

clean:
	rm -rf $(BIN) $(OBJ)

gencombos:
	$(CC) scripts/gen_combos.cpp -o scripts/gen $(CFLAGS)