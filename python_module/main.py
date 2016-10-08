__author__='marcin'


from ctypes import cdll
from axfrWrapper import axfrVector
import ctypes

lib = cdll.LoadLibrary('/home/mkoi/mgr/myDig/build/lib/libAxfrLib.so')

v = lib.axfrLookup_new()

a = axfrVector(v)
print a

#import database


#hosts = ['www.google.com', 'www.facebook.com', 'www.yahoo.com']
#db = database.Psql("credentials.json")
#db.readCredentials()
#db.openConnection()

#db.insertScan('onet.pl', hosts)
