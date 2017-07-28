from .utils import *
import binascii	

NULL = 0X05
BOOLEAN = 0X01
INTEGER = 0X02
class ASN1(object):
	
	def __init__(self, encodingRule):
		self.encodingRule = encodingRule
	
	''' typeValues: Dictionary with values and types '''
	def encode(self, typeValues):
		if type(typeValues) != list:
			ValueError("parameter must be a list of pairs with type and value")
		if self.encodingRule == "BER":
			return self.berEncode(typeValues)
		else:
			ValueError("Encoding Rule Not Implemented")

	''' typeValues: Dictionary with values and types '''
	def decode(self, typeValues, encoding = 'byte'):
		if type(typeValues) != list:
			ValueError("parameter must be a list of pairs with type and value")
		if self.encodingRule == "BER":
			if (str.lower(encoding) == 'hex'):
				typeValues = bytearray.fromhex(typeValues)
			return self.berDecode(typeValues)
		else:
			ValueError("Encoding Rule Not Implemented")

	def berEncode(self, typeValueLst, code = 'byte'):
		ba = bytearray()
		for i in range(len(typeValueLst)):
			typeValue = typeValueLst[i]
			tp = typeValue[0]
			val = typeValue[1]
			if (str(code) == 'hex'):
				val = bytearray.fromhex(val)
			#TAG
			ba.append(tp)
			#LENGTH
			if tp == NULL:				
				ba.append(0x00)
			if tp == BOOLEAN:
				ba.append(0x01)
			if tp == INTEGER:
				if type(val) in [int, Integer, long]:
					val = intToBin(val)
#				print 'Value To Encode:'
#				print binascii.hexlify(val)
				ba.extend(self.getValueLen(val))
			else:
				ValueError("Not implemented")
			#VALUE
			if val != None:
				ba.extend(val)
		return bytes(ba)

	def getValueLen(self, val):
		l = len(val)
		ba = bytearray()
		if l < 0X8F:
			ba.append(l)
		else:
			lba = intToBin(l)
			ba.append(len(lba) | 0x80)
#			print 'Length:'
#			print binascii.hexlify(ba)
			ba.extend(lba)
#		print binascii.hexlify(ba)
		return ba
	
	def berDecode(self, barray):
		typeValueList = []
		idx = 0
		iter = 0
		while idx < len(barray) and iter < 10:
			tp, val, idx = self.decodeItem(barray, idx)
			if tp == INTEGER:
				typeValueList.append([tp, val])
			else:
				ValueError('Not Implemented')
			iter += 1
		return typeValueList

	def decodeItem(self, barray, idx):
		# Read tag: first byte
		tp = barray[idx]
		idx += 1
		# Get the value
		l = barray[idx]
		idx += 1
		if (0x80 & l) != 0:
			l = l & 0x7F
			idx += l		
			l = binToInt(barray[idx - l: idx])	
		val = barray[idx: idx + l]
		idx += l
		return tp, val, idx

