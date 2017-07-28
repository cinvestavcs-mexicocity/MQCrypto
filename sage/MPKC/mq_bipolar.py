from sage.all import *
import multiprocessing as mp
import time

''' General class for bipolar type systems
'''
class MQBipolar(object):
	# With the must simple parameters, a default construction for the elements
	def __init__(self, q, n, m):
		# Hopefully a Conway polynomial exists in database.
		self.Fq = GF(q, 'X')
		self.q = q
		self.n = n
		self.m = m
		self.PFqn = PolynomialRing(self.Fq, 'X', self.n)

	# Complete specification of the scheme elements. Fill with after P if only public map is known
	def setPriv(self, S, T, Q, IQ = None, IS = None, IT = None):
		self.Q = Q
		self.S = S
		self.T = T
		self.IQ = IQ
		self.IS = IS
		self.IT = IT

	def setPub(self, P):
		self.P = P

	def init(self):
		self.genS()
		self.genT()
		self.genQ()
		self.genP()

	def genQ(self):
		raise NotImplementedError("Implement me!!")

	def genS(self):
		self.S = self.genAff(self.m)
		self.IS = self. S.inverse()

	def genT(self):
		self.T = self.genAff(self.n)
		self.IT = self.T.inverse()

	def genAff(self, n):
		AG = AffineGroup(n, self.Fq)
		return AG.random_element()

	def genP(self):
		Xi = vector(self.PFqn.gens())
		self.P = self.Smap(self.Qmap(self.Tmap(Xi)))

	def Pmap(self, Fqvec):
		return self.applyMap(self.P, Fqvec)

	def Qmap(self, Fqvec):
		return self.applyMap(self.Q, Fqvec)

	# Apply inverse of Q. Should be implemented accordingly
	def IQmap(self, Fqvec):
		return self.applyMap(self.IQ, Fqvec)

	def Smap(self, Fqvec):
		return self.applyMap(self.S, Fqvec)

	def Tmap(self, Fqvec):
		return self.applyMap(self.T, Fqvec)

	def ISmap(self, Fqvec):
		return self.applyMap(self.IS, Fqvec)

	def ITmap(self, Fqvec):
		return self.applyMap(self.IT, Fqvec)

	def privMap(self, Fqvec):
		return self.ITmap(self.IQmap(self.ISmap(Fqvec)))

	def pubMap(self, Fqvec):
		return self.Pmap(Fqvec)

	def applyMap(self, M, Fqvec):
		if type(M) == list:
			if type(M[0]) == list and len(M[0]) == 3:
				res = self.applyQuadMatrix(M, Fqvec)
			else:
				res = []
				for i in range(len(M)):
					res.append(M[i](list(Fqvec)))
		elif type(M) == type(AffineGroup(1, GF(2)).random_element()):
			res = M.A() * vector(Fqvec) + M.b()
		else:
			res = M(list(Fqvec))
		return res

	def applyQuadMatrix(self, M, Fqvec):
		res = []
		lvec = Fqvec[:M[0][0].nrows()]
		for i in range(len(M)):	
			res.append(lvec*M[i][0]*Fqvec + M[i][1]*Fqvec + M[i][2])
		return vector(res)

	# def applyQuadMap(self, lvec, Fqvec, M):
	# 	start = time.time()
	# 	res = map(lambda i: lvec*M[i][0]*Fqvec + M[i][1]*Fqvec + M[i][2], M)
	# 	print('QuadMap {}: {}'.format(i, time.time() - start))
	# 	return res