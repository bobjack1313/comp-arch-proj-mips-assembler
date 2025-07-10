/*------------------------------------------------------------------------------
  File:        encoder.h
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Declares functions for encoding parsed MIPS instructions into
               32-bit binary machine code strings.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies:
    - parser.h -- for tokens
    - <string>, <vector>, <unordered_map>, <cstdint>
  -----------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>  
#include "parser.h" 

/**
 * Converts a list of parsed MIPS tokens into binary machine code strings.
 *
 * @param tokens - Parsed instructions of operators and operands
 * @param symbolTable - Maps labels to addresses in branches or jumps
 * @return vector of a string that represents a 32-character binary value
 */
std::vector<std::string> assemble(const std::vector<Token>& tokens,
                                  const std::unordered_map<std::string, uint32_t>& symbolTable);

/**
 * Encodes an R-type MIPS instruction into a 32-bit integer.
 *
 * @param funct - Function code  space of 6 bits
 * @param rs    - Source register space of 5 bits
 * @param rt    - Target register space of 5 bits
 * @param rd    - Destination register space of 5 bits
 * @return Encoded 32-bit integer representation
 */
uint32_t encode_R(uint32_t funct, uint32_t rs, uint32_t rt, uint32_t rd);

/**
 * Encodes an I-type MIPS instruction into a 32-bit integer.
 *
 * @param opcode - Opcode for the instruction space of 6 bits
 * @param rs     - Base/source register space of 5 bits
 * @param rt     - Target/destination register space of 5 bits
 * @param imm    - Immediate or offset value space of 16 bits
 * @return Encoded 32-bit integer representation
 */
uint32_t encode_I(uint32_t opcode, uint32_t rs, uint32_t rt, int16_t imm);

/**
 * Encodes a J-type MIPS instruction into a 32-bit integer.
 *
 * @param opcode - Opcode for jump space of 6 bits
 * @param address - Target address for jump space of 26 bits
 * @return Encoded 32-bit integer representation
 */
uint32_t encode_J(uint32_t opcode, uint32_t address);
