#include <string>
#include <fstream>
#include <gtest/gtest.h>
#include <glog/logging.h>

#include "storage.h"
#include "kv_schema.h"
#include "endianconv.h"

namespace paradigm4 {
namespace cannon {
namespace test {
class TestStorage : public testing::Test {
public:
    virtual void SetUp();
    virtual void TearDown();

    TestStorage();

protected:
    static std::string _schema_path;
    static std::string _kv_path;
};

std::string TestStorage::_schema_path = "../schema.json";
std::string TestStorage::_kv_path = "../weights";

TestStorage::TestStorage() {
}

void TestStorage::SetUp() {
}

void TestStorage::TearDown() {
}

TEST_F(TestStorage, load_failed_when_path_is_not_directory) {
    KVSchema kv_schema(_schema_path);
    Storage s(kv_schema);
    std::string path = "../not_dir";
    std::ofstream os(path);
    os.close();
    bool r = s.load(path);
    EXPECT_FALSE(r);
}

TEST_F(TestStorage, load_dir_succ) {
    KVSchema kv_schema(_schema_path);
    Storage s(kv_schema);    bool r = s.load(_kv_path);
    EXPECT_TRUE(r);
}

TEST_F(TestStorage, seek_found) {
    KVSchema kv_schema(_schema_path);
    Storage s(kv_schema);
    s.load(_kv_path);
    int64_t key = 1;
    memrev64ifle(&key);
    const byte *value = s.seek((const byte *)(&key));
    EXPECT_NE(value, nullptr);
    byte *copy_value = new byte[kv_schema.get_value_size()];
    memcpy(copy_value, value, kv_schema.get_value_size());
    memrev64ifle(copy_value);
    EXPECT_DOUBLE_EQ(1.2, *((double *)(copy_value)));
    delete []copy_value;
}

TEST_F(TestStorage, seek_not_found) {
    KVSchema kv_schema(_schema_path);
    Storage s(kv_schema);
    s.load(_kv_path);
    int64_t key = 101;
    const byte *value = s.seek((const byte *)(&key));
    EXPECT_EQ(value, nullptr);
}

} // namespace test
} // namespace cannon
} // namespace paradigm4

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    testing::InitGoogleTest(&argc, argv);
    FLAGS_log_dir= "../ut_logs";
    return RUN_ALL_TESTS();
}
