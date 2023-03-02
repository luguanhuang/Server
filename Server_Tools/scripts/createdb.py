#!/usr/bin/python
import sys
import os

#sys.argv.append("25")

if len(sys.argv) != 2:
	print 'createdb.py serverID'
	exit(1)

f = open("dbconf/dbinit.sql")
sqlData = f.read()
f.close()

sqls = sqlData.replace("\r\n", '').split(';')
sid = int(sys.argv[1])
srid = "%d"%(sid << 32)

for sql in sqls:
	sql = sql.replace("{sid}", sys.argv[1]).replace("{srid}", srid)
	print sql+";"
	os.system('mysql -u root -e "%s"'%sql)
