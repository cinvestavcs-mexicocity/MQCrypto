from pyasn1.type import univ, namedtype

class Record(univ.Sequence):
	def encode(self, encoding):
		if encoding.upper() == 'BER':
			from pyasn1.codec.ber import encoder
		elif encoding.upper() == 'CER':
			from pyasn1.codec.cer import encoder
		elif encoding.upper() == 'DER':
			from pyasn1.codec.der import encoder
		else:
			ValueError('Unknown encoding')
		return encoder.encode(self)

	def decode(self, encoding, code):
		if encoding.upper() == 'BER':
			from pyasn1.codec.ber import decoder
		elif encoding.upper() == 'CER':
			from pyasn1.codec.cer import decoder
		elif encoding.upper() == 'DER':
			from pyasn1.codec.der import decoder
		else:
			ValueError('Unknown encoding')
		return decoder.decode(code)

	def __str__(self):
		return self.prettyPrint()


class MIPublicRecord(Record):
	componentType = namedtype.NamedTypes(namedtype.OptionalNamedType('primeField', univ.Integer()),
		namedtype.OptionalNamedType('baseField', univ.Integer()),
		namedtype.OptionalNamedType('extensionField', univ.Integer()),
		namedtype.NamedType('nvars', univ.Integer()),
		namedtype.NamedType('publicSystem', univ.Integer()))

	def setPrimeField(self, p):
		self.setComponentByName('primeField', p)

	def setBaseField(self, baseField):
		self.setComponentByName('baseField', baseField)

	def setExtensionField(self, extField):
		self.setComponentByName('extensionField', extField)

	def setNvars(self, n):
		self.setComponentByName('nvars', n)

	def setPublicSystem(self, pubSystem):
		self.setComponentByName('publicSystem', pubSystem)

class MIPrivateRecord(Record):
	componentType = namedtype.NamedTypes(namedtype.OptionalNamedType('primeField', univ.Integer()),
		namedtype.OptionalNamedType('baseField', univ.Integer()),
		namedtype.OptionalNamedType('extensionField', univ.Integer()),
		namedtype.OptionalNamedType('theta', univ.Integer()),
		namedtype.NamedType('ndim', univ.Integer()),
		namedtype.NamedType('affine1', univ.Integer()),
		namedtype.NamedType('mdim', univ.Integer()),
		namedtype.NamedType('affine2', univ.Integer()))

	def setPrimeField(self, p):
		self.setComponentByName('primeField', p)

	def setBaseField(self, baseField):
		self.setComponentByName('baseField', baseField)

	def setExtensionField(self, extField):
		self.setComponentByName('extensionField', extField)

	def setNvars(self, theta):
		self.setComponentByName('theta', theta)

	def ndim(self, n):
		self.setComponentByName('ndim', n)

	def setAffine1(self, affine1):
		self.setComponentByName('affine1', affine1)

	def setAffine2(self, affine2):
		self.setComponentByName('affine2', affine2)

	def mdim(self, m):
		self.setComponentByName('mdim', m)
