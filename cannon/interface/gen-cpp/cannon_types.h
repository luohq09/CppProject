/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef cannon_TYPES_H
#define cannon_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace paradigm4 { namespace cannon {

struct ItemStatus {
  enum type {
    FOUND = 0,
    NOT_FOUND = 1,
    ERROR = 2
  };
};

extern const std::map<int, const char*> _ItemStatus_VALUES_TO_NAMES;

struct Status {
  enum type {
    OK = 0,
    PARTIAL_OK = 1,
    ERROR = 2
  };
};

extern const std::map<int, const char*> _Status_VALUES_TO_NAMES;

class CannonRequest;

class CannonResponse;

typedef struct _CannonRequest__isset {
  _CannonRequest__isset() : keys(false) {}
  bool keys :1;
} _CannonRequest__isset;

class CannonRequest {
 public:

  CannonRequest(const CannonRequest&);
  CannonRequest& operator=(const CannonRequest&);
  CannonRequest() : keys() {
  }

  virtual ~CannonRequest() throw();
  std::string keys;

  _CannonRequest__isset __isset;

  void __set_keys(const std::string& val);

  bool operator == (const CannonRequest & rhs) const
  {
    if (!(keys == rhs.keys))
      return false;
    return true;
  }
  bool operator != (const CannonRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CannonRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CannonRequest &a, CannonRequest &b);

inline std::ostream& operator<<(std::ostream& out, const CannonRequest& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _CannonResponse__isset {
  _CannonResponse__isset() : status(false), valueStatuses(false), values(false) {}
  bool status :1;
  bool valueStatuses :1;
  bool values :1;
} _CannonResponse__isset;

class CannonResponse {
 public:

  CannonResponse(const CannonResponse&);
  CannonResponse& operator=(const CannonResponse&);
  CannonResponse() : status((Status::type)0), values() {
  }

  virtual ~CannonResponse() throw();
  Status::type status;
  std::vector<ItemStatus::type>  valueStatuses;
  std::string values;

  _CannonResponse__isset __isset;

  void __set_status(const Status::type val);

  void __set_valueStatuses(const std::vector<ItemStatus::type> & val);

  void __set_values(const std::string& val);

  bool operator == (const CannonResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(valueStatuses == rhs.valueStatuses))
      return false;
    if (!(values == rhs.values))
      return false;
    return true;
  }
  bool operator != (const CannonResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CannonResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CannonResponse &a, CannonResponse &b);

inline std::ostream& operator<<(std::ostream& out, const CannonResponse& obj)
{
  obj.printTo(out);
  return out;
}

}} // namespace

#endif