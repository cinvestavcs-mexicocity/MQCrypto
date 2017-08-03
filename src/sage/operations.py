from sage.all import *
from MPKC.MatsumotoImai.Sflash.sflashv1 import *
from MPKC.MatsumotoImai.Sflash.sflashv2 import *
from MPKC.UOV.uov import *
from MPKC.MatsumotoImai.matsumoto_imai import *
from MPKC.Utils.utils import *
import struct

def genKeyPair(clazz, path, args = None):
	scheme = getInstance(clazz, args)
	scheme.init()
	fkey = open(path + '.priv', 'wb')
	# print'Q\n{}'.format(scheme.Q)
	if 'UOV' in clazz:
		Qbin, Tbin = sysToBin(scheme.Q), affToBin(scheme.IT)
		ba = tlvWrite([sysToBin(scheme.Q), affToBin(scheme.IT)])
		# print('**** PRIV STORED\nQ: {}, len: {}\nT: {}, len: {}\nba: {}, len: {}'.format(toHex(Qbin), len(Qbin), toHex(Tbin), len(Tbin), toHex(ba), len(ba)))
	else:
		ba = tlvWrite([affToBin(scheme.IS), affToBin(scheme.IT)])
		#''.join('{:02x}'.format(ba))))
	fkey.write(ba)
	fkey.flush()
	fkey.close()
	fkey = open(path + '.pub', 'wb')
	ba = tlvWrite([sysToBin(scheme.P)])
	# print('**** PUB STORED\nP: {}'.format(toHex(ba)))
	fkey.write(ba)
	fkey.flush()
	fkey.close()
	return scheme

def loadPubKey(path):
	fkey = open(path, 'rb')
	ba = bytearray(fkey.read())
	ba = tlvRead(ba)[0]
	# print('**** PUB READ\nP: {}'.format(toHex(ba)))
	fkey.close()
	return ba

def loadPrivKey(clazz, path):
	fkey = open(path, 'rb')
	ba = bytearray(fkey.read())
	ba = tlvRead(ba)
	if 'UOV' in clazz:
		Q, S, T = ba[0], None, ba[1]
	else:
		Q, S, T = None, ba[0], ba[1]
	# print('**** PRIV READ\nQ: {}\nS: {}\nT: {}'.format(toHex(Q), toHex(S), toHex(T)))
	fkey.close()
	return Q, S, T

def tlvWrite(binVals):
	ba = bytearray()
	for i,e in enumerate(binVals):
		ba.extend(bytearray.fromhex("{0:0{1}x}".format(len(e),8)))
		ba.extend(e)
	return ba

def tlvRead(ba):
	# First 4 bytes are length
	l = len(ba)
	idx = 0
	arr = []
	while idx < l:
		# lv = binToInt(ba[idx: idx+4])
		lv = struct.unpack(">L", ba[idx: idx+4])[0]
		idx += 4
		# print('balen: {}'.format(lv))
		arr.append(ba[idx: idx + lv])	
		# print('baidx: {}'.format(toHex(ba[idx: idx + lv])))
		idx += lv
	return arr

def sign(clazz, msg, keypath, path, args = None):
	scheme = getInstance(clazz, args)
	msg = hexToBin(msg)
	Fqvec = binToVec(msg, scheme.Fq, scheme.m)
	Q, S, T = loadPrivKey(clazz, keypath + '.priv')
	# print('Vector: {}'.format(Fqvec))
	if 'Sflashv1' in clazz:
		Fq = GF(2)
	else:
		Fq = scheme.Fq
	if 'UOV' in clazz:
		scheme.Q = binToSys(Q, Fq, scheme.m, scheme.v, scheme.o)
		scheme.IT = binToAff(T, Fq, scheme.n)
	else:
		scheme.IS = binToAff(S, Fq, scheme.n)
		scheme.IT = binToAff(T, Fq, scheme.n)
	# print'Q\n{}'.format(scheme.Q)
	sign = scheme.privMap(Fqvec)
	sfile = open(path + '.sgn', 'wb')
	# print('sign: {}'.format(binToHex(vecToBin(sign))))
	sfile.write(vecToBin(sign))
	sfile.flush()
	sfile.close()

def verify(clazz, msg, keypath, sgnpath, args = None):
	scheme = getInstance(clazz, args)
	msg = hexToBin(msg)
	sfile = open(sgnpath + '.sgn', 'rb')
	sgn = bytearray(sfile.read())
	# print('Read: {}'.format(binToHex(sgn)))
	# print('Msg: {}'.format(binToHex(msg)))
	sfile.close()
	Fqmsg = binToVec(msg, scheme.Fq, scheme.m)
	Fqsgn = binToVec(sgn, scheme.Fq, scheme.n)
	Pbin = loadPubKey(keypath + '.pub')
	if 'Sflashv1' in clazz:
		Fq = GF(2)
	else:
		Fq = scheme.Fq
	scheme.setPub(binToSys(Pbin, Fq, scheme.m, scheme.n))
	Fqver = scheme.pubMap(Fqsgn)
	# print('Fqver: {}'.format(Fqver))
	# print('Fqmsg: {}'.format(Fqmsg))
	return Fqver == Fqmsg[:len(Fqver)]
	

def getInstance(clazz, args = None):
	if 'MatsumotoImai' == clazz:
		scheme = MatsumotoImai(args['q'], args['n'], args['alpha'], args['h'])
		scheme.Fqn = Fqn
	elif 'UOV' == clazz:
		scheme = UOV(args['q'], args['o'], args['v'])
	else:
		scheme = globals()[clazz]()
	return scheme