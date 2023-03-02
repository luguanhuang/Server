#!/usr/bin/python
import os
import json
import select
import subprocess
import threading
import Queue
from socket import *

class SendDataThread(threading.Thread):
	def __init__(self, queue, fd, addr):
		threading.Thread.__init__(self)
		self.queue = queue
		self.fd = fd
		self.addr = addr
		self.count = 0
		self.setDaemon(True)

	def run(self):
		output = {'data':None, 'cmd':'message'}
		while True:
			info = self.queue.get()
			self.queue.task_done()
			#print info
			if info[0] == 'data':
				#print 'send data thread read data'
				output['data'] = info[1]
				self.fd.sendto(json.dumps(output), self.addr)
			elif info[0] == 'end':
				#print 'send data thread read end'
				self.count += 1
				if self.count == 2:
					break
		#print 'send data thread stoped'
		output['data'] = ''
		self.fd.sendto(json.dumps(output), self.addr)
				

class ReadOutputThread(threading.Thread):
	def __init__(self, fd, queue):
		threading.Thread.__init__(self)
		self.fd = fd
		self.queue = queue
		self.setDaemon(True)

	def run(self):
		while True:
			try:
				data = self.fd.readline()
			except:
				break
			if not data: break
			self.queue.put(('data', data.strip()))
		try:
			self.fd.close()
		except:
			pass
		self.queue.put(('end', ''))
		#print 'read output thread stoped'


def pipeoutput(cmd, addr, fd, dir):
	if dir:
		print 'enter dir', dir
		os.chdir(dir)
	output = {'data':' '.join(cmd), 'cmd':'info', 'dir':dir}
	print output
	fd.sendto(json.dumps(output), addr)
	print 'start subprocess with cmd', ' '.join(cmd)
	p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	#print 'stdout, stderr = ', p.stdout, p.stderr
	q = Queue.Queue()
	t1 = ReadOutputThread(p.stdout, q)
	t2 = ReadOutputThread(p.stderr, q)
	t3 = SendDataThread(q, fd, addr)
	t1.start()
	t2.start()
	t3.start()
	p.wait()
	q.join()
	output = {'data':'execute command [' + ' '.join(cmd) + '] ', 'cmd':'info'}
	if p.returncode == 0:
		output['data'] += "success"
	else:
		output['data'] += "failed with return code: %d" % p.returncode
	fd.sendto(json.dumps(output), addr)
	return p.returncode

def process(data, addr, fd):
	print data
	info = json.loads(data)
	if 'cmd' not in info:
		print 'request', data, 'not found cmd'
	else:
		try:
			if info['cmd'] == 'tar':
				pipeoutput(['/bin/bash', 'mkdist.sh'], addr, fd, '/home/QA')
			elif info['cmd'] == 'tasks':
				for task in info['tasks']:
					dir = ''
					if 'dir' in task:
						dir = task['dir']
					pipeoutput(task['task'], addr, fd, dir)
			elif info['cmd'] == 'run':
				dir = ''
				if 'dir' in info:
					dir = info['dir']
				pipeoutput(info['task'], addr, fd, dir)
		except Exception as e:
			output = {'data':'exception: ' + str(e), 'cmd':'info'}
			fd.sendto(json.dumps(output), addr)

fd = socket(AF_INET, SOCK_DGRAM)
fd.bind(('0.0.0.0', 31500))

while True:
	data, addr = fd.recvfrom(8192)
	#print addr
	if data:
		process(data, addr, fd)

fd.close()


