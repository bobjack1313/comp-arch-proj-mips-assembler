# Tiny MIPS Assembler

## Overview

This project implements a **Tiny MIPS Assembler** in C++ for CSCE 5610. It reads human-readable MIPS assembly code (`.s` files), parses instructions and labels, and outputs 32-bit binary machine code suitable for simulation on the CELL MIPS simulator or similar tools.

This assembler supports sets of MIPS instructions (R-type, I-type, J-type), and uses two separate passes to parse and resolve symbols from a table. The output file will contain binary string representations of the intructions with one per line.

---

## Authors

- Souban Ahmed  
- Bhargav Alimili  
- Bob Jack  
- Janaki Ramaiah Venigalla  

**Course:** CSCE 5610 – Computer Architecture  
**Group:** Group 2  
**Instructor:** Dr. Beilei Jiang

---

## Features

- Parses MIPS `.s` files line by line
- Ignores whitespace and comments 
- Resolves labels using a symbol table  
- Encodes supported instructions into correct 32-bit binary    
- Outputs binary to a `.txt` file   
- Modular C++ design with clean headers and documentation

---

## Instruction Sets

### R-type:
- `add`
- `sub`
- `and`
- `or`
- `slt`

### I-type:
- `lw`
- `sw`
- `beq`
- `addi`

### J-type:
- `j`

---

## Build Instructions

If you have **g++** and **make** installed. From the root project directory:

```bash
make
```
To manually compile use the following:
```
g++ -std=c++17 -Wall -Wextra -pedantic tiny-mips-asm.cpp parser.cpp encoder.cpp converters.cpp -o tiny-mips-asm
```
---

## Program Operation Instructions

Once compiled, run the assembler using:

```
./tiny-mips-asm input.s output.txt
```
- `input.s`: MIPS assembly source file
- `output.txt`: Destination file for 32-bit binary output
