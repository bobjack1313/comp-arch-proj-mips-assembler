

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

private:
    // Register range from 0-31
    std::array<uint32_t, 32> registers; 
    // Memory representation where insturctions are loaded
    std::vector<uint32_t> instructionMemory;  
    // Simplified flat memory
    std::vector<uint8_t> memory;
    // Program counter           
    uint32_t pc;     

};
   
#endif // End TINY_MIPS_CPU_H 