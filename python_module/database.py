__author__ = 'marcin'

import time, datetime
import psycopg2
import json
from time import gmtime, strftime

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
        #print self.host, self.dbname, self.user, self.password

    def openConnection(self):
        self.conn = psycopg2.connect("host='"+self.host+"' dbname='"+self.dbname+"' user ='"+self.user+"' password='"+self.password+"'")
        self.cursor = self.conn.cursor()

    def executeQuery(self, query):
        self.cursor.execute(query)
        return self.cursor.fetchall()

    def commitTransaction(self):
        self.conn.commit()

    def closeConnection(self):
        self.cursor.close()
        self.conn.close()

    def insertScan(self, root_domain, domain_list):
        insert_scan_query='SELECT * FROM v.ins_scan(\'%s\', \'%s\')'
        domain_list = str(map(str, domain_list))
        domain_list = domain_list.replace('[', '{').replace(']', '}').replace('\"', '//').replace('\'', '\"')
        query = insert_scan_query%(root_domain, domain_list)
        #print query
        #with open('db_script'+str(time), 'w') as fi:
        #    fi.write(query)
            
        x = self.executeQuery(query)
        #print x
    def insertDomainList(self, domain_list):
        insert_domain_array_query ='SELECT * FROM v.get_id_of_domains(\'%s\')'
        domain_list = str(map(str, domain_list))
        domain_list = domain_list.replace('[', '{').replace(']', '}').replace('\'', '\"')
        time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
        query = insert_domain_array_query%(domain_list)
        
        x = self.executeQuery(query)
        print x
