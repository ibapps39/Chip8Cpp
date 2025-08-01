# make CC=clang++, etc
CC ?= g++
CFLAGS = -march=native -O3 -Wall -std=c++17 -Wno-missing-braces
LDFLAGS =  
INCLUDES = -I./include

# SDL2
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)


# Directory structure
SRC_DIR = ./src
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj

# Files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
TARGET = $(BUILD_DIR)/main

# Create required directories
$(shell mkdir -p $(BUILD_DIR) $(OBJ_DIR))

# Default target to build the program
all: $(TARGET)

# Ensure directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link objects into executable
$(TARGET): $(BUILD_DIR) $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS) || ($(MAKE) clean && exit 1)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ || ($(MAKE) clean && exit 1)


# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# test:
# 	rm -rf $(OBJ_DIR)/*.o $(TARGET)
# 	make -j `sysctl -n hw.ncpu` all
JOBS := $(shell nproc 2>/dev/null || sysctl -n hw.ncpu)
fast:
	make -j $(JOBS) all

# Rebuild everything from scratch
rebuild: clean all

fast_rebuild: clean fast

# Generate dependency files
depend: $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -MM $^ | sed 's|^|$(OBJ_DIR)/|' > .depend

-include .depend

.PHONY: all clean rebuild depend test fast fast_rebuild
