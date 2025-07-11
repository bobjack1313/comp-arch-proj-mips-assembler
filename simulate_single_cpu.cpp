/*------------------------------------------------------------------------------
  File:        simulate_single_cpu.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project (Bonus)
  Purpose:     Single CPU simulator driver that loads binary MIPS instructions
               and executes them using TinyMipsCPU.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025
------------------------------------------------------------------------------*/
#include "simulate_single_cpu.h"
#include "tiny_mips_cpu.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

using namespace std;


int main(int argc, char* argv[]) {
    DEBUG_MODE = true; 
	// Check input file validity
    if (argc != 2) {
        cerr << "Usage: ./simulate_single_cpu <binary_file.txt>\n";
        return 1;
    }
    // Open file to read contents
    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Error: Cannot open file " << argv[1] << '\n';
        return 1;
    }

    // Create vector for 32 bit instr
    vector<uint32_t> instructions;
    // String for each line read
    string line;
    while (getline(inputFile, line)) {
        if (line.length() == 32) {
            uint32_t binary = bitset<32>(line).to_ulong();
            instructions.push_back(binary);
        }
    }
    TinyMipsCPU cpu;

    cout << "Initial Register State:\n";
    cpu.displayRegisters();

    // Load instruction vector with the bitsets
    cpu.loadProgram(instructions);
    cpu.executeProgram();

    cout << "\nFinal Register State:\n";
    cpu.displayRegisters();

    return 0;
}