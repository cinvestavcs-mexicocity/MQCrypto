from MPKC.UOV.uov import *

# Fq = GF(q, 'x')
# VS = VectorSpace(Fq, o)
# msg = vecToBin(VS.random_element())
# path = '/home/edgar/MQCrypto/python/keygen/uov_key'
# clazz = 'UOV'
# args = {'q': q, 'o': o, 'v': v}

q = 2**7
m = 26
n = 37
Fq = GF(q, 'x')
VS = VectorSpace(Fq, m)
msg = vecToBin(VS.random_element())
path = '/home/edgar/MQCrypto/python/keygen'
clazz = 'Sflashv2'
args = None

scheme1 = genKeyPair(clazz, path, args)
print('keygen')

start = time.time()
ba = sysToBin(scheme.P)
# print ''.join('{:02x}'.format(x) for x in ba)
print('Pub enc: {} - size: {}b'.format(time.time()-start, len(ba)))

start = time.time()
ba = sysToBin(scheme.Q)
ba.extend(affToBin(scheme.T))
print('Priv enc: {} - size: {}b'.format(time.time()-start, len(ba)))

sgn, scheme2 = sign(clazz, path, msg, args)
print('sign: {}'.format(''.join('{:02x}'.format(x) for x in sgn)))
# print (scheme1.Q == scheme2.Q and scheme1.IT == scheme2.IT)
ver, scheme3 = verify(clazz, path, msg, sgn, args)
# print (scheme1.P == scheme3.P)
print('verified: {}'.format(ver))