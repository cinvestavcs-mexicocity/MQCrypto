#load('mi_key_pair.sage')
from key_pair import *

''' Class for Multivariate schemes variants '''
class MQVariation(object):
    def __init__(self, parameters):
        self.parameters = parameters
    
    def applyPublic(self, publicKey):
        return publicKey;
        
    def applyPrivate(self, privateKey):
        return privateKey;
    
    def getParameter(self, param):
        return self.parameters[param]

'''
'''
class MinusVariation(MQVariation):
    def __init__(self, parameters):
        MQVariation.__init__(self, parameters)
    
    def applyPublic(self, publicKey):
        polSet = publicKey.getSystem()
        publicKey.setSystem(polSet[0:len(polSet) - self.getParameter('r')])
        return publicKey
        
'''
'''
class PlusVariation(MQVariation):
    def __init__(self, parameters):
        MQVariation.__init__(self, parameters)
    
    def getPlusParam(self):
        return self.parameters['s']
    
    def applyPublic(self, publicKey):
        polSet = publicKey.getSystem()
        R = polSet[0].parent()
        for i in range(self.getParameter('s')):
            polSet.append(R.random_element())
        publicKey.setSystem(polSet)
        return publicKey

'''
'''
class MinusPlusVariation(MQVariation):
    def __init__(self, parameters):
        MQVariation.__init__(self, parameters)
    
    def applyPublic(self, publicKey):
        polSet = publicKey.getSystem()
        polSet = polSet[0:len(polSet) - self.getParameter('r')]
        R = polSet[0].parent()
        for i in range(self.getParameter('s')):
            polSet.append(R.random_element())
        publicKey.setSystem(polSet)
        return publicKey
