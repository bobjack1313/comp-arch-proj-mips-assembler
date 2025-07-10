/*------------------------------------------------------------------------------
  File:        tiny-mips-asm.h
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
#pragma once

int runAssembler(const std::string& inputFilePath, const std::string& outputFilePath);
