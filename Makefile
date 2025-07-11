#------------------------------------------------------------------------------
# Makefile for Tiny MIPS Assembler & CPU Simulator - CSCE 5610 Group Project
#------------------------------------------------------------------------------

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Source files
# Assembler
ASM_SRC = tiny_mips_asm.cpp parser.cpp encoder.cpp converters.cpp
ASM_HDR = parser.h encoder.h converters.h tiny_mips_asm.h

# Single CPU
CPU_SRC = simulate_single_cpu.cpp tiny_mips_cpu.cpp
CPU_HDR = simulate_single_cpu.h tiny_mips_cpu.h

# Output binaries
ASM_TARGET = tiny_mips_asm
CPU_TARGET = simulate_single_cpu

# Default rule
all: $(ASM_TARGET) $(CPU_TARGET)

# Assembler build rule
$(ASM_TARGET): $(ASM_SRC) $(ASM_HDR)
	$(CXX) $(CXXFLAGS) $(ASM_SRC) -o $(ASM_TARGET)

# CPU simulator build rule
$(CPU_TARGET): $(CPU_SRC) $(CPU_HDR)
	$(CXX) $(CXXFLAGS) $(CPU_SRC) -o $(CPU_TARGET)

# Clean up build artifacts
clean:
	rm -f $(ASM_TARGET) $(CPU_TARGET)

# Rebuild everything
rebuild: clean all