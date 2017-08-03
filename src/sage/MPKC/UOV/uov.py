from sage.all import *
from ..mq_bipolar import *

class UOV(MQBipolar):
	
	def __init__(self, q, o, v):
		super(UOV, self).__init__(q, o+v, o)
		self.o = o
		self.v = v
	
	def genQ(self):
		self.Q = []
		MS = MatrixSpace(self.Fq, self.v, self.n)
		VS = VectorSpace(self.Fq, self.n)
		for i in range(self.o):
			A = MS.random_element()
			for j in range(self.v):
				for k in range(j):
					A[j,k] = 0
			self.Q.append([A, VS.random_element(), self.Fq.random_element()])

	def genP(self):
		A = self.T.A()
		At = A.submatrix(nrows = self.v).transpose()
		b = self.T.b()
		bt = b[:self.v]
		self.P = map(lambda i: self.genPi(A, At, b, bt, i), range(self.m))

	def genS(self):
		pass

	def IQmap(self, Fqvec):
		count = 0
		found = False
		Xn = self.PFqn.gens()
		dXo = []
		for i in range(self.o):
			dXo.append({Xn[self.v+i]:1})
		while not found and count < 100:
			try:
				# Set random values to vinegar variables
				xv = map(lambda i: self.Fq.random_element(), range(self.v))
				alpha = xv[:]
				alpha.extend(Xn[self.v:])
				alpha = vector(alpha)
				alpha = self.Qmap(alpha)

				# Create and solve linear system of oil variables
				A = []
				b = []
				for px in alpha:
					A.append(map(lambda deg: px.coefficient(deg), dXo))
					b.append(px.constant_coefficient())
				A = matrix(self.Fq, A)
				b = vector(self.Fq, b) - Fqvec

				# Add values of solutions for oil variables
				y = A.solve_right(b)
				xv.extend(y)
				found = True
			except ValueError:
				count += 1
		if (count == 100):
			raise ValueError('Solution not found')
#		print('found: {}'.format(xv))
		return vector(xv)

	def ISmap(self, Fqvec):
		return Fqvec

	def Smap(self, Fqvec):
		return Fqvec

	def genPi(self, A, At, b, bt, i):
		Mi = self.Q[i][0]
		vi = self.Q[i][1]
		ci = self.Q[i][2]
		start = time.time()
		MiA = Mi*A
		Mib = Mi*b
		QF = At*MiA
		# print('A\n{}'.format(A))
		# Triangular form
		for j in range(self.n):
			for k in range(j):
				QF[k,j] = QF[j,k] + QF[k,j]
				QF[j,k] = 0
		# print('Atr\n{}'.format(A))
		Pi = [QF, At*Mib + bt*MiA + A.transpose()*vi, vi*b + bt*Mib + ci]
		return Pi