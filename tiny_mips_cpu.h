/*------------------------------------------------------------------------------
  File:        tiny_mips_cpu.h
  Project:     Tiny MIPS CPU - CSCE 5610 Group Project
  Purpose:     Header file for the cpu driver of cpu simulator. Declares the 
               interface for a simple single-cycle MIPS CPU simulator.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Description:
               Defines the TinyMipsCPU class that simulates execution of 32-bit 
               binary MIPS instructions. Supports a basic register file, 
               instruction memory, and memory for load/store operations. 
               Implements 10 instructions:

               - R-type: add, sub, and, or, slt, nor
               - I-type: lw, sw, beq
               - J-type: j

               The class is designed to test output from the Tiny MIPS 
               Assembler and simulate how those instructions would 
               execute in a basic MIPS-compatible processor model.

  Dependencies:
    - <string>, <cstdint>, <vector>, <string>, <array> 
  -----------------------------------------------------------------------------*/
#ifndef TINY_MIPS_CPU_H
#define TINY_MIPS_CPU_H

#include <cstdint>
#include <vector>  
#include <array>


class TinyMipsCPU { 
public:
    TinyMipsCPU();
  
    // Load binary instructions (as 32-bit unsigned integers) 
    void loadProgram(const std::vector<uint32_t>& instructions); 

    // Run the program until completion - jumps to invalid PC or runs out of code
    void executeProgram(); 

    // Execute one instruction and update PC
    bool performStep(); 

private:
    // Register range from 0-31
    std::array<uint32_t, 32> registers; 
    // Memory representation where insturctions are loaded
    std::vector<uint32_t> instructionMemory;  
    // Simplified flat memory
    std::vector<uint8_t> memory;
    // Program counter           
    uint32_t pc;  
    
    // Instruction decoding helpers accesses
    uint32_t getOpcode(uint32_t instruction) const; 

    // Instruction implementations
    void runStyleRType(uint32_t instruction); 
    void runStyleIType(uint32_t instruction);
    void runStyleJType(uint32_t instruction); 



};
   
#endif // TINY_MIPS_CPU_H 