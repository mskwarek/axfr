__author__='marcin'


import database


hosts = ['www.google.com', 'www.facebook.com', 'www.yahoo.com']
db = database.Psql("credentials.json")
db.readCredentials()
db.openConnection()
print 'onet.pl', hosts
db.insertScan('onet.pl', hosts)

db.commitTransaction()