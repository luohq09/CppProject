#include <vector>
#include <assert.h>

using namespace std;

void qsort(vector<int> &nums, int s, int e) {
    if (s >= e) return;
    if (s + 1 == e) {
        if (nums[s] > nums[e]) swap(nums[s], nums[e]);
        return;
    }

    int a = nums[e], b = nums[(e + s) / 2], c = nums[e];
    int partition = min(max(a, b), max(b, c));

    // Partition
    int i = s, j = s, k = e;
    while (j <= k) {
        if (nums[j] < partition) {
            swap(nums[i++], nums[j++]);
        } else if (nums[j] > partition) {
            swap(nums[j], nums[k--]);
        } else {
            ++j;
        }
    }

    qsort(nums, s, --i);
    qsort(nums, ++k, e);
}

void test() {
    vector<int> t = {1, 3, 6, 0, 9, 1, 1, 1, 1, 1, 100, 999, 123 };
    qsort(t, 0, (int)t.size() - 1);
    for (int i = 0; i < t.size() - 1; ++i) {
        assert(t[i] <= t[i + 1]);
    }
}

int main() {
    test();
    return 0;
}