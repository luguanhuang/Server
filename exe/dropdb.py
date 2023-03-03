#!/usr/bin/python
import sys
import os
import MySQLdb

def InputInteger(strTips):
	while True:
		strTemp = raw_input(strTips)
		if strTemp.isdigit():
			return strTemp
		else:
			print("Is not digit")


def QueryDB(host, user, pwd, sql):
        try:   
                conn = MySQLdb.connect(host, user, pwd, "mysql")
                cursor = conn.cursor()
                cursor.execute(sql)
                datas = cursor.fetchall()
                for data in datas:
                        print data[0]

                cursor.close()
                conn.close()
        except MySQLdb.Error,e:
                print "Mysql Error %d: %s" % (e.args[0], e.args[1])
                conn.close()


strServerID = InputInteger("Input your server id:")
strDBname  = "world" + strServerID
strSql = "drop database " + strDBname

QueryDB("10.0.128.229", "root", "", strSql)
print "---------->>>done"
print "press any key to show all databases:"
raw_input()

strSql = "show databases"
QueryDB("10.0.128.229", "root", "", strSql)

print "press any key to exit"
raw_input()