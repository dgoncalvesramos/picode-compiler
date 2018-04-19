
.org 0
  inw a
  inw b
  addw c,a,b,$1
  outw c
  stop
a: .word 0
b: .word 0
c: .word 0

