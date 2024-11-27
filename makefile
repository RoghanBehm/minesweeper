# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -I./include -I/usr/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lSDL2

# Directories and files
TARGET = bin/minesweeper
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Build target
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# GDB target
gdb: $(TARGET)
	gdb $(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS)
	rm -rf $(OBJ_DIR) $(BIN_DIR)
