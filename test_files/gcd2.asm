### PGCD 2
.org 0
jmp 1024
pgcd: # a/b/ret=%1/[%2]/%0
  ifl %1==[%2] then pgcd_end
  ifl %1>[%2] then AsupB
              else BsupA
BsupA:
  subl [%2],[%2],%1 @pgcd
AsupB:
  subl %1,%1,[%2] @pgcd
pgcd_end:
  movl %0,%1
  ret

.org 1024
  inl %1 # a
  inl 1024-4 # b
  movw %2,$1024-4
  call pgcd
  outl %0
  stop
