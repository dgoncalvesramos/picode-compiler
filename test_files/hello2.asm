  movl %1,$str
loop:
  movb %0,[%1]++
  ifb  %0 == $0 then fin 
  outb %0 @loop
str:   .string "hello world (2)"
fin:
    stop
