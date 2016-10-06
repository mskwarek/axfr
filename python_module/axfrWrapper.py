from ctypes import cdll
import ctypes
import time 
lib = cdll.LoadLibrary('/home/marcin/ClionProjects/myDig/build/lib/libAxfrLib.so')

class axfrLookup(object):
    lib = cdll.LoadLibrary('/home/marcin/ClionProjects/myDig/build/lib/libAxfrLib.so')

    def __init__(self):
        self.obj = self.lib.axfrLookup_new()
    def performLookup(self, domain, ns):
        self.lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "udaoo sie"
    def getResult(self, domain, ns):
        self.lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "after perform"
        self.lib.axfrLookup_getReturnedDomains.restype = ctypes.POINTER(ctypes.c_char_p * self.lib.axfrLookup_getSizeOfReturnedData(self.obj))
        dupa = self.lib.axfrLookup_getReturnedDomains(self.obj)
        print dupa
        #lib.print_data(dupa)
    def destroy(self):
        self.lib.axfrLookup_destroy(self.obj)

    def dumbData(self):
        self.lib.axfrLookup_getDumbVector.argtype = ctypes.c_void_p
        self.lib.axfrLookup_getDumbVector.restype = ctypes.POINTER(ctypes.c_char_p * 3) #ctypes.POINTER(ctypes.c_char_p * 3)
        x = self.lib.axfrLookup_getDumbVector(self)
        #return '[{}]'.format(', '.join(str(self[i]) for i in range(len(self))))
        print x[0] 

    def dumbDataInt(self):
        self.lib.axfrLookup_getDumbVectorInt.argtype = None
        self.lib.axfrLookup_getDumbVectorInt.restype = ctypes.POINTER(ctypes.c_int)
        x = self.lib.axfrLookup_getDumbVectorInt()
        print x[3]

def perform_lookup(in_list):
    x = axfrLookup()
    print "dziala", in_list[0], in_list[1]
    x.getResult(in_list[0], in_list[1])
    x.destroy()
    return 0
    

def start_new_process(domain, ns):
    p = Process(target=perform_lookup, args=(domain, ns))    
    p.start()
    processes.append(p)

from multiprocessing import Process, Queue, Lock
import multiprocessing
import threading

def log_result(result):
    print "result", result

def process_lookup(input_list):
    pool = multiprocessing.Pool(None)
    for i in input_list:
        print i
        pool.apply_async(perform_lookup, args = (i, ), callback = log_result)
    pool.close()
    pool.join()
    print(result_list)

def column(matrix, i):
    return [row[i] for row in matrix]



if __name__ == '__main__':
    input_list = [line.rstrip('\n').split(' ') for line in open('/home/marcin/ClionProjects/myDig/python_module/inputData')]
    process_lookup(input_list)

    db = database.Psql("credentials.json")
    db.readCredentials()
    db.openConnection()

    hosts = column(input_list, 0)
    #print hosts
    start = time.time()
    print db.insertDomainList(hosts)
    db.commitTransaction()
    end = time.time()
    print end-start
    db.closeConnection()


if __name__ == '__main__':
    #main_scan()
    x = axfrLookup()
    x.dumbDataInt()