/*------------------------------------------------------------------------------
  File:        simulate_cpu.h
  Project:     Tiny MIPS Assembler - CSCE 5610 Group Project (Bonus)
  Purpose:     Declaration for the CPU simulator's entry point

  Authors:     Souban Ahmed, Bhargav Alimili, Bob Jack, Janaki Ramaiah Venigalla
  Group:       CSCE5610 - Group 2
  Instructor:  Dr. Beilei Jiang
  Date:        July 2025

  Description:
    This file declares the entry point for running the TinyMipsCPU
    simulator. It reads binary MIPS instructions (from output.txt),
    converts them into uint32_t, loads them into the CPU, and executes.
------------------------------------------------------------------------------*/
#ifndef SIMULATE_SINGLE_CPU_H
#define SIMULATE_SINGLE_CPU_H

int simulate_single_cpu_main(int argc, char* argv[]);

#endif // SIMULATE_SINGLE_CPU_H