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
#include <unordered_map>

using namespace std;

// Default flag for debugging
bool DEBUG_MODE = false;

/*  
 *  Class init - Set registers, PC to 0 and 
 *  Init 1024 x 4 bytes = 4096 bytes = 4KB
*/
TinyMipsCPU::TinyMipsCPU() 
    : pc(0), registers{}, memory(1024, 0) { }

// Use to catch infinite loops from bad test code
int maxSteps = 0;
int steps = 0;

// Display func declaration
void displayBits(uint32_t value, int bits);

// Need a function to load the instructions into the cpu class
void TinyMipsCPU::loadProgram(const vector<uint32_t>& instructions) {
    instructionMemory = instructions;
    maxSteps = instructionMemory.size();
    pc = 0;
}

// Will cycle through each instruction step until completion
void TinyMipsCPU::executeProgram() {
    // Heartbeat loop for each step
    while(performStep()) {
        if (++steps > maxSteps) {
            cerr << "[ERROR] Max instruction count exceeded. Possible infinite loop." << endl;
            break;
        }
    }
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

    // Initial iteration output display
    cout << "\n=== Executing Instruction ===\n";
    cout << "Binary: " << bitset<32>(current_instruction) << "\n";
    cout << "Opcode: " << opcode << "\n";

    // Separate 0 for R-Type | 2, 3 for J-Type | Remaining are I-Type
    if (opcode == 0) {
        cout << "R-Type" << " Instruction\n\n";
        runStyleRType(current_instruction); 

    } else if (opcode == 2 || opcode == 3) {
        cout << "J-Type" << " Instruction\n\n";
        runStyleJType(current_instruction); 

    } else {
        cout << "I-Type" << " Instruction\n\n";
        runStyleIType(current_instruction, opcode);
    }

    // Show post-state summary
    displayRegisters();
    cout << endl;
    displayMemory(0, 64); 

    // Increment pc + 4
    pc += 4;
    return true;
}

