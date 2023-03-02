#!/usr/bin/python
import os
import sys
import re

f = os.popen('ps -ef | grep %s'%sys.argv[1])
for line in f.readlines():
       print line
f.close()	
