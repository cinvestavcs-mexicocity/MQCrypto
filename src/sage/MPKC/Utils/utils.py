from sage.all import *
import time

''' Converts the byte array to a vector in the vector space (Fq)^n
'''
def binToVec(ba, Fq, n):
	d = Fq.degree()
	if n == 0:
		l = len(ba)
		n = int(ceil(8 * l / d))
	v = binToInt(ba)
	vec = intToVec(v, Fq, n)
	return vec

def intToVec(v, Fq, n):
	d = Fq.degree()
	V = []
	pad = 0
	i = 0
	while i < d:
		pad = pad << 1
		pad = pad | 1
		i += 1
	i = 0
	if d > 1:
		to_int = Fq.fetch_int
	else:
		to_int = Fq
	while i < n:
		V.append(to_int(v & pad))
		v = v >> d
		i += 1
	return vector(V)		

def vecToInt(vec):
	l = len(vec)
	Fq = vec[0].parent()
	d = Fq.degree()
	y = 0
	if d > 1:
		for i in range(l):
			y = y | (vec[i].integer_representation() << (i * d))
	else:
		for i in range(l):
			y = y | int(vec[i]) << (i * d)

	# for i in range(l):
	# 	y = (y << d) | vec[i].integer_representation()
	# map(lambda e: e.integer_representation(), vec)
	return y

def affToBin(Aff):
	vec = Aff.A().list()
	vec.extend(Aff.b())
	return vecToBin(vec)

def binToAff(ba, Fq, n):
	AG = AffineGroup(n, Fq)
	d = Fq.degree()
	aux = d*n**2
	vecIdx = int(floor(1.0*aux/8))
	vecSft = aux % 8
	mat = binToMat(ba, Fq, n, n)
	vec = intToVec(binToInt(ba[vecIdx:]) >> vecSft, Fq, n)
	return AG(mat, vec)
	
def vecToBin(vec):
	l = len(vec)
	Fq = vec[0].parent()
	d = Fq.degree()
	size = int(8.0 * d / 8)
	ba = bytearray()
	i, j = 0, 8
	while j < l:
		ba.extend(intToBin(vecToInt(vec[i:j]), size))
		j += 8
		i += 8
	if i < l:
		ba.extend(intToBin(vecToInt(vec[i:]), int(ceil(d*(l-i) / 8.0))))
	return ba

def matToBin(mat):
	vec = mat.list()
	return vecToBin(vec)

def binToMat(ba, Fq, n, m):
	A = matrix(Fq, n,m)
	d = Fq.degree()
	v = binToInt(ba)
	for i in range(n):
		idx = int(ceil(m*d/8))
		A[i] = intToVec(v, Fq, m)
		v = v >> (d*m)
	return A

def binToInt(ba):
	n = 0
	for i in range(len(ba)):
		n = n | (ba[i] << (8*i))
	return n

def intToBin(x, n = 0):
	if n == 0:
		j = 0
		while (x >> j) > 0:
			n += 1
	ba = bytearray(n)
	for i in range(len(ba)):
		ba[i] = x & 0xff
		x = x >> 8
	return ba

def upMatToBin(mat):
	n = mat.nrows()
	vec = []
	for i in range(n):
		vec.extend(mat[i].list()[i:])
	return vecToBin(vec)

def upMatToVec(mat):
	n = mat.nrows()
	vec = []
	for i in range(n):
		vec.extend(mat[i].list()[i:])
	return vec

def binToUpMat(ba, Fq, n, m):
	v = binToInt(ba)
	return intToUpMat(v, Fq, n, m)

def intToUpMat(v, Fq, n, m):
	d = Fq.degree()
	mat = matrix(Fq, n, m)
	for i in range(n):
		#print v
		mat[i] = intToVec(v, Fq, m)
		v = (v >> (m*d)) << (i+1)*d
	return mat

'''
'''
def quadPolToVec(p):
	vec = []
	Fq = p.constant_coefficient().parent()
	Xn = p.parent().gens()
	n = len(Xn)
	#Quadratic coefficients
	for i in range(n):
		c = p.coefficient({Xn[i]:2})
		vec.append(Fq(c))
		p = p - c*Xn[i]**2
		for j in range(i+1, n): 
			# print('c: {}'.format(c))
			c = p.coefficient({Xn[i]:1, Xn[j]:1})
			vec.append(Fq(c))
			p = p - c*Xn[i]*Xn[j]
	#Linear coefficients
	for i in range(n):
		c = p.coefficient({Xn[i]: 1})
		vec.append(Fq(c))
	#Constant coefficient
	vec.append(Fq(p.constant_coefficient()))
	return vec

def sysToBin(P):
	p = P[0]
	vec = []
	if type(p) == list or type(p) == tuple:
		for p in P:
			vec.extend(upMatToVec(p[0]))
			vec.extend(p[1])
			vec.append(p[2])
	else:
		for p in P:
			vec.extend(quadPolToVec(p))
	return vecToBin(vec)

