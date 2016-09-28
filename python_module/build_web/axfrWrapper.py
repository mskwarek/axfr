from ctypes import cdll
import time 
import database

lib = cdll.LoadLibrary('/home/mkoi/mgr/myDig/build/lib/libAxfrLib.so')

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
    for i in range(len(input_list)):
        pool = multiprocessing.Pool(None)
        #print i
        try:
            pool.apply_async(perform_lookup, args = (input_list[i], ), callback = log_result)
            i+=1
        except:
            time.sleep(5)
            continue
    pool.close()
    pool.join()


def column(matrix, i):
    return [row[i] for row in matrix]



if __name__ == '__main__':
    input_list = [line.rstrip('\n').split(' ') for line in open('/home/mkoi/mgr/myDig/python_module/inputData')]
    #process_lookup(input_list)

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
