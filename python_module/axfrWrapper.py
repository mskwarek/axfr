from ctypes import cdll
import time 
lib = cdll.LoadLibrary('/home/marcin/ClionProjects/myDig/build/lib/libAxfrLib.so')

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

if __name__ == '__main__':
    input_list = [line.rstrip('\n').split(' ') for line in open('/home/marcin/ClionProjects/myDig/python_module/inputData')]
    process_lookup(input_list)
