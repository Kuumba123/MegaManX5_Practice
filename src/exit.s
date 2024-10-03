.set noreorder
    li $v0,1
    nop
    j 0x80033494
    sb $v0,0x23($s0)