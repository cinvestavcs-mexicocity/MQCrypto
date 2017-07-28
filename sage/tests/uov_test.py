from MPKC.UOV.uov import *
from MPKC.Utils.utils import *
import time

start = time.time()
q = 2**4
o = 40
v = 2*o
n = o+v

scheme = UOV(q, o, v)
print('Object init: {}'.format(time.time()-start))

start = time.time()
scheme.init()
print('Key gen: {}'.format(time.time()-start))

vec = []
for i in range(o):
	vec.append(scheme.Fq.random_element())
vec = vector(scheme.Fq, vec)
start = time.time()
sign = scheme.privMap(vec)
print('Sign: {}'.format(time.time()-start))#, result - {}'.format(time.time()-start, sign))

start = time.time()
ver = scheme.pubMap(sign)
assert(vec == ver)
print('Verif: {}'.format(time.time()-start))

