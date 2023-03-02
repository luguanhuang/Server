#!/usr/bin/python
import os
import sys

#while i <= 26:
#database = "world" + str(i);
#print database
i = 1
while i <= 1000:
	os.system('mysql -u root -e "insert into world3.account values(%d, -1, null, null, null, null, 123, 0, 0)"'%i)
	i = i + 1
