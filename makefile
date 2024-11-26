# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -I./include -I/usr/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lSDL2_ttf

# Targets
TARGET = minesweeper
SOURCES = main.cpp render.cpp game.cpp settings.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS)
