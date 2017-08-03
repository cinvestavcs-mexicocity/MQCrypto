from MPKC.MatsumotoImai.Sflash.sflashv1 import *
from MPKC.Utils.utils import *
import time

start = time.time()
scheme = Sflashv1()
print('Object init: {}'.format(time.time()-start))

start = time.time()
scheme.init()
print('Key gen: {} - Pub size: {} b, Priv size: {}'.format(time.time()-start, len(sysToBin(scheme.P)), 2*len(affToBin(scheme.S))))

vec = vector(scheme.Fq, [1]*37)
for i in range(37):
	vec[i] = scheme.Fq.random_element()
start = time.time()
sign = scheme.privMap(vec)
print('Sign: time - {}'.format(time.time()-start))#, result - {}'.format(time.time()-start, sign))

start = time.time()
ver = scheme.pubMap(sign)
assert(vec[:26] == ver)
print('Verif: {}'.format(time.time()-start))

# start = time.time()
# scheme = Sflashv1()
# print('Object init: {}'.format(time.time()-start))

# start = time.time()
# scheme.init()
# print('Key gen: {}'.format(time.time()-start))

# vec = vector(scheme.Fq, [1]*37)
# for i in range(37):
# 	vec[i] = scheme.Fq.random_element()
# start = time.time()
# sign = scheme.privMap(vec)
# print('Sign: time - {}'.format(time.time()-start))#, result - {}'.format(time.time()-start, sign))

# start = time.time()
# ver = scheme.pubMap(sign)
# assert(vec[:26] == ver)
# print('Verif: {}'.format(time.time()-start))