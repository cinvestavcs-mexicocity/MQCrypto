from MPKC.MatsumotoImai.matsumoto_imai import *
from MPKC.Utils.utils import *
import time

# Toy example
x = var('x')
# Parameters
q = 2**2
n = 3
h = 26
alpha = 2
f = x **3 + x + 1

# # Find h (inverse of q**t+1)
# qn = q**n-1
# result = xgcd(q**alpha+1, qn)
# if result[0] != 1:
# 	print result[0]
# 	raise ValueError('Valio')
# h = result[1] % qn


# # Irreducible polynomial of degree n
# Fq = GF(q, 'X')
# PFqn = PolynomialRing(Fq, 'T')
# f = PFqn.irreducible_element(n)
# print f

# # Sflash values without minus variant
# i, j, x = var('i', 'j', 'x')
# q = 2**7
# n = 37
# alpha = 11
# qn = q**n-1
# h = xgcd(q**alpha+1, qn)[1] % qn
# #466815348407827775651785930077564057715134843197125142436974563133435145084864#2**258+sum(sum(2**j,j,154*i+76,154*i+152),i,0,17)
# f = x**37 + x**12 + x**10 + x**2 + 1

start = time.time()
scheme = MatsumotoImai(q, n, alpha, h)
Fqn = scheme.Fq.extension(f)
# print Fqn.modulus()
scheme.setFqn(Fqn)
print('Object init: {}'.format(time.time()-start))

start = time.time()
scheme.init()
print('Key gen: {}'.format(time.time()-start))

vec = vector(scheme.Fq, [1]*n)
for i in range(n):
	vec[i] = scheme.Fq.random_element()
start = time.time()
sign = scheme.privMap(vec)
print('Sign: time - {}'.format(time.time()-start))

start = time.time()
ver = scheme.pubMap(sign)
assert(vec == ver)
print('Verif: time - {}'.format(time.time()-start))