/*------------------------------------------------------------------------------
  File:        converters.h
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Declares conversion utility functions used by the encoder.
               Includes functions for translating register names and integers
               into machine-usable formats like binary strings. 

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang 
  Date:        July 2025

  Dependencies:
    - <string>
    - <cstdint>
  -----------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <cstdint> 

/**
 * Converts a register name into its numeric value. 
 *
 * @param regName - Register name as a string - must start with '$'
 * @return Register number (0–31) if valid  
 * @throws std::runtime_error on unknown register
 */
int reg_number(const std::string& regName); 

/**
 * Converts a 32-bit unsigned integer to a 32-character binary string.
 *
 * @param value - Integer to convert 
 * @return String of 0s and 1s representing the binary value
 */
std::string to_binary32(uint32_t value);
