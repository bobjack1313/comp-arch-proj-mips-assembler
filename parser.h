/*------------------------------------------------------------------------------
  File:        parser.h
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Declares structures and functions for parsing MIPS assembly source
               lines and constructing the symbol table.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies:
    - <string>, <vector>, <unordered_map>, <cstdint>
  -----------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>
#include <unordered_map> 
#include <cstdint> 

// Represents a parsed instruction with its operation and operands
struct Token {
    // Operation (e.g., "add", "lw", "beq")
    std::string op; 
    // Operands (e.g., "$t0", "$a1", "label") 
    std::vector<std::string> args;  
};

/**
 * Parses a list of MIPS assembly lines into Token objects.
 * Also builds a symbol table mapping labels to instruction addresses.
 *
 * @param lines     - Assembly lines read from source file
 * @param symTable  - Symbol table to populate with label addresses
 * @return A vector of Token structs representing parsed instructions 
 */
std::vector<Token> parse(const std::vector<std::string>& lines,
                         std::unordered_map<std::string, uint32_t>& symTable);
