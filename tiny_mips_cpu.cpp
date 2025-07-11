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
#include <iomanip>

using namespace std;

// Default flag for debugging
bool DEBUG_MODE = false;

/*  
 *  Class init - Set registers, PC to 0 and 
 *  Init 1024 x 4 bytes = 4096 bytes = 4KB
*/
TinyMipsCPU::TinyMipsCPU() 
    : pc(0), registers{}, memory(1024, 0) { }

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
        runStyleIType(current_instruction,  opcode);
    }
    // Increment pc + 4
    pc += 4;
    return true;
}

void TinyMipsCPU::displayRegisters() const {
    for (int i = 0; i < 32; ++i) {
        cout << "R" << setw(2) << setfill('0') << i << ": " << setw(10) << registers[i];

        if (i % 4 == 3) 
            cout << '\n';
        else 
            cout << '\t';
    }
}

void TinyMipsCPU::displayMemory(uint32_t start, uint32_t end) const {
    for (uint32_t addr = start; addr <= end; addr += 4) {
        cout << "M[" << addr << "] = " << loadWord(addr) << '\n';
    }
}

/*
| 31-26 | 25-21 | 20-16 | 15-11 | 10-6  | 5-0 |
|opcode |  rs   |  rt   |  rd   | shamt |funct|
*/
void TinyMipsCPU::runStyleRType(uint32_t instruction) {
    uint32_t rs = getRs(instruction);
    uint32_t rt = getRt(instruction);
    uint32_t rd = getRd(instruction);
    uint32_t shamt = getShamt(instruction);
    uint32_t funct = getFunct(instruction);

    if (DEBUG_MODE) {
        cout << "**> Starting RType instruction " << endl;
        displayBits(rs, 5);
        displayBits(rt, 5);
        displayBits(rd, 5);
        displayBits(shamt, 5);
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

/*
| 31-26 | 25-21 | 20-16 | 15-0 |
|opcode |  rs   |  rt   |  imm | 
*/
void TinyMipsCPU::runStyleIType(uint32_t instruction, uint32_t opcode) {
    uint32_t rs = getRs(instruction);
    uint32_t rt = getRt(instruction);
    int16_t imm = getImmediate(instruction);

    if (DEBUG_MODE) {
        cout << "**> Starting IType instruction " << endl;
        displayBits(rs, 5);
        displayBits(rt, 5);
        displayBits(imm, 16);
    }

    switch (opcode) {
        // Beq - Function Code 4
        case 0x4:
            if (registers[rs] == registers[rt]) {
                pc += (imm << 2) + 4;
                return;
            }
            break;
        // Load Word - Function Code 35
        case 0x23:
            registers[rt] = loadWord(registers[rs] + imm);
            break;
        // Store Word - Function Code 43       
        case 0x2B:
            storeWord(registers[rs] + imm, registers[rt]);
            break;

        default:
            cerr << "Unknown I-type opcode: " << opcode << endl;
            break;
    }
}

/*
| 31-26 | 25-0  |
|opcode |  addr | 
*/
void TinyMipsCPU::runStyleJType(uint32_t instruction) { 
    uint32_t addr = getAddress(instruction);
    uint32_t addrShift = (addr << 2);
    uint32_t upperFour = pc & 0xF0000000;
    uint32_t fullJumpAddress = upperFour | addrShift;

    if (DEBUG_MODE) {
        cout << "**> Starting JType instruction " << endl;
        cout << "- Jump Address Check - " << addr << endl;
        cout << "upperFour: "<< upperFour << " " << "AddShft: " <<  addrShift << endl;
        cout << "Full Address: " << fullJumpAddress;
    }
    pc = fullJumpAddress;
}

uint32_t TinyMipsCPU::loadWord(uint32_t addr) const {
    // Check Mem Bounds
    if (addr + 3 >= memory.size()) 
        return 0;

    uint32_t msb = memory[addr] << 24;
    uint32_t nsb1 = memory[addr + 1] << 16;
    uint32_t nsb2 = memory[addr + 2] << 8;
    uint32_t lsb = memory[addr + 3];

    if (DEBUG_MODE) {
        cout << "- Load Word Bits - ";
        cout << msb << " " << nsb1 <<  " " << nsb2 <<  " " << lsb << endl;
    }
    return msb | nsb1 | nsb2 | lsb;
}

void TinyMipsCPU::storeWord(uint32_t addr, uint32_t val) {
    // Check Mem Bounds
    if (addr + 3 >= memory.size()) 
        return;

    if (DEBUG_MODE) {
        cout << "- Store Word Bits - ";
        cout << ((val >> 24) & 0xFF) << " ";
        cout << ((val >> 16) & 0xFF) <<  " ";
        cout << ((val >> 8) & 0xFF) << " ";
        cout << (val & 0xFF) << endl;
    }

    memory[addr] = (val >> 24) & 0xFF;
    memory[addr + 1] = (val >> 16) & 0xFF;
    memory[addr + 2] = (val >> 8) & 0xFF;
    memory[addr + 3] = val & 0xFF; 
}

// Function to display the register
string TinyMipsCPU::registerName(uint32_t reg) const {
    return "$" + to_string(reg);
}


// Helper function to extract the bits from instruction
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
// Immediates are signed values... watch the type
int16_t TinyMipsCPU::getImmediate(uint32_t instruction) const {
    return static_cast<int16_t>(extractBits(instruction, 0, 16));
}

uint32_t TinyMipsCPU::getShamt(uint32_t instruction) const {
    return extractBits(instruction, 6, 5);
}

uint32_t TinyMipsCPU::getAddress(uint32_t instruction) const {
    return extractBits(instruction, 0, 26);
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
