#!/usr/bin/python
import signal
import time
import sys
import os

ON  = 1
OFF = 0

class ServerCmd:
	def __init__(self, cmd, param):
		self._cmd = cmd
		self._param = param
		self._state = OFF

	def reset(self):
		self._state = OFF

	def setOn(self):
		self._state = ON

	def cmd(self):
		return self._cmd

	def param(self):
		return self._param

	def work(self):
		if self._state == OFF:
			print 'run [' + self._cmd + " " + self._param + ']' + ' at ' + time.ctime()
			pid = os.fork()
			if pid == 0:
				for fd in xrange(1024):
					try:
						os.close(fd)
					except OSError:
						pass
				os.open(os.devnull, os.O_RDWR)
				os.dup2(0,1)
				os.dup2(0,2)
				os.execl(self._cmd, self._cmd, self._param)
			return 1
		else:
			return 0
			

cmddict = [ 
	ServerCmd("./webserver", "150"), 
	ServerCmd("./gmserver", "200"), 
	ServerCmd("./loginserver", "100"), 
	ServerCmd("./logserver", "1-1"), 
	ServerCmd("./dbserver", "1-1"), 
	ServerCmd("./gameserver", "1-1"), 
	ServerCmd("./gateserver", "1-1"), 
]

CHECK_USER = "publish"
CHDIR = "/home/publish/code/server/Build/Debug/bin"

def CheckProcess():
	for i in cmddict: i.reset()

	f = os.popen("ps -ef | grep server")
	for line in f.readlines():
		process = line.split()
		USER = process[0]
		PID = process[1]
		EXE = process[7]
		PARAM1 = ""
		if len(process) >= 9:
			PARAM1 = process[8]

		if len(CHECK_USER) > 0:
			if USER != CHECK_USER:
				continue
		if (EXE == 'ch') or (EXE == 'grep'):
				continue

		for i in cmddict:
			if EXE == i.cmd() and PARAM1 == i.param():
				i.setOn()
				#print 'found ' + i.cmd() + ' is on'
	f.close()

	ret = 0
	for i in cmddict: ret += i.work()

	if ret > 0:
		while True:
			try:
				wt = os.waitpid(-1, os.WNOHANG)
				if wt[0] == 0:
					break
			except OSError:
				break

	return ret

def DoCommand(buffer):
	global cmddict
	cmds = buffer.split()
	if len(cmds) == 0:
		return

	cmd = cmds[0].lower()
	n = len(cmds)
	if cmd == "add":
		if n == 3:
			cmddict.append(ServerCmd(cmds[1], cmds[2])) 
			print 'add ok'
		else:
			print 'add cmd param'
	elif cmd == "list":
		k = 0
		for i in cmddict:
			print k, i.cmd(), i.param()
			k += 1
		print 'total %d commands'%len(cmddict)
	elif cmd == 'remove':
		if n == 2:
			i = int(cmds[1])
			if len(cmddict) > i:
				print 'remove [%s %s] ok'%(cmddict[i].cmd(), cmddict[i].param())
				del cmddict[i]
			else:
				print 'index out of range %d/%d'(i,len(cmddict))
		else:
			print 'remove index'
	elif cmd == 'clear':
		cmddict = []
		print 'clear'
	else:
		print '-----[ usage ]-----'
		print 'add cmd param'
		print 'remove index'
		print 'list'
		print 'clear'
		print '------------------'


class Tick:
	def __init__(self, symbols):
		self.symbols = symbols
		self.symbollen = len(symbols)
		self.sid = 0

	def foward(self):
		sys.stdout.write(' ')
	
	def backward(self):
		sys.stdout.write('\b')

	def draw(self):
		sys.stdout.write(self.symbols[self.sid])
		sys.stdout.flush()
		self.sid = (self.sid + 1)%self.symbollen


if __name__ == "__main__":

	print 'mkfifo guard.fifo'
	try:
		os.mkfifo("guard.fifo")
	except OSError as err:
		pass
	fifo = os.open("guard.fifo", os.O_NONBLOCK|os.O_RDONLY)

	Count = 0
	os.chdir(CHDIR)
	tk = Tick('-\|/')
	tk.foward()
	while True:

		tk.backward()
		tk.draw()

		try:
			buffer = os.read(fifo, 512)
		except OSError as err:
			if err.errno == errno.EAGIN or err.errno == errno.EWOULDBLOCK:
				buffer = None
			else:
				raise
		
		if buffer:
			DoCommand(buffer)

		Count += 1
		if Count == 5:
			Count = 0
			if CheckProcess() > 0:
				tk.foward()
		
		time.sleep(1)
