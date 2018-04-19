### PGCD 1
.org 0
jmp main
a: .long 0
b: .long 0
main:
  inl a
  inl a+4
loop:
  ifl a==b then end
  ifl a<b  then AinfB
           else BinfA
BinfA:
  subl a,a,b @loop
AinfB:
  subl b,b,a @loop
end:
  outl a
  stop
