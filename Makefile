# Makefile

CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pthread
SRC=src/main.cpp
TEST=tests/test_main.cpp
OUT=bin/test_output

all: $(OUT)

$(OUT): $(SRC) $(TEST)
	$(CXX) $(CXXFLAGS) $(SRC) $(TEST) -lgtest -lgtest_main -o $(OUT) 2> logs/build.log

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT) logs/build.log
