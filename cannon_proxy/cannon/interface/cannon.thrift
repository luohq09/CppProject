namespace cpp paradigm4.cannon
namespace java paradigm4.cannon
namespace py paradigm4.cannon

enum ItemStatus {
    FOUND,
    NOT_FOUND,
    ERROR
}

enum Status {
    OK,
    PARTIAL_OK,
    ERROR
}

struct ValueItem {
    1: ItemStatus status,
    2: double value
}

struct CannonResponse {
    1: Status status,
    2: list<ValueItem> values
}

struct CannonKV {
    1: i64 key,
    2: double value
}

service Cannon {
    CannonResponse get(1: list<i64> keys, 2: i32 data_version);
    bool insert(1: list<CannonKV> kvs, 2: i32 data_version);
    bool clear_data(1: i32 data_version);
}