def binToSys(ba, Fq, m, n1, n2 = 0):
	P = []
	n = n1 + n2
	matsize = n1 * (n1+1) /2 + n1*n2
	# print('matsize: {}'.format(matsize))
	chunksize = matsize + n + 1
	d = Fq.degree()
	size = d / 8.0
	offset = 0
	idxs = 0
	idxe = int(ceil(chunksize * size))
	degpow = 1 << d
	# print('chunksize: {}, size: {}, degpow: {}'.format(chunksize, size, degpow))
	for i in range(m):
		# print('*******P{}*********\nidxs: {}, idxe: {}, offset: {}'.format(i, idxs, idxe, offset))
		Pi = []
		v = binToInt(ba[idxs: idxe]) >> offset
		# print('ba: {}'.format(''.join('{:02x}'.format(x) for x in ba[idxs: idxe])))
		# print('{}: {}'.format('{0:02x}: '.format(v), v))
		Pi.append(intToUpMat(v, Fq, n1, n))
		v = v >> d*matsize
		# print('{}: {}'.format('{0:02x}: '.format(v), v))
		Pi.append(intToVec(v, Fq, n))
		v = v >> d*n
		# print('{}: {}'.format('{0:02x}: '.format(v), v))
		if d > 1:
			to_int = Fq.fetch_int
		else:
			to_int = Fq
		Pi.append(to_int(v % degpow))
		offset = (offset + chunksize*d) % 8
		aux = idxe
		idxe = idxe + int(ceil((chunksize * d - (-offset % 8)) / 8.0))
		idxs = aux - int(ceil(offset / 8.0))
		# print('jump: {}'.format(int(ceil((chunksize * d + offset - 8) / 8.0))))
		P.append(Pi)
	return P

def binToHex(ba):
	if ba == None:
		return ''
	return ''.join('{:02x}'.format(x) for x in ba)

def hexToBin(hexstr):
	return bytearray(hexstr.decode("hex"))

''' Higher degree in leftmost position
def polToInt(pol):
	polInt = 0
	coeff = pol.list()
	for i in range(len(coeff)):
		if coeff[i] == 1:
			polInt = polInt | (1 << i)
	return polInt'''

# def polToInt(pol):
# 	return int(pol)

# def polToBin(pol):
# 	return intToBin(polToInt(pol))

# '''
# '''
# def quadraticPolToInt(p):
# 	bin = 0
# 	vars = p.parent().gens()
# 	varNum = len(vars)
# 	#Quadratic coefficients
# 	for i in range(varNum):
# 		for j in range(i, varNum):
# 			c = int(p.coefficient(vars[i]*vars[j]))
# 			bin = (bin << 1) | c
# 			p = p - c*vars[i]*vars[j]
# 	#Linear coefficients
# 	for i in range(varNum):
# 		c = int(p.coefficient(vars[i]))
# 		bin = (bin << 1) | c
# 	#Constant coefficient
# 	bin = (bin << 1) | int(p.constant_coefficient())
# 	return bin

# '''
# '''
# def quadraticPolToMatrix(p):
# 	vars = p.parent().gens()
# 	varNum = len(vars)
# 	MS = MatrixSpace(p.constant_coefficient().parent(), varNum)
# 	VS = VectorSpace(p.constant_coefficient().parent(), varNum)
# 	#Quadratic coefficients
# 	Qp = []
# 	vp = []
# 	for i in range(varNum):
# 		Qp.append([0]*varNum)
# 		for j in range(i, varNum):
# 			c = p.coefficient(vars[i]*vars[j])
# 			Qp[i][j] = c
# 			p = p - c*vars[i]*vars[j]
# 	#Linear coefficients
# 	for i in range(varNum):
# 		vp.append(p.coefficient(vars[i]))
# 	vp = VS(vp)
# 	Qp = MS(Qp)
# 	#Constant coefficient
# 	return [Qp, vp, p.constant_coefficient()]

# def quadMapToMatrixForm(P):
# 	if type(P) != list:
# 		pp = []
# 		for p in enumerate(P):
# 			pp.append(quadraticPolToMatrix(p[1]))
# 	else:
# 		pp = P
# 	return pp

# ''' FROM OLD CODE '''
# def affToBin(self, aff):
# 	ba = 
# 	baseField = affine.matrix()[0,0].parent()
# 	deg = baseField.polynomial().degree()
# 	affineMatrix = affine.matrix()[0:deg, 0:deg]
# 	affineVector = affine.matrix()[0:deg, deg:deg+1]
# 	affineMatrixBin = bytearray()
# 	affineVectorBin = bytearray()
# 	affine1VectorBin.append(GF2nElmToBin(affineVector, baseField))



# ''' Int to 0-1 polynomials for base and extension fields'''
# def getBaseAndExtension(self, m, n):
# 	#r.<X>=GF(2)[]
# 	r = PolynomialRing(GF(7), 'X')
# 	pol = 0
# 	i = 0
# 	while (m > 0):
# 		pol = pol + (m & 1)*X**i
# 		m = m >> 1
# 		i += 1
# 	k=GF(2**pol.degree(), 'x', pol)
# 	R=PolynomialRing(k,'T')
# 	T=R.gens()[0]
# 	i = 0
# 	pol = k(0)
# 	while n > 0:
# 		pol = pol + k(n & 1)*T**i
# 		n = n >> 1
# 		i += 1
# 	K=k.extension(T**37+T**12+T**10+T**2+1, 't')
# 	return k, K
