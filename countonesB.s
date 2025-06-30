.syntax unified
.thumb
.global count_ones_B
.type count_ones_B, %function

count_ones_B:
    movs    r2, #0          // count = 0
    movs    r3, #0          // i = 0
1:
    mov     r1, r0
    lsrs    r1, r1, r3
    ands    r1, r1, #1
    adds    r2, r2, r1
    adds    r3, r3, #1
    cmp     r3, #32
    blt     1b
    mov     r0, r2
    bx      lr 