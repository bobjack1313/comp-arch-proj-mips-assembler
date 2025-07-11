/*------------------------------------------------------------------------------
  File:        tiny_mips_asm.h
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project
  Purpose:     Header file for the main driver of the assembler. Declares the
               runAssembler function, which handles the full assembly workflow.

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Dependencies:
    - <string>
  -----------------------------------------------------------------------------*/
#ifndef TINY_MIPS_ASM_H
#define TINY_MIPS_ASM_H

int runAssembler(const std::string& inputFilePath, const std::string& outputFilePath);


#endif // TINY_MIPS_ASM_H