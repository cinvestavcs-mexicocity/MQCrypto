from sage.all import *
from ..mq_bipolar import *
# import concurrent.futures
# from multiprocessing.pool import cpu_count, Manager
from multiprocessing.pool import ThreadPool
import multiprocessing as mp
import time, os

class UOV(MQBipolar):
	
	def __init__(self, q, o, v):
		super(UOV, self).__init__(q, o+v, o)
		self.o = o
		self.v = v
	
	def genQ(self):
		start = time.time()
		self.Q = []
		MS = MatrixSpace(self.Fq, self.v, self.n)
		VS = VectorSpace(self.Fq, self.n)
		for i in range(self.o):
			self.Q.append([MS.random_element(), VS.random_element(), self.Fq.random_element()])

	def genP(self):
		A = copy(self.T.A())
		At = A.submatrix(nrows = self.v).transpose()
		b = copy(self.T.b())
		bt = b[:self.v]
		self.P = [0] * self.m
		start = time.time()

		###### Sequencial code:
		# map(lambda i: self.genPi(A, At, b, bt, i), range(self.m))

		####### ThreadPool. Does not run multiprocesses
		# nthreads = 2
		# print('genP with {} threads'.format(nthreads))
		# pool = ThreadPool(nthreads)
		# self.P = pool.map(lambda i: self.genPi(A, At, b, bt, i), range(self.m))
		
		###### Processes. Compute chunks in each process. Allows use of cores
		# Prepare chunks of indices for each process
		nprocs = 4#cpu_count()
		chunks = [int(math.ceil(self.m / float(nprocs)))] * nprocs
		total = sum(chunks)
		i = 0
		while total > self.m:
			chunks[i] -= 1
			total -= 1
			i += 1

		# # manager = mp.pool.Manager()
		# out_q = mp.Queue()
		lr = 0
		hr = 0
		procs = []
		for i in range(nprocs):
			hr += chunks[i]
			# Process i computes values for chunk i
			print('range proc-{}: {}'.format(i, range(lr, hr)))
			p = mp.Process(target=self.genPproc, args=(A, At, b, bt, range(lr, hr)))#, out_q))
			lr += chunks[i]
			procs.append(p)
			p.start()
		print('joining')
		# added = 0
		for p in procs:
			# while added < self.m:
			# 	# Flushing queue to join remaining processes. Full queue blocks
			# 	try:
			# 		g = out_q.get()
			# 		self.P[g[0]] = g[1]
			# 		added += 1
			# 		print('{} of {}'.format(added, self.m))
			# 	except:
			# 		break
			p.join()
			print('{} joined :)'.format(p.name))
		# out_q.close()


		## ProcessPool. Processes creation. Results stored in out_q
		# manager = Manager()
		# Shared matrices for processes
		# matrices = manager.dict()
		# matrices.put('A', A)
		# matrices.put('At', At)
		# matrices.put('b', b)
		# matrices.put('bt', bt)
		# Shared dictionary for results
		# results = manager.dict()
		# nprocs = cpu_count()
		# with concurrent.futures.ProcessPoolExecutor(max_workers=nprocs) as group_link_processes:
		# 	for i in range(self.m):
		# 		group_link_processes.submit(self.genPi, A, At, b, bt, )
		print('P[0]: {}'.format(self.P[0]))
		print('P: {}'.format(time.time() - start))

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
				xv = []
				for i in range(self.v):
					xv.append(self.Fq.random_element())
				alpha = xv[:]
				alpha.extend(Xn[self.v:])
				alpha = vector(alpha)
				start = time.time()
				alpha = self.Qmap(alpha)
				print('IQ - Q: {}'.format(time.time() - start))

				A = []
				b = []
				for i in range(len(alpha)):
					A.append(map(lambda d: alpha[i].coefficient(d), dXo))
					b.append(alpha[i].constant_coefficient())
				A = matrix(self.Fq, A)
				b = vector(self.Fq, b) + Fqvec

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

	
	def genPproc(self, A, At, b, bt, idx):#, out_q):
		print('Performing genPaux')
		start = time.time()
		# res = []
		for i in idx:
			# Mi = self.Q[i][0]
			# vi = self.Q[i][1]
			# ci = self.Q[i][2]
			# MiA = Mi*A
			# Mib = Mi*b
			# # self.genPi( A, At, b, bt, i)
			# [At*MiA, At*Mib + bt*MiA + A.transpose()*vi, vi*b + bt*Mib + ci]
			self.genPi(A, At, b, bt, i)
			# out_q.put((i, [At*MiA, At*Mib + bt*MiA + A.transpose()*vi, vi*b + bt*Mib + ci]))
		# return res
		print('genPaux: {}'.format(time.time() -  start))

	def genPi(self, A, At, b, bt, i):
		# Mi = self.Q[i][0]
		# vi = self.Q[i][1]
		# ci = self.Q[i][2]

		start = time.time()
		# MiA = Mi*A
		# Mib = Mi*b
		# Pi = 
		# [At*MiA, At*Mib + bt*MiA + A.transpose()*vi, vi*b + bt*Mib + ci]
		A.transpose()*A
		A.transpose()*A
		print('genP[{}]: {}, process: {}'.format(i, time.time() -  start, os.getpid()))	
		# print('genP: {}'.format(self.P))
		# self.P[i] = Pi
		# return Pi

	# def genPiProc(self, A, At, b, bt, i, out_dict):
	# 	Mi = self.Q[i][0]
	# 	vi = self.Q[i][1]
	# 	ci = self.Q[i][2]
	# 	MiA = Mi*A
	# 	Mib = Mi*b
	# 	out_dict.put(i, self.genPi(A, At, b, bt, i))
