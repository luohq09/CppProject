//
//  main.cpp
//  KMP
//
//  Created by LuoHuaqing on 10/27/15.
//  Copyright © 2015 LuoHuaqing. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>

#include <sstream>

#include <stdlib.h>

using namespace std;

vector<int> getNext(const string &pattern)
{
    auto len = pattern.size();
    vector<int> next;
    next.resize(len);
    
    next[0] = -1;
    for (int i = -1, j = 0; j < len; )
    {
        if (i == -1 || pattern[i] == pattern[j]) {
            next[++j] = ++i;
        } else {
            i = next[i];
        }
    }
    
    return next;
}

int KMP(const string &src, const string &ptn)
{
    if (ptn.empty()) return 0;
    if (src.size() < ptn.size()) return -1;
    
    vector<int> next = getNext(ptn);
    int i = 0, j = 0;
    const int sLen = (int)src.size();
    const int pLen = (int)ptn.size();
    while (i < pLen && j < sLen) {
        if (i == -1 || ptn[i] == src[j]) {
            ++i; ++j;
        } else {
            i = next[i];
        }
    }
    
    if (i == pLen) {
        return j - pLen;
    }
    
    return -1;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string src = "1212334567";
    string ptn = "3345";
    auto res = KMP(src, ptn);
    
    vector<int> a = { 1, 2, 3, 3, 4 };
    unique(a.begin(), a.end());
    
    return 0;
}
