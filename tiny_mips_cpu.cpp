/*------------------------------------------------------------------------------
  File:        tiny_mips_cpu.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project (Bonus)
  Purpose:     Implements a basic single-cycle MIPS CPU simulator

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025
------------------------------------------------------------------------------*/

#include "tiny_mips_cpu.h"
#include <iostream>
#include <bitset>

using namespace std;


TinyMipsCPU::TinyMipsCPU() {
  cerr << "TinyMipsCPU is active\n";
}
// Display func declaration
void displayBits(uint32_t value, int bits);

// Need a function to load the instructions into the cpu class
void TinyMipsCPU::loadProgram(const vector<uint32_t>& instructions) {
    instructionMemory = instructions;
    pc = 0;
}

// Will cycle through each instruction step until completion
void TinyMipsCPU::executeProgram() {
    // Heartbeat loop for each step
    while(performStep());
}

// Works through the instruction | picks type | segments
bool TinyMipsCPU::performStep() {
    // Reject badly formed instructions - not div by 4
    if (pc >= instructionMemory.size() * 4) return false;

    // Get the current instruction from pc
    uint32_t current_instruction = instructionMemory[pc / 4];
    // Extract opcode from first 6 bit
    uint32_t opcode = getOpcode(current_instruction);

    // Debugging
    displayBits(opcode, 6);

    // Separate 0 for RType | 2, 3 for J Type | Remaining are IType
    if (opcode == 0) {
        runStyleRType(current_instruction); 

    } else if (opcode == 2 || opcode == 3) {
        runStyleJType(current_instruction); 

    } else {
        runStyleIType(current_instruction);
    }
    // Increment pc + 4
    pc += 4;
    return true;
}

// Dummy implementations for now:
void TinyMipsCPU::runStyleRType(uint32_t instr) {
    cout << "R-type stub\n";
}

void TinyMipsCPU::runStyleIType(uint32_t instr) {
    cout << "I-type stub\n";
} 

void TinyMipsCPU::runStyleJType(uint32_t instr) { 
    cout << "J-type stub\n";
} 

// Fucntion to display the register




// Helper function to extract the bits from instruction
uint32_t extractBits(uint32_t value, int start, int length) {
    uint32_t bitShifted = value >> start;
    uint32_t mask  = ((1 << length) - 1);
    // Debug
    displayBits(bitShifted, 6); 
    return bitShifted & mask;
}

uint32_t TinyMipsCPU::getOpcode(uint32_t instr) const {
    return extractBits(instr, 26, 6);
}

// Debugging visual bit display
void displayBits(uint32_t bits, int numBits) {
    // Mask to keep only the numBits lower bits
    uint32_t mask = (numBits >= 32) ? 0xFFFFFFFF : ((1u << numBits) - 1);
    bits &= mask;

    // Always use 32 and slice from right
    bitset<32> b(bits); 
    string output = b.to_string().substr(32 - numBits);

    cout << "Decimal: " << bits << "\n";
    cout << "Binary (" << numBits << " bits): " << output << "\n";
}
