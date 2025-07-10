#------------------------------------------------------------------------------
# Makefile for Tiny MIPS Assembler - CSCE 5610 Group Project 
#------------------------------------------------------------------------------ 
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Source files 
SRC = tiny_mips_asm.cpp parser.cpp encoder.cpp converters.cpp
HDR = parser.h encoder.h converters.h tiny_mips_asm.h

# Output binary  
TARGET = tiny_mips_asm

# Default rule  
all: $(TARGET)

# Link object files into final executable 
$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# Rebuild everything 
rebuild: clean all
