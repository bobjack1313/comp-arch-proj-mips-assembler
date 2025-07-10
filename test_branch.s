# test_branch.s
# Test branch instructions

loop:
    addi $t0, $t0, -1       # decrement $t0
    beq  $t0, $zero, end    # if $t0 == 0, jump to end
    j loop                  # else jump to loop
end: 
    addi $t1, $zero, 1
