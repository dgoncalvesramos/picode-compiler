.org 0
    jmp main

.org 0x1000
main:
    jmp main2

.org 0x7000
#.org 0x7FFF
main2:
    jmp main3

.org 0x7FFA
main3:
    addl %1, $0, $1, $2
