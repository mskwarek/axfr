"""version interface module. Return database version, database connecting strings and application build version"""

def runDecoder(params):
    text = str(params['text'])
    input_key = str(params['input_key'])
    """the calculation from C++ library"""
	
    return {
        "decoded" : "test"
    }
    
def runCoder(params):
    text = str(params['text'])
    input_key = str(params['input_key'])
    """the calculation from C++ library"""
	
    return {
        "coded" : "test"
    }
