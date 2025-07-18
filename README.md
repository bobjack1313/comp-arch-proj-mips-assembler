# Tiny MIPS Assembler

## Overview

This project implements a **Tiny MIPS Assembler** in C++ for CSCE 5610. It reads human-readable MIPS assembly code (`.s` files), parses instructions and labels, and outputs 32-bit binary machine code suitable for simulation on the CELL MIPS simulator or similar tools.

This assembler supports sets of MIPS instructions (R-type, I-type, J-type), and uses two separate passes to parse and resolve symbols from a table. The output file will contain binary string representations of the intructions with one per line.

For the bonus segment of this project, there is a MIPS CPU simulator that loads the generated machine code, decodes it, and executes each instruction. The registers and memory are updates with each intstruction read. The results are displayed to show what occurs in the simulation.

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

## Bonus Section Features

- Implements a single-cycle MIPS CPU simulator in C++
- Loads and executes machine code generated by the assembler
- Simulates register operations, memory access, and program flow
- Supports all 10 bonus instructions: add, sub, and, or, slt, nor, lw, sw, beq, and j

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

To manually compile main project use the following:
```
g++ -std=c++17 -Wall -Wextra -pedantic tiny_mips_asm.cpp parser.cpp encoder.cpp converters.cpp -o tiny_mips_asm
```

To manually compile the bonus portion use:
```
g++ -std=c++17 -Wall -Wextra -pedantic simulate_single_cpu.cpp tiny_mips_cpu.cpp -o simulate_single_cpu
```
---

## Program Operation Instructions

Once compiled, run the assembler using:

```
./tiny_mips_asm input.s output.txt
```
- `input.s`: MIPS assembly source file
- `output.txt`: Destination file for 32-bit binary output

### Sample Assembler Input File

<pre><code>
Loop: addi $t0, $zero, 5 # Initialize $t0 = 5 
addi $t1, $zero, 3 # Initialize $t1 = 3 
add $t2, $t0, $t1 # $t2 = $t0 + $t1 = 8 
sw $t2, 0($zero) # Store $t2 into memory[0] 
lw $t3, 0($zero) # Load memory[0] into $t3 
sub $t4, $t2, $t1 # $t4 = $t2 - $t1 = 5
</code></pre>

### Sample Assembler Output Text

<pre><code>
00100000000010000000000000000101
00100000000010010000000000000011
00000001000010010101000000100000
10101100000010100000000000000000 
10001100000010110000000000000000 
00000001010010010110000000100010 
</code></pre>

## Bonus Program Operation Instructions

After the output file has been created. Load it into the single CPU simulator with:
```
./simulate_single_cpu output.txt
```
- `output.txt`: Text file containing binary representation of machine language to be simulated

### Sample Single CPU Simulator Input File

<pre><code>
00100000000010000000000000000111
00100000000010010000000000000011
00000001000010010101000000100000
00000001000010010101100000100010
</code></pre>

### Sample Single CPU Output File

<pre><code>
Initial Register State:
R00: 0000000000	R01: 0000000000	R02: 0000000000	R03: 0000000000
R04: 0000000000	R05: 0000000000	R06: 0000000000	R07: 0000000000
R08: 0000000000	R09: 0000000000	R10: 0000000000	R11: 0000000000
R12: 0000000000	R13: 0000000000	R14: 0000000000	R15: 0000000000
R16: 0000000000	R17: 0000000000	R18: 0000000000	R19: 0000000000
R20: 0000000000	R21: 0000000000	R22: 0000000000	R23: 0000000000
R24: 0000000000	R25: 0000000000	R26: 0000000000	R27: 0000000000
R28: 0000000000	R29: 0000000000	R30: 0000000000	R31: 0000000000

Initial Memory State:

Memory Contents (0 to 64):
[No non-zero memory in this range]

=== Executing Instruction ===
Binary: 00100000000010000000000000000111
Opcode: 8
I-Type Instruction

Instruction: addi $t0, $zero, 7
  Values: $zero = 0
  Result: $t0 = 0 + 7 = 7
R00: 0000000000	R01: 0000000000	R02: 0000000000	R03: 0000000000
R04: 0000000000	R05: 0000000000	R06: 0000000000	R07: 0000000000
R08: 0000000007	R09: 0000000000	R10: 0000000000	R11: 0000000000
R12: 0000000000	R13: 0000000000	R14: 0000000000	R15: 0000000000
R16: 0000000000	R17: 0000000000	R18: 0000000000	R19: 0000000000
R20: 0000000000	R21: 0000000000	R22: 0000000000	R23: 0000000000
R24: 0000000000	R25: 0000000000	R26: 0000000000	R27: 0000000000
R28: 0000000000	R29: 0000000000	R30: 0000000000	R31: 0000000000


