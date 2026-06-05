# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -O2 -Wall -Wno-missing-braces

# Libraries (Using the system ones installed via pacman)
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

# The main source file
SRC = main.cpp

# The output executable
TARGET = game.exe

# The build command
all:
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LIBS)

# Clean up
clean:
	rm -f $(TARGET)
