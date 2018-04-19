#test if with non equal condition file
.org 0
jmp main
a: .long 0
b: .long 0
main:
  inl a
  inl b
loop:
	ifl a!=b then end
tutu: outb $'E' 
		outb $'G'
		outb $'A'
		outb $'L'
		outb $'E'
		stop
end: outb $'P'
	  outb $'A'
	  outb $'S'
	  outb $' '
	  outb $'E'
	  outb $'G'
	  outb $'A'
	  outb $'L'
	  outb $'E'
	  stop
