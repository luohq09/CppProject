#include <string>
#include <vector>
#include <assert.h>

using namespace std;

string stringMultiply(const string &s1, const string &s2) {
    if (s1.empty() || s2.empty() || s1[0] == '0' || s2[0] == '0') return "0";
    const auto len1 = s1.length();
    const auto len2 = s2.length();
    vector<int> proInt(len1 + len2, 0);
    for (int i = (int)len1 - 1; i >= 0; --i) {
        int c = 0;
        for (int j = (int)len2 - 1; j >= 0; --j) {
            int product = proInt[i + j + 1] + (s1[i] - '0') * (s2[j] - '0') + c;
            c = product / 10;
            proInt[i + j + 1] = product % 10;
        }
        proInt[i] = c;
    }

    int s = proInt[0] ? 0 : 1;
    string ret;
    ret.reserve(len1 + len2);
    while (s < proInt.size()) {
        ret += ('0' + proInt[s++]);
    }

    return ret;
}

void test() {
    int a = 12942;
    int b = 19392;
    int proInt = a * b;
    string proStr = stringMultiply(to_string(a), to_string(b));
    assert(proStr == to_string(proInt));
}

int main() {
    test();
    return 0;
}