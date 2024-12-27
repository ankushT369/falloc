# Compiler and flags
CXX = g++
CXXFLAGS = -pthread

# Libraries
LIBS = -lgtest -lgtest_main

# Source files
SRC = unit.cc ../falloc.c

# Output binary
OUTPUT = exec

# Default target
all: $(OUTPUT)

# Link object files and create the output binary
$(OUTPUT): $(SRC)
	$(CXX) $(SRC) -o $(OUTPUT) $(LIBS) $(CXXFLAGS)

# Clean up generated files
clean:
	rm -f $(OUTPUT)

