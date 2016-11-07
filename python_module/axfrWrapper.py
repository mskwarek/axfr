import time
import database
import logger
from axfrVector import *
from multiprocessing import Process, Queue, Lock, Pool
import multiprocessing
import threading
import axfrInterface
import subprocess
import shlex

class scan_input():
    def __init__(self, domain, ns):
        self.domain = domain
        self.ns = ns
    def __repr__(self):
        return self.domain+" "+self.ns
    
def insert_scan_to_db(returnedVec, domain):
    #print returnedVec
    db = database.Psql("/home/mkoi/mgr/myDig/python_module/credentials.json")
    db.readCredentials()
    db.openConnection()
    db.insertScan(domain, returnedVec)
    db.commitTransaction()
    db.closeConnection()

def run_cmd(cmd):
    proc=subprocess.Popen(shlex.split(cmd),stdout=subprocess.PIPE)
    out,err=proc.communicate()
    #print cmd
    return out

def try_subprocess(i):
    cmd = 'mkdir /home/mkoi/tst/dir_'+str(i)
    run_cmd(cmd)

def parse_output(out):
    returnedVec = []
    returnedVec = out.split('\n')
    returnedVec = filter(None, returnedVec)
    return returnedVec

def insert_to_db_if_needed(returnedVec, domain):
    if(len(returnedVec) > 0):
        insert_scan_to_db(returnedVec, domain)
    
def try_to_do_lookup(scan_arguments):
    cmd='/home/mkoi/mgr/bind-9.11.0/bin/dig/dig @'+scan_arguments.ns+' '+scan_arguments.domain+' +short +time=1 axfr'
    out = run_cmd(cmd)
    db_data = parse_output(out)
    insert_to_db_if_needed(db_data, scan_arguments.domain)
    return 0

import sys
    
def process_one_case(input_list):
    success = False
    while not success:
        try:
            try_to_do_lookup(input_list)
            success = True
        except Exception as e:
            #print e
            #print "exception, try again"
            pass
        
class processes():
    def __init__(self):
        self.processes_list = []
    def push_process(self, proc):
        success = False
        while not success:
            try:
                self.processes_list.append(proc)
                success = True
            except:
                self.release()
    def release(self):
        try:
            self.processes_list.pop(0).join()
            #print "join"
        except:
            pass
            
def process_python(in_list):
    i = 0
    a = processes()
    print len(in_list)
    while i < len(in_list):
        print i
        try:
            p = Process(target = process_one_case, args = (in_list[i], ))
            #p = Process(target = try_subprocess, args = (i, ))
            p.start()
            a.push_process(p)
            logger.Logger().increment()
            i += 1
        except:
            print "try to release"
            a.release()

def main_scan():
    input_list = [scan_input(line.rstrip('\n').split(' ')[0], line.rstrip('\n').split(' ')[1]) for line in open('/home/mkoi/mgr/myDig/python_module/inputData')]
    process_python(input_list)

from serve import *    
if __name__ == '__main__':
    main_scan()
