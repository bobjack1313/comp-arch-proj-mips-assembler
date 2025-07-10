# test_lt_comp.s
# less than test

# Set on less than test
    addi $t0, $zero, 4 
    addi $t1, $zero, 10
    slt  $t2, $t0, $t1      # $t2 = 1
    slt  $t3, $t1, $t0      # $t3 = 0
