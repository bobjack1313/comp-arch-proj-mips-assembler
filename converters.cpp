/*------------------------------------------------------------------------------
  File:        converters.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project 
  Purpose:     Implements conversion functions for register names and binary
               string formatting. Used by the encoder during instruction encoding.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2  
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies: 
    - converters.h
    - <stdexcept>, <unordered_map>, <bitset> 
  -----------------------------------------------------------------------------*/
#include "converters.h"
#include <unordered_map> 
#include <stdexcept>
#include <bitset> 

using namespace std;

// Map of register names to numbers 
static const unordered_map<string, int> registerMap = {  
    {"$zero", 0}, {"$at", 1},
    {"$v0", 2}, {"$v1", 3}, 
    {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, 
    {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
    {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},  
    {"$t8", 24}, {"$t9", 25}, 
    {"$k0", 26}, {"$k1", 27}, 
    {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31} 
};

/**
 * Converts a register name like "$t0" to its corresponding register number.
 * 
 * Throws an error if the register is unrecognized.
 */
int reg_number(const string& regName) { 
    auto it = registerMap.find(regName); 
    if (it != registerMap.end()) {
        return it->second; 
    }
    // Throw unknowns
    throw runtime_error("Unknown register: " + regName);
}

/**
 * Converts a 32-bit unsigned integer into a string of 0s and 1s.
 *
 * Example: 5 ---> "00000000000000000000000000000101"
 */
string to_binary32(uint32_t value) { 
    return bitset<32>(value).to_string();
}
