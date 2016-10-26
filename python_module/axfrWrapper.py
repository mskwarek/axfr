from ctypes import cdll
import ctypes
import time
import database


class Vector(object):
    charptr = ctypes.POINTER(ctypes.c_char)
    lib = cdll.LoadLibrary('/home/mkoi/mgr/myDig/build/lib/libAxfrLib.so')
    lib.axfrLookup_getDumbVector.restype = ctypes.c_void_p
    lib.axfrLookup_getDumbVector.argtypes = []
    lib.axfrLookup_getDumbSize.restype = ctypes.c_int
    lib.axfrLookup_getDumbSize.argtypes = [ctypes.c_void_p]
    lib.axfrLookup_get_to_return.restype = ctypes.c_char_p
    lib.axfrLookup_get_to_return.argtypes = [ctypes.c_void_p, ctypes.c_int]
    lib.axfrLookup_getSizeOfReturnedData.restype = ctypes.c_int
    lib.axfrLookup_getSizeOfReturnedData.argtypes = [ctypes.c_void_p]
    lib.axfrLookup_getReturnedDomain.restype = ctypes.c_char_p
    lib.axfrLookup_getReturnedDomain.argtypes = [ctypes.c_void_p, ctypes.c_int]
    lib.axfrLookup_getScannedDomainName.restype = ctypes.c_char_p
    lib.axfrLookup_getScannedDomainName.argtypes = [ctypes.c_void_p]

    def __init__(self):
        self.vector = Vector.lib.axfrLookup_getDumbVector()
        Vector.lib.axfrLookup_fillWithData(self.vector)
    def __getitem__(self, i):
        if 0 <= i < len(self):
            return self.getitem_cb(i)
        raise IndexError('Vector index out of range')
    def __len__(self):
        return self.getlen_cb()
    def __repr__(self):
        return '[{}]'.format("', '".join(self[i] for i in range(len(self))))

    def getlen_cb(self):
        raise NotImplementedError
    def getitem_cb(self, i):
        return NotImplementedError

class axfrVector(Vector):

    def __init__(self, lookup):
        self.lookup = lookup
    def getlen_cb(self):
        return Vector.lib.axfrLookup_getSizeOfReturnedData(self.lookup)
    def getitem_cb(self, i):
        return Vector.lib.axfrLookup_getReturnedDomain(self.lookup, i)
    
    
class axfrLookup(object):
    lib = cdll.LoadLibrary('/home/mkoi/mgr/myDig/build/lib/libAxfrLib.so')
    def getDomainName(self):
        return Vector.lib.axfrLookup_getScannedDomainName(self.obj)
    def __init__(self):
        self.obj = self.lib.axfrLookup_new()
    def performLookup(self, domain, ns):
        self.lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "udaoo sie"
    def getResult(self, domain, ns):
        self.lib.axfrLookup_performLookup(self.obj, domain, ns)
        print "after perform"
        self.returnedVec = axfrVector(self.obj)
        self.domainName = self.getDomainName()
    def insertDataToDb(self, db):
        db.insertScan(self.domainName, self.returnedVec)
        db.commitTransaction()

    def destroy(self):
        self.lib.axfrLookup_destroy(self.obj)

            
def perform_lookup(in_list):
    x = axfrLookup()
    print "dziala", in_list[0], in_list[1]
    x.getResult(in_list[0], in_list[1])
    if(len(x.returnedVec) > 0):
        db = database.Psql("/home/mkoi/mgr/myDig/python_module/credentials.json")
        db.readCredentials()
        db.openConnection()
        x.insertDataToDb(db)
        db.closeConnection()
    x.destroy()

    
def perform_lookup_and_print(in_list):
    x = axfrLookup()
    print "dziala", in_list[0], in_list[1]
    x.getResult(in_list[0], in_list[1])
    if(len(x.returnedVec) > 0):
        print x
    x.destroy()
from multiprocessing import Process, Queue, Lock
import multiprocessing
import threading
    
def process_lookup(input_list):
    for i in range(len(input_list)):
        try:
            p = Process(target=perform_lookup, args=(input_list[i], ))
            p.start()
            p.join()
        except:
            time.sleep(5)
        i+=1

def process_lookup_and_print(input_list):
    for i in range(len(input_list)):
        try:
            p = Process(target=perform_lookup_and_print, args=(input_list[i], ))
            p.start()
            p.join()
        except:
            time.sleep(5)
        i+=1
        
def main_scan():
    input_list = [line.rstrip('\n').split(' ') for line in open('/home/mkoi/mgr/myDig/python_module/inputData')]
    process_lookup(input_list)

    
if __name__ == '__main__':
    input_list = [line.rstrip('\n').split(' ') for line in open('/home/mkoi/mgr/myDig/python_module/inputData')]
    print "DUPA"
    process_lookup_and_print(input_list)
    for i in range(1, 100):
        process_lookup_and_print([['verizonwireless.com.', 'ns-dbln.verizonwireless.com.']])

    #x = axfrLookup()
    #x.dumbDataInt()
    
