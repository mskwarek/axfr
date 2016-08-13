from ctypes import cdll
lib = cdll.LoadLibrary('../build/lib/libAxfrLib.so')

class axfrLookup(object):
    def __init__(self):
        self.obj = lib.axfrLookup_new()
    def performLookup(self, domain, ns):
        lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "udaoo sie"
    def getResult(self, domain, ns):
        lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "after perform"
        dupa = lib.axfrLookup_getResult(self.obj)
        lib.print_data(dupa)


x = axfrLookup()
x.getResult("infusionsoft.com", "ns3.infusionsoft.com")
