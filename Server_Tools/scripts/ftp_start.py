#!/usr/bin/python

from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer

authorizer = DummyAuthorizer()
authorizer.add_anonymous("/home/publish/", perm='elradfmw')

handler = FTPHandler
handler.authorizer = authorizer

server = FTPServer(("10.0.128.229", 21210), handler)
server.serve_forever()

