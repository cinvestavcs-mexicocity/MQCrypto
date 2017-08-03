from sage.all import *
from ..mq_bipolar import *
import time

class MatsumotoImai(MQBipolar):

	def __init__(self, q, n, alpha, h):
		super(MatsumotoImai, self).__init__(q, n, n)
		self.alpha = alpha
		self.h = h
		self.Cstar = None
		self.Fqn = None # By now, set extenson in subclasses. Probably better find a Conway like pol

	def setFqn(self, Fqn):
		self.Fqn = Fqn

	def genQ(self):
		return

	def genP(self):
		start = time.time()
		Xn = self.PFqn.gens()
		TX = list(self.Tmap(Xn))
		QTX = self.Fmap(TX)
		self.P = self.Smap(QTX)
		# print('MI.genP: {}'.format(time.time() - start))
		
	def getExpTable(self):
		raise NotImplementedError("Implement me!!")

	def IQmap(self, Fqvec):
		#Transform from k^n to K
		W = self.Fqn.gen()
		Wi = vector(map(lambda j: W**j, range(self.n)))
		B = vector(Fqvec) * Wi
 
        #Compute B^{h}
		A = B**(self.h)
        
        #Transform from K to k^n
		return vector(A.list())

	# Map F of MI scheme
	def Fmap(self, TX):
		if self.Cstar is None:
			# Create Polynomial Ring Fq[X1,...,Xn,T]
			Ln = map(lambda x: 'L' + str(x), range(self.n))
			Ln.append(str(self.Fqn.gen()))
			PFqnT = PolynomialRing(self.Fq, Ln)

			# Quotient Ring Fq[X1,...,Xn,T]/(Xi^q-Xi, f(T)) where f(T) is the irreducible polynomial of F_{q^n}
			Ln = PFqnT.gens()
			T = Ln[-1]
			Ln = Ln[:-1]
			Lbar = map(lambda x: x**self.q-x, Ln)
			Lbar.append(self.Fqn.modulus())
			QR = PFqnT.quo(Lbar)

			# Element X0 + X1*T + ... + X[n-1]*T^{n-1} 
			Ln = QR.gens()
			T = Ln[-1]
			Ln = vector(Ln[:-1])
			Tn = vector(map(lambda x: T**x, range(self.n)))

			# Quadratic map
			Qbar = ((Ln*Tn)**(self.q**self.alpha+1)).lift()

			FqXn = PolynomialRing(self.Fq, 'L', self.n, order = 'invlex')
			self.Cstar = map(lambda x: FqXn(Qbar.coefficient({T:x})), range(self.n))
		return map(lambda i: self.Cstar[i](list(TX)), range(self.n))

	# def Qmap(self, Fqvec):
	# 	Qx = []
	# 	powtab = self.getExpTable()
	# 	for i in range(self.n):
	# 		Qx.append(sum(map(lambda j: Fqvec[j], powtab[i])))
		
	# 	W = self.Fqn.gen()
	# 	q = self.Fq.order()
	# 	Wi = vector(map(lambda x: W**x, range(self.n)))
	# 	Qx = ((vector(Qx) * Wi) * (vector(Fqvec) * Wi)).lift().list()
	# 	return Qx