from ctypes import cdll
import time 
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

def start_new_process(domain, ns):
    p = Process(target=perform_lookup, args=(domain, ns))    
    p.start()

def try_to_start_process(domain, ns):
    yield "dupa"
    
from multiprocessing import Process, Queue, Lock

if __name__ == '__main__':
    input_list = [line.rstrip('\n').split(' ') for line in open('inputData')]
    j = 0;
    for i in input_list:
        print j, " of ", len(input_list)
        j+=1
        while True:
            try:
                start_new_process(i[0], i[1])
                break
            except:
                print "wait"
                time.sleep(1)
        
            #p.join()
        #p.join() # this blocks until the process terminates
