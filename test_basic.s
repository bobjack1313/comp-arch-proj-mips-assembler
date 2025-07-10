# test_basic.s
# Basic arithmetic test


main:
    addi $t0, $zero, 7      # $t0 = 7
    addi $t1, $zero, 3      # $t1 = 3
    add  $t2, $t0, $t1      # $t2 = $t0 + $t1 = 10
    sub  $t3, $t0, $t1      # $t3 = $t0 - $t1 = 4
