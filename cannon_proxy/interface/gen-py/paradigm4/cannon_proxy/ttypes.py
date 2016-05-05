#
# Autogenerated by Thrift Compiler (0.9.3)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TException, TApplicationException

from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol, TProtocol
try:
  from thrift.protocol import fastbinary
except:
  fastbinary = None


class ItemStatus:
  FOUND = 0
  NOT_FOUND = 1
  ERROR = 2

  _VALUES_TO_NAMES = {
    0: "FOUND",
    1: "NOT_FOUND",
    2: "ERROR",
  }

  _NAMES_TO_VALUES = {
    "FOUND": 0,
    "NOT_FOUND": 1,
    "ERROR": 2,
  }

class Status:
  OK = 0
  PARTIAL_OK = 1
  ERROR = 2

  _VALUES_TO_NAMES = {
    0: "OK",
    1: "PARTIAL_OK",
    2: "ERROR",
  }

  _NAMES_TO_VALUES = {
    "OK": 0,
    "PARTIAL_OK": 1,
    "ERROR": 2,
  }


class CannonRequest:
  """
  Attributes:
   - keys
  """

  thrift_spec = (
    None, # 0
    (1, TType.LIST, 'keys', (TType.I64,None), None, ), # 1
  )

  def __init__(self, keys=None,):
    self.keys = keys

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.LIST:
          self.keys = []
          (_etype3, _size0) = iprot.readListBegin()
          for _i4 in xrange(_size0):
            _elem5 = iprot.readI64()
            self.keys.append(_elem5)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('CannonRequest')
    if self.keys is not None:
      oprot.writeFieldBegin('keys', TType.LIST, 1)
      oprot.writeListBegin(TType.I64, len(self.keys))
      for iter6 in self.keys:
        oprot.writeI64(iter6)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.keys)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class ValueItem:
  """
  Attributes:
   - status
   - value
  """

  thrift_spec = (
    None, # 0
    (1, TType.I32, 'status', None, None, ), # 1
    (2, TType.DOUBLE, 'value', None, None, ), # 2
  )

  def __init__(self, status=None, value=None,):
    self.status = status
    self.value = value

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.I32:
          self.status = iprot.readI32()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.DOUBLE:
          self.value = iprot.readDouble()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('ValueItem')
    if self.status is not None:
      oprot.writeFieldBegin('status', TType.I32, 1)
      oprot.writeI32(self.status)
      oprot.writeFieldEnd()
    if self.value is not None:
      oprot.writeFieldBegin('value', TType.DOUBLE, 2)
      oprot.writeDouble(self.value)
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.status)
    value = (value * 31) ^ hash(self.value)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class CannonResponse:
  """
  Attributes:
   - status
   - values
  """

  thrift_spec = (
    None, # 0
    (1, TType.I32, 'status', None, None, ), # 1
    (2, TType.LIST, 'values', (TType.STRUCT,(ValueItem, ValueItem.thrift_spec)), None, ), # 2
  )

  def __init__(self, status=None, values=None,):
    self.status = status
    self.values = values

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.I32:
          self.status = iprot.readI32()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.LIST:
          self.values = []
          (_etype10, _size7) = iprot.readListBegin()
          for _i11 in xrange(_size7):
            _elem12 = ValueItem()
            _elem12.read(iprot)
            self.values.append(_elem12)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('CannonResponse')
    if self.status is not None:
      oprot.writeFieldBegin('status', TType.I32, 1)
      oprot.writeI32(self.status)
      oprot.writeFieldEnd()
    if self.values is not None:
      oprot.writeFieldBegin('values', TType.LIST, 2)
      oprot.writeListBegin(TType.STRUCT, len(self.values))
      for iter13 in self.values:
        iter13.write(oprot)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.status)
    value = (value * 31) ^ hash(self.values)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)