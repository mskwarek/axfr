__author__ = 'marcin'

import time, datetime
import psycopg2
import json

class Psql(object):
    def __init__(self, cred_file_path):
        self.credentialsFile = cred_file_path
    def readCredentials(self):
        with open(self.credentialsFile) as credential_file:
           credentials = json.load(credential_file)
        self.host = credentials['host']
        self.dbname = credentials['dbname']
        self.user = credentials['user']
        self.password = credentials['password']
        print self.host, self.dbname, self.user, self.password
    def openConnection(self):
        self.conn = psycopg2.connect("host='"+self.host+"' dbname='"+dbname+"' user ='"+user+"' password='"+password+"'")
        self.cursor = self.conn.cursor()
