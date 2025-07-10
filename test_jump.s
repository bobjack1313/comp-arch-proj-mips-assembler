# test_jump.s
# Jump test

start:
    addi $t0, $zero, 5 
    j skip 
    addi $t0, $t0, 99       # should be skipped
skip:
    addi $t1, $zero, 1
