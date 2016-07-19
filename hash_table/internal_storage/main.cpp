#include <iostream>
#include <vector>
#include <assert.h>

#include <ctime>

#include "immutable_hash_table.h"

using namespace std;
using namespace paradigm4;

int main() {
    vector<immutable_hash_table<int, int>::entryT> vec =
            { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {33, 33}, {44, 44,}, {199999, 199999} };
    auto backup = vec;
    immutable_hash_table<int, int> ht(vec);

    clock_t begin = clock();
    for (auto e : backup) {
        auto p = ht.find(e.first);
        assert(p->second == e.second);
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<< "Elapsed time: " << elapsed_secs << " seconds" << endl;
    return 0;
}