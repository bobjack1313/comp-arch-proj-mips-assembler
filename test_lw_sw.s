addi $t0, $zero, 20    # address
addi $t1, $zero, 1234  # value
sw   $t1, 0($t0)       # store 1234 at memory[20]
lw   $t2, 0($t0)       # load memory[20] into $t2