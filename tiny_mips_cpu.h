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
    - <cstdint>, <vector>, <array>, <string>
  -----------------------------------------------------------------------------*/
#ifndef TINY_MIPS_CPU_H
#define TINY_MIPS_CPU_H

#include <cstdint>
#include <vector>  
#include <array>
#include <string>
#include <unordered_set>



class TinyMipsCPU {
public:
    TinyMipsCPU();
    // Load binary instructions (as 32-bit unsigned integers) 
    void loadProgram(const std::vector<uint32_t>& instructions); 
    // Run the program until completion - jumps to invalid PC or runs out of code
    void executeProgram(); 
    // Execute one instruction and update PC
    bool performStep(); 
    // Print the current register state 
    void displayRegisters() const; 
    void displayRegisters1(const std::unordered_set<int>& changedRegs) const;
    // Print a memory snapshot (debug)
    void displayMemory(uint32_t start, uint32_t end) const;


private:
    // Program counter           
    uint32_t pc;  
    // Register range from 0-31
    std::array<uint32_t, 32> registers; 
    // Simplified flat memory
    std::vector<uint8_t> memory;
    // Memory representation where insturctions are loaded
    std::vector<uint32_t> instructionMemory;
    
    // Instruction decoding helpers accesses
    uint32_t getOpcode(uint32_t instruction) const; 
    uint32_t getRs(uint32_t instruction) const;
    uint32_t getRt(uint32_t instruction) const; 
    uint32_t getRd(uint32_t instruction) const;
    uint32_t getFunct(uint32_t instruction) const;
    int16_t  getImmediate(uint32_t instruction) const;
    uint32_t getShamt(uint32_t instruction) const;
    uint32_t getAddress(uint32_t instruction) const; 

    // Instruction implementations
    void runStyleRType(uint32_t instruction); 
    void runStyleIType(uint32_t instruction, uint32_t opcode);
    void runStyleJType(uint32_t instruction); 

    // Memory helpers
    uint32_t loadWord(uint32_t address) const; 
    void storeWord(uint32_t address, uint32_t value);

    // Utility
    std::string registerName(uint32_t reg) const;
    std::string getNamedRegister(uint32_t reg) const;
};
   
extern bool DEBUG_MODE;

#endif // TINY_MIPS_CPU_H 