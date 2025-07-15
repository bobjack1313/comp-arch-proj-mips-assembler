/*------------------------------------------------------------------------------
  File:        parser.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Implements parsing logic for MIPS assembly source code.
               Extracts instruction tokens and populates the label symbol table.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies:
    - parser.h
    - <sstream>, <algorithm>, <cstdint>
  -----------------------------------------------------------------------------*/
#include "parser.h"
#include <sstream>
#include <algorithm>
#include <cstdint>

using namespace std;


// Helper function to trim leading and trailing whitespace from a string.
static string trim(const string& s) {
    const char* whitespace = " \t\n\r";
    size_t start = s.find_first_not_of(whitespace);
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

// Helper function to split a string by commas or whitespace into tokens.
static vector<string> splitArguments(const string& str) {
    vector<string> args;
    stringstream ss(str);
    string token;

    while (getline(ss, token, ',')) {
        stringstream inner(token);
        string word;
        while (inner >> word) {
            args.push_back(word);
        }
    }
    return args;
}

/**
 * Main parsing function. Reads cleaned assembly lines and returns Token objects.
 * Also builds the symbol table in a first pass.
 */
vector<Token> parse(const vector<string>& lines,
                         unordered_map<string, uint32_t>& symTable) {
    vector<Token> tokens;
    // Program counter starts at 0, incremented by 4 per instruction
    uint32_t pc = 0;  

    for (const string& rawLine : lines) {
        // Remove comments - starting with #
        // NOTE: Should we include // ???
        string line = rawLine;
        size_t commentPos = line.find('#');
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);
        }
        line = trim(line);
      
        // Skip blank lines
        if (line.empty())
          continue;

        // Label check in loop
        size_t colonPos = line.find(':');
        if (colonPos != string::npos) {
            string label = trim(line.substr(0, colonPos));
            
            // Map label to current instruction address
            symTable[label] = pc;  

            // Check for an instruction after the label
            if (colonPos + 1 < line.length()) {
                line = trim(line.substr(colonPos + 1));
            } else {
                // No instruction here
                continue;  
            }
        }

        if (line.empty()) 
          continue;

        // Extract operation - the first word
        stringstream ss(line);
        string op;
        ss >> op;

        // Extract remaining string as arguments
        string argString;
        getline(ss, argString);
        vector<string> args = splitArguments(argString);

        // Add token to the list
        tokens.push_back({op, args});
        // Advance the instruction by 4 bytes
        pc += 4;  
    }
    return tokens;
}
