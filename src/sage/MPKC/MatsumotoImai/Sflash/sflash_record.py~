from ..mi_record import *

class SflashPublicRecord(MIPublicRecord):
	def nada(self):
		return None

class SflashPrivateRecord(Record):
	componentType = namedtype.NamedTypes(namedtype.NamedType('ndim', univ.Integer()),
		namedtype.NamedType('affine1', univ.Integer()),
		namedtype.NamedType('mdim', univ.Integer()),
		namedtype.NamedType('affine2', univ.Integer()),
		namedtype.NamedType('delta', univ.OctetString()))
	
	def setDelta(self, delta):
		self.setComponentByName('delta', delta)

	def setNdim(self, n):
		self.setComponentByName('ndim', n)

	def setAffine1(self, affine1):
		self.setComponentByName('affine1', affine1)

	def setAffine2(self, affine2):
		self.setComponentByName('affine2', affine2)

	def setMdim(self, m):
		self.setComponentByName('mdim', m)
