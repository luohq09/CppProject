import sys
sys.path.append('../../interface/gen-py')

import time

from paradigm4.cannon_proxy import Cannon_Service
from paradigm4.cannon_proxy.ttypes import *
from paradigm4.cannon_proxy.constants import *

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

__author__ = 'luohuaqing'

try:
    transport = TSocket.TSocket('localhost', 9998)
    transport = TTransport.TBufferedTransport(transport)
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    client = Cannon_Service.Client(protocol)
    transport.open()
    while True:
            req = CannonRequest([1, 2, 3, 4])
            rsp = client.get(req)
            print rsp
            time.sleep(2)
    transport.close()

except Thrift.TException, tx:
    print "%s" % tx.message
