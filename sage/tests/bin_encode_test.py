from sage.all import *
from MPKC.Utils.utils import *

q = 16
n = 100
Fq = GF(q, 'X')
AG = AffineGroup(n, Fq)

print('vec')
S = AG.random_element()
# print S.b()
#Vector test
b = S.b()
ba = vecToBin(b)
vec = binToVec(ba, Fq, n)
# print('vec: ' + str(vec))
assert(vec == b)
#raise print('vec == b?: ' + str(vec == b))

# Matrix test
print('mat')
A = S.A()
ba = matToBin(A)
# print('ba: ' + str(ba))
mat = binToMat(ba, Fq, n, n)
assert(mat == A)
#print('mat == A?: ' + str(mat == A))

# Affine test
print('aff')
ba = affToBin(S)
# print('ba: ' + str(ba))
aff = binToAff(ba, Fq, n)
assert(aff == S)
#print('aff == S?: ' + str(aff == S))
