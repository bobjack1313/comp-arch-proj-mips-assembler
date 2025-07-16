# test_lw_sw.s
# Testing the Use of LW and SW

addi $t0, $zero, 20      # base address (memory[20])
addi $t1, $zero, 1234    # value 1
sw   $t1, 0($t0)         # store 1234 at memory[20]

addi $t3, $zero, 40      # different address (memory[40])
addi $t4, $zero, 5678    # value 2
sw   $t4, 0($t3)         # store 5678 at memory[40]

lw   $t2, 0($t0)         # load from memory[20] into $t2 (should be 1234)
lw   $t5, 0($t3)         # load from memory[40] into $t5 (should be 5678)

addi $t6, $zero, 60      # another address (memory[60])
sw   $t2, 0($t6)         # store value from $t2 (1234) to memory[60]

lw   $t7, 0($t6)         # load back from memory[60] into $t7 (should be 1234)