void TinyMipsCPU::displayRegisters1(const std::unordered_set<int>& changedRegs) const {
    for (int i : changedRegs) {
        cout << registerName(i) << ": 0x" << hex << setw(8) << setfill('0') << registers[i] << endl;
    }
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

// Displays memory that has contents
void TinyMipsCPU::displayMemory(uint32_t start, uint32_t end) const {
    cout << "\nMemory Contents (" << start << " to " << end << "):" << endl;

    bool any = false;
    for (uint32_t addr = start; addr <= end; addr += 4) {
        uint32_t val = loadWord(addr);
        if (val != 0) {
            cout << "M[" << setw(3) << addr << "] = " << hex << "0x" << val << dec << " (" << val << ")" << endl;
            any = true;
        }
    }

    if (!any)
        cout << "[No non-zero memory in this range]" << endl;
}

// Debugging Version - Shows zero values
// void TinyMipsCPU::displayMemory(uint32_t start, uint32_t end) const {
//     cout << "Memory Contents (" << start << " to " << end << "):\n";
//     for (uint32_t addr = start; addr <= end; addr += 4) {
//         uint32_t word = loadWord(addr);
//         cout << "M[" << addr << "] = 0x" << hex << setw(8) << setfill('0') << word << dec << "\n";
//     }
// }

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
        cout << "**> Starting R-Type instruction " << endl;
        displayBits(rs, 5);
        displayBits(rt, 5);
        displayBits(rd, 5);
        displayBits(shamt, 5);
        displayBits(funct, 6);
    }

    switch (funct) {
        // Add - Function Code 32
        case 0x20: registers[rd] = registers[rs] + registers[rt];
            cout << "Instruction: add " << getNamedRegister(rd)
                << ", " << getNamedRegister(rs) << ", " << getNamedRegister(rt) << '\n';
            cout << "  Values: " << registerName(rs) << " = " << registers[rs]
                << ", " << registerName(rt) << " = " << registers[rt] << '\n';
            cout << "  Result: " << registerName(rd) << " = "
                << registers[rs] << " + " << registers[rt]
                << " = " << registers[rd] << '\n';
            break;

        // Sub - Function Code 34    
        case 0x22: registers[rd] = registers[rs] - registers[rt];
            cout << "Instruction: sub " << getNamedRegister(rd)
                << ", " << getNamedRegister(rs) << ", " << getNamedRegister(rt) << '\n';
            cout << "  Values: " << registerName(rs) << " = " << registers[rs]
                << ", " << registerName(rt) << " = " << registers[rt] << '\n';
            cout << "  Result: " << registerName(rd) << " = "
                << registers[rs] << " - " << registers[rt]
                << " = " << registers[rd] << '\n';
            break;

        // And - Function Code 36
        case 0x24: registers[rd] = registers[rs] & registers[rt];
            cout << "Instruction: and " << getNamedRegister(rd) << ", " 
                << getNamedRegister(rs) << ", " << getNamedRegister(rt) << endl;
            cout << "  Values: " << getNamedRegister(rs) << " = " << registers[rs] << ", "
                << getNamedRegister(rt) << " = " << registers[rt] << endl;
            cout << "  Result: " << getNamedRegister(rd) << " = " 
                << registers[rs] << " & " << registers[rt] << " = " << registers[rd] << endl;
            break;

        // Or - Function Code 37
        case 0x25: registers[rd] = registers[rs] | registers[rt];
            cout << "Instruction: or " << getNamedRegister(rd) << ", " 
                << getNamedRegister(rs) << ", " << getNamedRegister(rt) << endl;
            cout << "  Values: " << getNamedRegister(rs) << " = " << registers[rs] << ", "
                << getNamedRegister(rt) << " = " << registers[rt] << endl;  
            cout << "  Result: " << getNamedRegister(rd) << " = " 
                << registers[rs] << " | " << registers[rt] << " = " << registers[rd] << endl;
            break;

        // Nor - Function Code 39
        case 0x27: registers[rd] = ~(registers[rs] | registers[rt]);
            cout << "Instruction: nor " << getNamedRegister(rd) << ", "
                << getNamedRegister(rs) << ", " << getNamedRegister(rt) << endl;
            cout << "  Values: " << getNamedRegister(rs) << " = " << registers[rs] << ", "
                << getNamedRegister(rt) << " = " << registers[rt] << endl; 
            cout << "  Result: " << getNamedRegister(rd) << " = ~("
                << registers[rs] << " | " << registers[rt] << ") = " << registers[rd] << endl; 
            break; 

        // Slt - Function Code 42 
        case 0x2A: registers[rd] = (int32_t)registers[rs] < (int32_t)registers[rt];
            cout << "Instruction: slt " << getNamedRegister(rd) << ", "
                << getNamedRegister(rs) << ", " << getNamedRegister(rt) << endl;
            cout << "  Values: " << getNamedRegister(rs) << " = " << static_cast<int32_t>(registers[rs]) << ", "
                << getNamedRegister(rt) << " = " << static_cast<int32_t>(registers[rt]) << endl;
            cout << "  Result: " << getNamedRegister(rd) << " = ("
                << static_cast<int32_t>(registers[rs]) << " < " << static_cast<int32_t>(registers[rt]) << ") → "
                << registers[rd] << endl;
            break;

        default:
            cerr << "Unknown R-type funct: " << funct << "\n";
            break;
    }

    cout << "\nModified Registers\n";
    cout << registerName(rs) << " = " << registers[rs] << endl;
    cout << registerName(rt) << " = " << registers[rt] << endl;
    cout << registerName(rd) << " = " << registers[rd] << endl << endl;
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
        cout << "**> Starting I-Type instruction " << endl;
        displayBits(rs, 5);
        displayBits(rt, 5);
        displayBits(imm, 16);
    }

    switch (opcode) {
        // Beq - Function Code 4
        case 0x4:
            cout << "Instruction: beq " << getNamedRegister(rs) << ", " << getNamedRegister(rt)
                << ", offset = " << static_cast<int16_t>(imm) << endl;
            cout << "  Values: " << getNamedRegister(rs) << " = " << registers[rs]
                << ", " << getNamedRegister(rt) << " = " << registers[rt] << endl;

            if (registers[rs] == registers[rt]) {
                uint32_t targetPC = pc + 4 + (static_cast<int16_t>(imm) << 2);
                cout << "  Branch Taken: PC set to " << targetPC << " (0x" << hex << targetPC << dec << ")" << endl;
                pc = targetPC;
                return;
            } else {
                cout << "  Branch Not Taken" << endl;
            }
            break;

        // Addi - Function Code 8
        case 0x8: { 
            cout << "Instruction: addi " << getNamedRegister(rt) << ", "
                << getNamedRegister(rs) << ", " << static_cast<int16_t>(imm) << endl;
            cout << "  Values: " << getNamedRegister(rs) << " = " << registers[rs] << endl;

            int32_t result = static_cast<int32_t>(registers[rs]) + static_cast<int16_t>(imm);
            registers[rt] = result;
            cout << "  Result: " << getNamedRegister(rt) << " = "
                << static_cast<int32_t>(registers[rs]) << " + " << static_cast<int16_t>(imm)
                << " = " << result << endl;
            break;
        }

        // Load Word - Function Code 35
        case 0x23: {

            uint32_t addr = registers[rs] + static_cast<int16_t>(imm);
            uint32_t value = loadWord(addr);
            registers[rt] = value;

            cout << "Instruction: lw " << getNamedRegister(rt) << ", "
                << static_cast<int16_t>(imm) << "(" << getNamedRegister(rs) << ")" << endl;
            cout << "  Effective address: " << addr << endl;
            cout << "  Loaded value: " << value << " -> " << getNamedRegister(rt) << endl;
            break;
        }

        // Store Word - Function Code 43       
        case 0x2B: {
            int32_t address = static_cast<int32_t>(registers[rs]) + static_cast<int16_t>(imm);
            storeWord(address, registers[rt]);

            cout << "Instruction: sw " << getNamedRegister(rt) << ", " << static_cast<int16_t>(imm)
                << "(" << getNamedRegister(rs) << ")\n";
            cout << "  Effective address: " << address << "\n";
            cout << "  Stored " << getNamedRegister(rt) << " (value: " << registers[rt]
                << ") into M[" << address << "]\n";

            displayMemory(address, address + 4);
            cout << endl;
            break;
        }

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
        cout << "**> Starting J-Type instruction\n";
        cout << "Raw address: 0x" << hex << addr << "\n";
        cout << "Shifted:     0x" << addrShift << "\n";
        cout << "PC Upper:    0x" << upperFour << "\n";
        cout << "Full Jump:   0x" << fullJumpAddress << "\n";
    }

    cout << "Instruction: j 0x" << hex << fullJumpAddress << dec << endl;
    cout << "  Jumping to address: " << fullJumpAddress << endl;

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

// Will map the register value to the assembly name
string TinyMipsCPU::getNamedRegister(uint32_t reg) const {
    static const unordered_map<uint32_t, string> regMap = {
        {0, "$zero"}, {1, "$at"},   {2, "$v0"},  {3, "$v1"},
        {4, "$a0"},   {5, "$a1"},   {6, "$a2"},  {7, "$a3"},
        {8, "$t0"},   {9, "$t1"},   {10, "$t2"}, {11, "$t3"}, 
        {12, "$t4"},  {13, "$t5"},  {14, "$t6"}, {15, "$t7"},
        {16, "$s0"},  {17, "$s1"},  {18, "$s2"}, {19, "$s3"},  
        {20, "$s4"},  {21, "$s5"},  {22, "$s6"}, {23, "$s7"},
        {24, "$t8"},  {25, "$t9"},  {26, "$k0"}, {27, "$k1"}, 
        {28, "$gp"},  {29, "$sp"},  {30, "$fp"}, {31, "$ra"} 
    };  

    auto it = regMap.find(reg); 
    if (it != regMap.end()) {
        return it->second; 
    } else {
        // Fallback 
        return "$r" + to_string(reg);  
    }
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
