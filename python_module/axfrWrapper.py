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

def perform_lookup(in_list):
    x = axfrLookup()
    print "dziala", in_list[0], in_list[1]
    x.getResult(in_list[0], in_list[1])
    x.destroy()
    return 0
    
processes = []    
def start_new_process(domain, ns):
    p = Process(target=perform_lookup, args=(domain, ns))    
    p.start()
    processes.append(p)

from multiprocessing import Process, Queue, Lock
import multiprocessing
import threading

num_processes = 64
my_list = [ 'XYZ', 'ABC', 'NYU' ]

threads = []


def clean_up():
    print "cleanup"
    for thread in threads:
        if not thread.isAlive():
            threads.remove(thread)

# run until all the threads are done, and there is no data left
def process_lookup(my_list):
    while threads or my_list:
    
        # if we aren't using all the processors AND there is still data left to
        # compute, then spawn another thread
        if (len(threads) < num_processes) and my_list:
            item_to_process =  my_list.pop()
            t = threading.Thread(target=perform_lookup, args=[item_to_process ])
            t.setDaemon(True)
            print item_to_process
            try:
                t.start()
                threads.append(t)
            except:
                clean_up()
            # in the case that we have the maximum number of threads check if any of them
            # are done. (also do this when we run out of data, until all the threads are done)
        else:
            clean_up()
                
if __name__ == '__main__':
    input_list = [line.rstrip('\n').split(' ') for line in open('inputData')]
    process_lookup(input_list)


'''

    pool = multiprocessing.Pool(2)
    results = []
    r = pool.map_async(perform_lookup, input_list, callback=results.append)
    r.wait()
    print results


    for i in input_list:
        print j, " of ", len(input_list), len(processes)
        j+=1
        while True:
            try:
                start_new_process(i[0], i[1])
                break
            except:
                print "wait"
                time.sleep(1)
   '''     
            #p.join()
        #p.join() # this blocks until the process terminates
