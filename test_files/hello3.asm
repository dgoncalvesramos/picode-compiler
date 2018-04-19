  movl %1,str_pt
loop:
  movb %0,[%1]++
  ifb  %0 == $0 then fin 
  outb %0 @loop
str_pt: .long   2+data+2*4
fin:
    stop
data:    .long 0
         .long 0
         .word 0
data3:   .string "hello world (3)"
