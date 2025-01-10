# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -I./include -I/usr/include/SDL2 -D_REENTRANT
LDFLAGS  = -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lSDL2_ttf

# Directories
SRC_DIRS  = src client
OBJ_DIR   = obj
BIN_DIR   = bin
TARGET    = $(BIN_DIR)/minesweeper

SOURCES = $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.cpp))

OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)


$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)          
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


gdb: $(TARGET)
	gdb $(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
