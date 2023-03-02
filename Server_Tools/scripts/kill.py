#!/usr/bin/python
import os
import sys
import re

if len(sys.argv) != 2:
	print sys.argv[0], "prcess"
else:
	processes = [] 
	f = os.popen('ps -ef | grep %s'%sys.argv[1])
	for line in f.readlines():
		params = line.split()
		#print params	
		if (params[0] != 'publish'):
			continue
		if (re.search(sys.argv[1], params[7])):
			print params[7] + ':' + params[1]
			processes.append(params[1])
	#print processes
	f.close()	
	for id in processes:
		print id
		os.system('kill -9 %s'%id)
