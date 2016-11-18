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
import random
import argparse
import os

class scan_input():
    def __init__(self, domain, ns):
        self.domain = domain
        self.ns = ns
    def __repr__(self):
        return self.domain+" "+self.ns

def insert_scan_to_db(returnedVec, domain):
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

def parse_arguments():
    parser = argparse.ArgumentParser(description='Write to file mode', epilog='Hehs')
    parser.add_argument('--output_folder',
                        dest='output',
                        required=True,
                        type=str,
                        help='Defines output folder for iteration results')
    return parser.parse_args()

def get_output_path():
    return './'+parse_arguments().output+'/'

def create_output_folder_if_needed():
    directory = get_output_path()
    if not os.path.exists(directory):
        os.makedirs(directory)

def write_result_to_file(returnedVec, domain):
    create_output_folder_if_needed()
    with open(get_output_path()+str(domain), 'w') as fi:
        fi.write("\n".join(returnedVec))
        
def insert_to_db_if_needed(returnedVec, domain):
    if(len(returnedVec) > 0):
        if re.match(";;", returnedVec[0]):
            logger.Logger().write_tf_log(domain)
        else:
            logger.Logger().write_axfr_log(domain)
            write_result_to_file(returnedVec, domain)
    logger.Logger().write_unknown_log(domain)
    #insert_scan_to_db(returnedVec, domain)
    
def try_to_do_lookup(scan_arguments):
    cmd='/home/mkoi/mgr/bind-9.11.0/bin/dig/dig @'+scan_arguments.ns+' '+scan_arguments.domain+' +short +tries=1 axfr'
    out = run_cmd(cmd)
    print out
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
            print e
            #print "exception, try again"
            #pass
        
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
            print "wtf"
            pass
    def release_all(self):
        while len(self.processes_list) > 0:
            self.release()
        
from time import gmtime, strftime
        
def process_python(in_list):
    i = 0
    a = processes()
    #print len(in_list)
    while i < len(in_list):
        if i %100 == 0:
            print i
        try:
            p = Process(target = process_one_case, args = (in_list[i], ))
            #p = Process(target = try_subprocess, args = (i, ))
            p.start()
            a.push_process(p)
            logger.Logger().increment()
            i += 1
        except Exception as e:
            print e
            #pass
        if i%20 == 0:
            time.sleep(1)
    a.release_all()
import re

def main_scan():
    input_list = [ ]
    for line in open('/home/mkoi/mgr/myDig/python_module/logs/after_logs3.chk'):
        domain = line.rstrip('\n').split('|')[0]
        ns = line.rstrip('\n').split('|')[2]
        if re.match("(\d+).(\d+).(\d+).(\d+)", ns):
            ns_ip = re.match("(\d+).(\d+).(\d+).(\d+)", ns).group()
            input_list.append(scan_input(domain, ns_ip))

    #print input_list
    in_l = random.sample(input_list, 100000)
    print "start: ", strftime("%Y-%m-%d %H:%M:%S", gmtime())
    process_python(in_l)
    print "end: ", strftime("%Y-%m-%d %H:%M:%S", gmtime())
    

if __name__ == '__main__':
    main_scan()
