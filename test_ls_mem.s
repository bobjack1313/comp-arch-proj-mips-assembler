# test_ls_mem.s
# Load and store test

    addi $t0, $zero, 16     # $t0 = 16
    addi $t1, $zero, 100    # $t1 = 100 (memory base)
    sw   $t0, 0($t1)        # store $t0 at address $t1
    lw   $t2, 0($t1)        # load into $t2 from address $t1
