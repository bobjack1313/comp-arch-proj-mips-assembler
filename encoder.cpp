/*------------------------------------------------------------------------------
  File:        encoder.cpp
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Implements functions to encode parsed MIPS instructions into
               32-bit binary machine code strings.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies:
    - encoder.h
    - helpers.h
    - <sstream>, <unordered_map>
  -----------------------------------------------------------------------------*/

#include "encoder.h" 
#include "helpers.h"

#include <sstream> 
#include <unordered_map>

using namespace std;

// R-type mapper of operations to their funct codes
static unordered_map<string, uint32_t> functMap = {
    {"add", 0x20},
    {"sub", 0x22}, 
    {"and", 0x24},
    {"or",  0x25}, 
    {"slt", 0x2A}
};

// Opcode mapper for instructions
static unordered_map<string, uint32_t> opcodeMap = {
    {"lw",   0x23}, 
    {"sw",   0x2B},
    {"beq",  0x04}, 
    {"addi", 0x08},
    {"j",    0x02}
};

/**
 * Encodes an R-type instruction: opcode | rs | rt | rd | shamt | funct
 */
uint32_t encode_R(uint32_t funct, uint32_t rs, uint32_t rt, uint32_t rd) { 
    return (0 << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (0 << 6) | funct;
}

/**
 * Encodes an I-type instruction: opcode | rs | rt | immediate
 */
uint32_t encode_I(uint32_t opcode, uint32_t rs, uint32_t rt, int16_t imm) {
    return (opcode << 26) | (rs << 21) | (rt << 16) | (static_cast<uint16_t>(imm) & 0xFFFF); 
}

/**
 * Encodes a J-type instruction: opcode | address
 */
uint32_t encode_J(uint32_t opcode, uint32_t address) {
    return (opcode << 26) | (address & 0x03FFFFFF);
}

/**
 * Assembles parsed tokens into 32-bit binary strings using the appropriate encoding function.
 */
vector<string> assemble(const vector<Token>& tokens, 
                          const unordered_map<string, uint32_t>& symbolTable) {
    vector<string> binaryInstructions;
    uint32_t pc = 0;

    for (const Token& token : tokens) {
        const string& op = token.op; 
        const vector<string>& args = token.args;
        uint32_t encoded = 0; 

        if (functMap.count(op)) {
          
            // R-type: add rd, rs, rt
            if (args.size() != 3) throw runtime_error("Invalid R-type instruction format");
            uint32_t rd = reg_number(args[0]);
            uint32_t rs = reg_number(args[1]); 
            uint32_t rt = reg_number(args[2]);
            encoded = encode_R(functMap[op], rs, rt, rd);
        }
        else if (opcodeMap.count(op)) {
            uint32_t opcode = opcodeMap[op];

            if (op == "lw" || op == "sw") {
              
                // Format: lw rt, offset(rs)
                if (args.size() != 2) throw std::runtime_error("Invalid format for lw/sw");
                uint32_t rt = reg_number(args[0]); 
                size_t lparen = args[1].find('(');  
                size_t rparen = args[1].find(')');
                if (lparen == std::string::npos || rparen == std::string::npos)
                    throw std::runtime_error("Invalid memory access format"); 

                int16_t offset = std::stoi(args[1].substr(0, lparen)); 
                uint32_t rs = reg_number(args[1].substr(lparen + 1, rparen - lparen - 1));
                encoded = encode_I(opcode, rs, rt, offset); 
            }
            else if (op == "beq") {
              
                // Format: beq rs, rt, label
                if (args.size() != 3) throw std::runtime_error("Invalid beq format");
                uint32_t rs = reg_number(args[0]); 
                uint32_t rt = reg_number(args[1]); 
                const std::string& label = args[2];

                if (!symbolTable.count(label)) throw std::runtime_error("Undefined label: " + label);
                int offset = (symbolTable.at(label) - (pc + 4)) / 4;
                encoded = encode_I(opcode, rs, rt, static_cast<int16_t>(offset)); 
            }
            else if (op == "addi") {
              
                // Format: addi rt, rs, imm
                if (args.size() != 3) throw std::runtime_error("Invalid addi format");
                uint32_t rt = reg_number(args[0]);
                uint32_t rs = reg_number(args[1]); 
                int16_t imm = static_cast<int16_t>(std::stoi(args[2]));
                encoded = encode_I(opcode, rs, rt, imm);
            }
            else if (op == "j") {
              
                // Format: j label
                if (args.size() != 1) throw std::runtime_error("Invalid j format");
                const std::string& label = args[0]; 
                if (!symbolTable.count(label)) throw std::runtime_error("Undefined label: " + label);
                uint32_t addr = symbolTable.at(label) >> 2;
                encoded = encode_J(opcode, addr);
            }
        } else {
            throw std::runtime_error("Unknown operation: " + op);
        }

        // Convert encoded instruction to binary string and add to output
        binaryInstructions.push_back(to_binary32(encoded));
        pc += 4; 
    }

    return binaryInstructions; 
} 