Memory Contents (0 to 64):
[No non-zero memory in this range]

=== Executing Instruction ===
Binary: 00100000000010010000000000000011
Opcode: 8
I-Type Instruction

Instruction: addi $t1, $zero, 3
  Values: $zero = 0
  Result: $t1 = 0 + 3 = 3
R00: 0000000000	R01: 0000000000	R02: 0000000000	R03: 0000000000
R04: 0000000000	R05: 0000000000	R06: 0000000000	R07: 0000000000
R08: 0000000007	R09: 0000000003	R10: 0000000000	R11: 0000000000
R12: 0000000000	R13: 0000000000	R14: 0000000000	R15: 0000000000
R16: 0000000000	R17: 0000000000	R18: 0000000000	R19: 0000000000
R20: 0000000000	R21: 0000000000	R22: 0000000000	R23: 0000000000
R24: 0000000000	R25: 0000000000	R26: 0000000000	R27: 0000000000
R28: 0000000000	R29: 0000000000	R30: 0000000000	R31: 0000000000


Memory Contents (0 to 64):
[No non-zero memory in this range]

=== Executing Instruction ===
Binary: 00000001000010010101000000100000
Opcode: 0
R-Type Instruction

Instruction: add $t2, $t0, $t1
  Values: $8 = 7, $9 = 3
  Result: $10 = 7 + 3 = 10

Modified Registers
$8 = 7
$9 = 3
$10 = 10

R00: 0000000000	R01: 0000000000	R02: 0000000000	R03: 0000000000
R04: 0000000000	R05: 0000000000	R06: 0000000000	R07: 0000000000
R08: 0000000007	R09: 0000000003	R10: 0000000010	R11: 0000000000
R12: 0000000000	R13: 0000000000	R14: 0000000000	R15: 0000000000
R16: 0000000000	R17: 0000000000	R18: 0000000000	R19: 0000000000
R20: 0000000000	R21: 0000000000	R22: 0000000000	R23: 0000000000
R24: 0000000000	R25: 0000000000	R26: 0000000000	R27: 0000000000
R28: 0000000000	R29: 0000000000	R30: 0000000000	R31: 0000000000


Memory Contents (0 to 64):
[No non-zero memory in this range]

=== Executing Instruction ===
Binary: 00000001000010010101100000100010
Opcode: 0
R-Type Instruction

Instruction: sub $t3, $t0, $t1
  Values: $8 = 7, $9 = 3
  Result: $11 = 7 - 3 = 4

Modified Registers
$8 = 7
$9 = 3
$11 = 4

R00: 0000000000	R01: 0000000000	R02: 0000000000	R03: 0000000000
R04: 0000000000	R05: 0000000000	R06: 0000000000	R07: 0000000000
R08: 0000000007	R09: 0000000003	R10: 0000000010	R11: 0000000004
R12: 0000000000	R13: 0000000000	R14: 0000000000	R15: 0000000000
R16: 0000000000	R17: 0000000000	R18: 0000000000	R19: 0000000000
R20: 0000000000	R21: 0000000000	R22: 0000000000	R23: 0000000000
R24: 0000000000	R25: 0000000000	R26: 0000000000	R27: 0000000000
R28: 0000000000	R29: 0000000000	R30: 0000000000	R31: 0000000000


Memory Contents (0 to 64):
[No non-zero memory in this range]

Final Register State:
R00: 0000000000	R01: 0000000000	R02: 0000000000	R03: 0000000000
R04: 0000000000	R05: 0000000000	R06: 0000000000	R07: 0000000000
R08: 0000000007	R09: 0000000003	R10: 0000000010	R11: 0000000004
R12: 0000000000	R13: 0000000000	R14: 0000000000	R15: 0000000000
R16: 0000000000	R17: 0000000000	R18: 0000000000	R19: 0000000000
R20: 0000000000	R21: 0000000000	R22: 0000000000	R23: 0000000000
R24: 0000000000	R25: 0000000000	R26: 0000000000	R27: 0000000000
R28: 0000000000	R29: 0000000000	R30: 0000000000	R31: 0000000000

Final Memory State:

Memory Contents (0 to 64):
[No non-zero memory in this range]

</code></pre>
