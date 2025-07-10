/*------------------------------------------------------------------------------
  File:        tiny-mips-asm.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Main driver for the Tiny MIPS Assembler. This file reads a MIPS
               assembly source file, parses it, encodes instructions to binary,
               and writes the output to a file.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE 5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies:
    - parser.h: for parsing instructions and building the symbol table
    - encoder.h: for translating parsed instructions into machine code
    - helpers.h: for utility functions (register mapping, trimming, etc.)
    - <fstream>, <iostream>, <vector>, <string>, <unordered_map>
  -----------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

#include "parser.h"
#include "encoder.h"
#include "helpers.h"
#include "tiny-mips-asm.h"

/**
 * Runs the assembler using an input and output file path.
 *
 * @param inputFilePath - Path to the .s file containing MIPS assembly
 * @param outputFilePath - Path to output file where binary will be written
 * @return 0 if successful, 1 on error
 */
int runAssembler(const string& inputFilePath, const string& outputFilePath) {
    // Open the input assembly file from user
    ifstream inputFile(inputFilePath);
    if (!inputFile) {
        cerr << "Error: Cannot open input file: " << inputFilePath << endl;
        return 1;
    }

    // Read the entire input file into a list of strings (line-by-line)
    vector<string> sourceLines;
    string line;
  
    while (getline(inputFile, line)) {
        sourceLines.push_back(line);
    }
    inputFile.close();

    // Symbol table will store labels and address mappings
    unordered_map<string, uint32_t> symbolTable;

    // Instructions are tokenized and syumbol table created (Part of first pass)
    vector<Token> tokens = parse(sourceLines, symbolTable);

    // Encode parsed instructions into 32-bit binary strings (Part of second pass)
    vector<string> binaryOutput = assemble(tokens, symbolTable);

    // Open the output file for writing the encoded machine code
    ofstream outputFile(outputFilePath);
    if (!outputFile) {
        cerr << "Error: Cannot open output file: " << outputFilePath << endl;
        return 1;
    }

    // Write each encoded binary instruction to the output file
    for (const auto& binary : binaryOutput) {
        outputFile << binary << '\n';
    }
    outputFile.close();

    // Display confirmation message to user
    cout << "Assembled " << binaryOutput.size() << " instruction(s) to " << outputFilePath << endl;
    return 0;
}

/**
 * Main function: handles command-line arguments and runs the assembler.
 *
 * Usage:
 *   ./tiny-mips-asm input.s output.txt
 */
int main(int argc, char* argv[]) {
    // Check that the correct num of args are used
    if (argc != 3) {
        cerr << "Usage: tiny-mips-asm <input_file.s> <output_file.txt>\n";
        return 1;
    }

    // Exec assembler with input and output file paths
    return runAssembler(argv[1], argv[2]);
}
