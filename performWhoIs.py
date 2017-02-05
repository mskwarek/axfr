__author__ = 'marcin'

import whois
import datetime
import argparse, pythonwhois, datetime, sys
import json
import socket
import time
import signal

def handler(signum, frame):
    print "Timeout!"
    raise Exception("end of time")

class performWhoIs(object):
    def __init__(self, domain, db):
        self.whoIsQuery = pythonwhois.get_whois(domain)
        self.database = db
        #self.asndb = pyasn.pyasn('/home/marcin/inz/domainTracker/config/ipasn.dat')
    def getWhoIs(self):
        return self.whoIsQuery
    def getRawWhoIs(self):
        return pythonwhois.parse.parse_raw_whois(self.whoIsQuery['raw'], normalized=True)
    
    @staticmethod
    def get_asn(ip_addr):
        try:
            #asndb = pyasn.pyasn('/home/marcin/inz/domainTracker/config/ipasn.dat')
            #ns_asn = asndb.lookup(ip_addr)[0]
            type_ = 'A'
        except:
            ns_asn = 100000
            type_ = 'AAAA'
        return ns_asn, type_
    
    @staticmethod
    def MyParser(json, list_attr):
        to_ret = json
        for attr in list_attr:
            try:
                to_ret = to_ret[attr]
            except:
                to_ret = 'none'
        return to_ret
            
    @staticmethod
    def pwhois(domain):
        signal.signal(signal.SIGALRM, handler)
        signal.alarm(10)
        try:
            data, server_list = pythonwhois.net.get_whois_raw(domain, with_server_list=True)
            print data[0]
            try:
                from collections import OrderedDict
            except ImportError as e:
                from ordereddict import OrderedDict

            def json_fallback(obj):
                if isinstance(obj, datetime.datetime):
                    return obj.isoformat()
                else:
                    return obj

        except Exception, exc:
            print exc
            return
        nameserv = []
        parsed = {}
        try:
            if len(server_list) > 0:
                parsed = pythonwhois.parse.parse_raw_whois(data, normalized=True, never_query_handles=False, handle_server=server_list[-1])
            else:
                parsed = pythonwhois.parse.parse_raw_whois(data, normalized=True)
        
            nameservers = parsed['nameservers']
            print parsed['nameservers']
            for ns in nameservers:
                ip = socket.gethostbyname(ns)
                as_, typ = whoIs.get_asn(ip)
                nameserv.append([ns+".", ip, as_, typ])
        except:
            nameserv.append(["NONAMEXERVER", "0.0.0.0", 300000, "NOTYPE"])
        json_string = json.dumps(parsed, default=json_fallback)
        
        registrar = whoIs.MyParser(parsed, ['registrar'])[0].replace("'", "")
        return registrar
