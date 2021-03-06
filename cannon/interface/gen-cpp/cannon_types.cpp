/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "cannon_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace paradigm4 { namespace cannon {

int _kItemStatusValues[] = {
  ItemStatus::FOUND,
  ItemStatus::NOT_FOUND,
  ItemStatus::ERROR
};
const char* _kItemStatusNames[] = {
  "FOUND",
  "NOT_FOUND",
  "ERROR"
};
const std::map<int, const char*> _ItemStatus_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kItemStatusValues, _kItemStatusNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kStatusValues[] = {
  Status::OK,
  Status::PARTIAL_OK,
  Status::ERROR
};
const char* _kStatusNames[] = {
  "OK",
  "PARTIAL_OK",
  "ERROR"
};
const std::map<int, const char*> _Status_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kStatusValues, _kStatusNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


CannonRequest::~CannonRequest() throw() {
}


void CannonRequest::__set_keys(const std::string& val) {
  this->keys = val;
}

uint32_t CannonRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->keys);
          this->__isset.keys = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t CannonRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("CannonRequest");

  xfer += oprot->writeFieldBegin("keys", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeBinary(this->keys);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(CannonRequest &a, CannonRequest &b) {
  using ::std::swap;
  swap(a.keys, b.keys);
  swap(a.__isset, b.__isset);
}

CannonRequest::CannonRequest(const CannonRequest& other0) {
  keys = other0.keys;
  __isset = other0.__isset;
}
CannonRequest& CannonRequest::operator=(const CannonRequest& other1) {
  keys = other1.keys;
  __isset = other1.__isset;
  return *this;
}
void CannonRequest::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "CannonRequest(";
  out << "keys=" << to_string(keys);
  out << ")";
}


CannonResponse::~CannonResponse() throw() {
}


void CannonResponse::__set_status(const Status::type val) {
  this->status = val;
}

void CannonResponse::__set_valueStatuses(const std::vector<ItemStatus::type> & val) {
  this->valueStatuses = val;
}

void CannonResponse::__set_values(const std::string& val) {
  this->values = val;
}

uint32_t CannonResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast2;
          xfer += iprot->readI32(ecast2);
          this->status = (Status::type)ecast2;
          this->__isset.status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->valueStatuses.clear();
            uint32_t _size3;
            ::apache::thrift::protocol::TType _etype6;
            xfer += iprot->readListBegin(_etype6, _size3);
            this->valueStatuses.resize(_size3);
            uint32_t _i7;
            for (_i7 = 0; _i7 < _size3; ++_i7)
            {
              int32_t ecast8;
              xfer += iprot->readI32(ecast8);
              this->valueStatuses[_i7] = (ItemStatus::type)ecast8;
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.valueStatuses = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->values);
          this->__isset.values = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t CannonResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("CannonResponse");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->status);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("valueStatuses", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I32, static_cast<uint32_t>(this->valueStatuses.size()));
    std::vector<ItemStatus::type> ::const_iterator _iter9;
    for (_iter9 = this->valueStatuses.begin(); _iter9 != this->valueStatuses.end(); ++_iter9)
    {
      xfer += oprot->writeI32((int32_t)(*_iter9));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("values", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeBinary(this->values);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(CannonResponse &a, CannonResponse &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.valueStatuses, b.valueStatuses);
  swap(a.values, b.values);
  swap(a.__isset, b.__isset);
}

CannonResponse::CannonResponse(const CannonResponse& other10) {
  status = other10.status;
  valueStatuses = other10.valueStatuses;
  values = other10.values;
  __isset = other10.__isset;
}
CannonResponse& CannonResponse::operator=(const CannonResponse& other11) {
  status = other11.status;
  valueStatuses = other11.valueStatuses;
  values = other11.values;
  __isset = other11.__isset;
  return *this;
}
void CannonResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "CannonResponse(";
  out << "status=" << to_string(status);
  out << ", " << "valueStatuses=" << to_string(valueStatuses);
  out << ", " << "values=" << to_string(values);
  out << ")";
}

}} // namespace
