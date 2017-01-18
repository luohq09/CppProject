namespace cpp paradigm4.cannon 
namespace java com._4paradigm.cannon

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
    1: binary keys
}

struct CannonResponse {
    1: Status status,
    2: list<ItemStatus> valueStatuses,
    3: binary values
}

service Cannon {
    CannonResponse get(1:CannonRequest req)
}
