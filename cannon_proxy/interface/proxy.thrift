namespace cpp paradigm4.cannon_proxy
namespace java paradigm4.cannon_proxy
namespace py paradigm4.cannon_proxy

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

struct CannonRequest {
    1: list<i64> keys
}

struct ValueItem {
    1: ItemStatus status,
    2: double value
}

struct CannonResponse {
    1: Status status,
    2: list<ValueItem> values
}

service Cannon_Service {
    string version();
    bool reload(1: list<string> paths);
    CannonResponse get(1: CannonRequest req);
}
