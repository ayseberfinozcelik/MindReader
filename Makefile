CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Iinclude
BUILD_DIR := build
GAME_BIN := $(BUILD_DIR)/mind_reader
TEST_BIN := $(BUILD_DIR)/mind_reader_tests

.PHONY: all run test clean

all: $(GAME_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(GAME_BIN): src/main.cpp src/mind_reader_logic.cpp include/mind_reader_logic.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) src/main.cpp src/mind_reader_logic.cpp -o $(GAME_BIN)

$(TEST_BIN): tests/test_mind_reader.cpp src/mind_reader_logic.cpp include/mind_reader_logic.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tests/test_mind_reader.cpp src/mind_reader_logic.cpp -o $(TEST_BIN)

run: $(GAME_BIN)
	./$(GAME_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
