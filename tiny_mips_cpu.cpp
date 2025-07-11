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

// Default flag for debugging
bool DEBUG_MODE = false;

/*  
 *  Class init - Set registers, PC to 0 and 
 *  Init 1024 x 4 bytes = 4096 bytes = 4KB
*/
TinyMipsCPU::TinyMipsCPU() 
    : pc(0), registers{}, memory(1024, 0) {
        // Debug
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
    if (DEBUG_MODE) {
        cout << "----- Instruction Iteration ------ " << endl;
    }
    // Reject badly formed instructions - not div by 4
    if (pc >= instructionMemory.size() * 4)
        return false;

    // Get the current instruction from pc
    uint32_t current_instruction = instructionMemory[pc / 4];
    // Extract opcode from first 6 bit
    uint32_t opcode = getOpcode(current_instruction);

    if (DEBUG_MODE) {
        cout << "Getting Opcode " << endl;
        displayBits(opcode, 6);
    }

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

void TinyMipsCPU::runStyleRType(uint32_t instruction) {
    uint32_t rs = getRs(instruction);
    uint32_t rt = getRt(instruction);
    uint32_t rd = getRd(instruction);
    uint32_t funct = getFunct(instruction);

    if (DEBUG_MODE) {
        cout << "**> Starting RType instruction " << endl;
        displayBits(rs, 5);
        displayBits(rt, 5);
        displayBits(rd, 5);
        displayBits(funct, 6);
    }

    switch (funct) {
        // Add - Function Code 32
        case 0x20: registers[rd] = registers[rs] + registers[rt];
            break;
        // Sub - Function Code 34    
        case 0x22: registers[rd] = registers[rs] - registers[rt];
            break;
        // And - Function Code 36
        case 0x24: registers[rd] = registers[rs] & registers[rt];
            break;
        // Or - Function Code 37
        case 0x25: registers[rd] = registers[rs] | registers[rt]; 
            break;
        // Nor - Function Code 39
        case 0x27: registers[rd] = ~(registers[rs] | registers[rt]);
            break; 
        // Slt - Function Code 42 
        case 0x2A: registers[rd] = (int32_t)registers[rs] < (int32_t)registers[rt];
            break;

        default:
            cerr << "Unknown R-type funct: " << funct << "\n";
            break;
    }
}

// Dummy implementations for now:
void TinyMipsCPU::runStyleIType(uint32_t instruction) {
    cout << "**> I-type stub\n";
} 

void TinyMipsCPU::runStyleJType(uint32_t instruction) { 
    cout << "**> J-type stub\n";
} 

// Function to display the register




/*
Helper function to extract the bits from instruction
| 31-26 | 25-21 | 20-16 | 15-11 | 10-6  | 5-0 |
|opcode |  rs   |  rt   |  rd   | shamt |funct|
*/
uint32_t extractBits(uint32_t value, int start, int length) {
    uint32_t bitShifted = value >> start;
    // Using mask flips all the bits and isolates what we need
    uint32_t mask  = ((1 << length) - 1);
    return bitShifted & mask;
}

uint32_t TinyMipsCPU::getOpcode(uint32_t instruction) const {
    return extractBits(instruction, 26, 6);
}

uint32_t TinyMipsCPU::getRs(uint32_t instruction) const {
    return extractBits(instruction, 21, 5);
}

uint32_t TinyMipsCPU::getRt(uint32_t instruction) const {
    return extractBits(instruction, 16, 5);
}

uint32_t TinyMipsCPU::getRd(uint32_t instruction) const {
    return extractBits(instruction, 11, 5);
}

uint32_t TinyMipsCPU::getFunct(uint32_t instruction) const {
    return extractBits(instruction, 0, 6);
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
