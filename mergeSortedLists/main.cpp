#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;

class Node {
public:
    int data;
    Node *next;
    
    Node(int d) : data(d), next(nullptr) {}
};

void printList(Node *h) {
    while (h) {
        cout << h->data << ' ';
        h = h->next;
    }
    cout << endl;
}

Node *mergeSortedLists(Node *h1, Node *h2) {
    if (h1 == nullptr) return h2;
    if (h2 == nullptr) return h1;
    
    Node dummy(-1);
    Node *tail = &dummy;
    while (true) {
        Node *min;
        if (h1->data < h2->data) {
            min = h1;
            h1 = h1->next;
        } else {
            min = h2;
            h2 = h2->next;
        }
        
        tail->next = min;
        tail = min;
        
        if (h1 == nullptr) {
            tail->next = h2;
            break;
        }
        
        if (h2 == nullptr) {
            tail->next = h1;
            break;
        }
    }
    
    return dummy.next;
}

// test code
Node *constructList(const vector<int> datas) {
    Node dummy(-1);
    Node *tail = &dummy;
    for (int data : datas) {
        Node *node = new Node(data);
        tail->next = node;
        tail = node;
    }
    
    return dummy.next;
}

void deconstructList(Node *h) {
    while (h) {
        Node *next = h->next;
        delete h;
        h = next;
    }
}

void orderTest(Node *h) {
    while (h && h->next) {
        assert(h->data <= h->next->data);
        h = h->next;
    }
}

#define MAX_LEN 20
#define MAX_DATA 100
void test(int n) {
    for (int i = 0; i < n; ++i) {
        int len1 = rand() % MAX_LEN;
        int len2 = rand() % MAX_LEN;
        
        vector<int> v1, v2;
        v1.reserve(len1);
        v2.reserve(len2);
        
        for (int j = 0; j < len1; ++j) {
            v1.push_back(rand() % MAX_DATA);
        }
        sort(v1.begin(), v1.end());
        for (int j = 0; j < len2; ++j) {
            v2.push_back(rand() % MAX_DATA);
        }
        sort(v2.begin(), v2.end());
        
        Node *h1 = constructList(v1);
        Node *h2 = constructList(v2);
        
        cout << "List 1: ";
        printList(h1);
        cout << "List 2: ";
        printList(h2);
        
        Node *newH = mergeSortedLists(h1, h2);
        
        cout << "Merged List: ";
        printList(newH);
        cout << endl;
        
        //test order
        orderTest(newH);
        
        deconstructList(newH);
    }
}

int main() {
    test(10);
}
