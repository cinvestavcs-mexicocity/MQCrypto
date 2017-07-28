from sage.all import *
from operations import *

# keygen scheme [args] outfile
scheme = sys.argv[1]
i = 2
argDict = None
if 'UOV' in scheme:
	# args for UOV: q, o, v (size of Fq, # of oil vars, #of vinager vars)
	argDict = {'q': int(sys.argv[i]), 'o': int(sys.argv[i+1]), 'v': int(sys.argv[i+2])}
	i += 3
elif 'Matsumoto' in scheme:
	# args for MatsumotoImai: q, t, n (size of Fq, exponent for q^t+1, degree of extension)
	argDict = {'q': int(sys.argv[i]), 't': int(sys.argv[i+1]), 'n': int(sys.argv[i+2])}
	i += 3
msg = sys.argv[i]
i += 1
keyfile = sys.argv[i]
i += 1
if i < len(sys.argv):
	sgnfile = sys.argv[i]
else:
	sgnfile = keyfile
if verify(scheme, msg, keyfile, sgnfile, argDict):
	print('Verify: OK')
else:
	print('Verify: Fail')