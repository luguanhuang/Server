#!/usr/bin/python
import os
import sys

if len(sys.argv) != 2:
	print sys.argv[0], "databaseName"
else:
	os.system('mysql -u root -e "update %s.account set role1 = \'\', role2=\'\', role3=\'\', role4=\'\', lastselect=-1"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.role"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.rolesummary"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.ranklist"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.globalconfig"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.guild"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.reward"'%sys.argv[1])
	os.system('mysql -u root -e "delete from %s.social"'%sys.argv[1])
	os.system('mysql -u root -e "select count(*) from %s.account where role1 != \'\'"'%sys.argv[1])
