# echo 3 1 2 3 4 5 6 | ./vmpico picocode
#   1 2 3
# + 4 5 6
# = 5 7 9
inl n
movl %1, $data
call readvecteur
call readvecteur

# v3 = v1 + v2
movl %0, $0           # cnt
movl %1, $data        # v1
addl %2, $data, n, n, n, n     # v2
addl %3, $data, n, n, n, n, n, n, n, n  # v3
sum:
ifl %0 == n then sum_fin
addl [%3]++,[%2]++,[%1]++
addl %0,%0,$1 @sum
sum_fin:

# print v3
movl %0, $0           # cnt
addl %3, $data, n, n, n, n, n, n, n, n  # v3
print:
ifl %0 == n then print_fin
outl [%3]++
addl %0,%0,$1 @print
print_fin:
stop

readvecteur: # into [%1]...
movl %0, $0           # cnt
readvecteur_boucle:
ifl %0 == n then readvecteur_fin
inl [%1]++
addl %0,%0, $1 @readvecteur_boucle
readvecteur_fin:
ret

n: .long 0
data:
