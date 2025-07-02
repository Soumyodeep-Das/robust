# All C tests must use the Unity framework and .h headers. Do NOT use assert.h for any C test files.
# All C++ tests use GoogleTest and .hpp headers.
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Iinclude -Isrc -Itests
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Isrc -pthread
GTEST_LIBS = -lgtest -lgtest_main
UNITY_DIR = tests/unity
UNITY_SRC = $(UNITY_DIR)/unity.c
UNITY_INC = -I$(UNITY_DIR)
SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin
LOG_DIR = logs

# Find all source and test files
SRC_C_FILES := $(wildcard $(SRC_DIR)/*.c)
SRC_CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_C_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_CPP_FILES := $(wildcard $(TEST_DIR)/*.cpp)

# Output binaries
C_OUTPUT = $(BIN_DIR)/test_output_c
CPP_OUTPUT = $(BIN_DIR)/test_output_cpp

# Default: build both, prefer C++ if both exist
run: run_cpp run_c

run_cpp: $(CPP_OUTPUT)
	@echo "Running C++ tests (GoogleTest):"
	./$(CPP_OUTPUT) || true

run_c: $(C_OUTPUT)
	@echo "Running C tests (Unity):"
	./$(C_OUTPUT) || true

# Build C++ tests with GoogleTest
$(CPP_OUTPUT): $(SRC_CPP_FILES) $(TEST_CPP_FILES)
	@mkdir -p $(BIN_DIR) $(LOG_DIR)
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LIBS) -o $@ 2> $(LOG_DIR)/build.log

# Build C tests with Unity
$(C_OUTPUT): $(SRC_C_FILES) $(TEST_C_FILES) $(UNITY_SRC)
	@mkdir -p $(BIN_DIR) $(LOG_DIR)
	$(CC) $(CFLAGS) $(UNITY_INC) $^ -o $@ 2> $(LOG_DIR)/build.log

clean:
	rm -rf $(BIN_DIR) $(LOG_DIR)

# Usage:
#   make run_c     # Run C tests (Unity)
#   make run_cpp   # Run C++ tests (GoogleTest)
#   make run       # Run both




# # Makefile

# CXX=g++
# CXXFLAGS=-std=c++17 -Wall -Wextra -pthread
# SRC=src/main.cpp
# TEST=tests/test_main.cpp
# OUT=bin/test_output

# all: $(OUT)

# $(OUT): $(SRC) $(TEST)
# 	$(CXX) $(CXXFLAGS) $(SRC) $(TEST) -lgtest -lgtest_main -o $(OUT) 2> logs/build.log

# run: $(OUT)
# 	./$(OUT)

# clean:
# 	rm -f $(OUT) logs/build.log
