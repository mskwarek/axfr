from ctypes import cdll
lib = cdll.LoadLibrary('../build/lib/libAxfrLib.so')

class axfrLookup(object):
    def __init__(self):
        self.obj = lib.axfrLookup_new()
    def performLookup(self, domain, ns):
        lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "udaoo sie"

x = axfrLookup()
x.performLookup("infusionsoft.com", "ns3.infusionsoft.com")
