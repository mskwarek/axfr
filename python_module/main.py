__author__='marcin'

import database

db = database.Psql("credentials.json")
db.readCredentials()
db.openConnection()

hosts = ['www.google.com', 'www.facebook.com', 'www.yahoo.com']
