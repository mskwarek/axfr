import time
import database
import logger
#from axfrVector import *
from multiprocessing import Process, Queue, Lock, Pool
import multiprocessing
import threading
import axfrInterface
import subprocess
import shlex
import random
import argparse
import os
import traceback

class scan_input():
    def __init__(self, domain, ns):
        self.domain = domain
        self.ns = ns
    def __repr__(self):
        return self.domain+" "+self.ns

def insert_scan_to_db(returnedVec, domain):
    db = database.Psql(parse_arguments().rootdir+"/python_module/credentials.json")
    db.readCredentials()
    db.openConnection()
    db.insertScan(domain, returnedVec)
    db.commitTransaction()
    db.closeConnection()

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
    parser.add_argument('--input_list',
                        dest='inp',
                        required=True,
                        type=str,
                        help='path to file with input data (domain|ns list)')

    parser.add_argument('--root_project_dir',
                        dest='rootdir',
                        required=True,
                        type=str,
                        help='Path to root project dir')
    return parser.parse_args()


def insert_to_db_if_needed(writeUtils, returnedVec, domain):
    if(len(returnedVec) > 0):
        if re.match(";; TF", returnedVec[0]):
            logger.Logger().write_tf_log(writeUtils.get_log_output_path(), domain)
        elif re.match(";; TO", returnedVec[0]):
            logger.Logger().write_to_log(writeUtils.get_log_output_path(), domain)
        else:
            logger.Logger().write_axfr_log(writeUtils.get_log_output_path(), domain)
            writeUtils.write_result_to_file(returnedVec, domain)
        return
    logger.Logger().write_unknown_log(writeUtils.get_log_output_path(), domain)
    #insert_scan_to_db(returnedVec, domain)


class Command(object):
    def __init__(self, cmd):
        self.cmd = cmd
        self.process = None
        
    def run(self):
        def run_cmd():
            self.process=subprocess.Popen(shlex.split(self.cmd),stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
            self.out,err=self.process.communicate()
            errcode = self.process.returncode
            suc = False
            while not suc:
                proc = self.process.poll()
                if proc is not None:
                    suc = True
                else:
                    print "still running"

        run_cmd()
        return self.out
def try_to_do_lookup(scan_arguments, write_utils):
    cmd=str(parse_arguments().rootdir)+'/bind-9.11.0/bin/dig/dig @'+scan_arguments.ns+' '+scan_arguments.domain+' +short +tries=1 axfr'
    #print cmd
    
    command = Command(cmd)
    out = command.run()
    db_data = parse_output(out)
    #print db_data
    insert_to_db_if_needed(write_utils, db_data, scan_arguments.domain)
    return 0

import sys
    
def process_one_case(input_list, write_utils):
    success = False
    timeout = 0
    while not success:
        try:
            try_to_do_lookup(input_list, write_utils)
            success = True
        except Exception as e:
            print e
            #traceback.print_exc()
            timeout+=1
            if timeout > 20:
                return
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
            self.processes_list.pop(0).join(timeout=20)
            #print "join"
        except:
            print "wtf"
            pass
    def release_all(self):
        while len(self.processes_list) > 0:
            self.release()
        
from time import gmtime, strftime
import utils

def process_python(in_list):
    i = 0
    timeout = 0
    a = processes()
    #print len(in_list)
    while i < len(in_list):
        write_utils = utils.WriteUtils(parse_arguments().output)
        if i %100 == 0:
            print i
        try:
            p = Process(target = process_one_case, args = (in_list[i], write_utils, ))
            #p = Process(target = try_subprocess, args = (i, ))
            p.start()
            a.push_process(p)
            #logger.Logger().increment()
            i += 1
        except Exception as e:
            timeout += 1
            if timeout > 4:
                i+=1
            #print e
            #pass
    a.release_all()
import re

def main_scan():
    input_list = [ ]
    for line in open(parse_arguments().inp):
        domain = line.rstrip('\n').split('|')[0]
        ns = line.rstrip('\n').split('|')[2]
        if re.match("(\d+).(\d+).(\d+).(\d+)", ns):
            ns_ip = re.match("(\d+).(\d+).(\d+).(\d+)", ns).group()
            input_list.append(scan_input(domain, ns_ip))

    #print input_list
    #in_l = random.sample(input_list, 10000)
    print "start: ", strftime("%Y-%m-%d %H:%M:%S", gmtime())
    process_python(input_list)
    print "end: ", strftime("%Y-%m-%d %H:%M:%S", gmtime())
    

if __name__ == '__main__':
    main_scan()
