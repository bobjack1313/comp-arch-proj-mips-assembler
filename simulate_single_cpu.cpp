/*------------------------------------------------------------------------------
  File:        simulate_cpu.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project (Bonus)
  Purpose:     Single CPU simulator driver that loads binary MIPS instructions
               and executes them using TinyMipsCPU.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025
------------------------------------------------------------------------------*/
#include "tiny_mips_cpu.h"
#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char* argv[]) {
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

    // Check file read
    string line;
    while (getline(inputFile, line)) {
        cout << line << endl;
    }

	// Read file ilne by line


	// Convert the strings to binary

	// Load binary into simulated_single_cpu 


    return 0;
}