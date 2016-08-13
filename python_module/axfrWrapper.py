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
    def destroy(self):
        lib.axfrLookup_destroy(self.obj)


        

def perform_lookup(domain, ns):
    x = axfrLookup()
    x.getResult(domain, ns)
    x.destroy()

from multiprocessing import Process, Queue

if __name__ == '__main__':
    for i in range(0,3):
        p = Process(target=perform_lookup, args=("infusionsoft.com", "ns3.infusionsoft.com"))
        p.start()
        p.join() # this blocks until the process terminates
