#include <iostream>
#include <vector>
#include <assert.h>

#include "immutable_hash_table.h"

using namespace std;
using namespace paradigm4;

void test() {
    const int n = 10000000;
    srand((unsigned)time(NULL));
    vector<std::pair<int, int>> elements;
    elements.reserve(n);
    for (int i = 0; i < n; ++i) {
        elements.emplace_back(rand(), rand());
    }
    vector<std::pair<int, int>> backup(elements);

    clock_t begin = clock();
    immutable_hash_table<int, int> ht(elements);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<< "Construction time: " << elapsed_secs << " seconds" << endl;

    begin = clock();
    for (auto e : backup) {
        auto p = ht.find(e.first);
        assert(p->second == e.second);
    }
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<< "Searching time: " << elapsed_secs << " seconds" << endl;
}

int main() {
    test();
    return 0;
}