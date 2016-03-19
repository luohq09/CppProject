#include <string>
#include <vector>
#include <assert.h>

using namespace std;

class SimpleRexNFA {
#define SPLIT 256
public:
    SimpleRexNFA(const string &ptn) {
        const int len = ptn.length();
        int i = 0;
        int last_state = -1;
        while (i < len) {
            const size_t size = _states.size();
            if (i + 1 < len && ptn[i + 1] == '*') {
                _states.resize(size + 1);
                State &split_state = _states.back();
                split_state.c = SPLIT;
                split_state.out = size + 1;

                _states.resize(size + 2);
                State &single_state = _states.back();
                single_state.c = ptn[i];
                single_state.out = size;

                i += 2;
            } else {
                _states.resize(size + 1);
                State &single_state = _states.back();
                single_state.c = ptn[i];

                ++i;
            }

            if (last_state != -1) {
                State &ins = _states[last_state];
                if (ins.c == SPLIT) {
                    ins.out1 = size;
                } else {
                    ins.out = size;
                }
            }

            last_state = size;
        }

        _last_state = _states.size();
        _states.resize(_last_state + 1);
        if (last_state != -1) {
            State &ins = _states[last_state];
            if (ins.c == SPLIT) {
                ins.out1 = _last_state;
            }
            else {
                ins.out = _last_state;
            }
        }

        _clist = 0;
    }

    bool is_matched(const string &src) {
        _lists[0].clear();
        _lists[1].clear();
        _clist = 0;
        for (State &ins : _states) {
            ins.last_list = -1;
        }

        _add_state(0, _clist);
        for (char c : src) {
            int nlist = 1 - _clist;
            for (int state : _lists[_clist]) {
                State &ins = _states[state];
                if (ins.c == c || ins.c == '.') {
                    _add_state(ins.out, nlist);
                }

                if (ins.last_list != nlist) {
                    ins.last_list = -1;
                }
            }

            _lists[_clist].clear();
            _clist = 1 - _clist;

            if (_lists[_clist].empty()) {
                return false;
            }
        }

        for (int state : _lists[_clist]) {
            if (state == _last_state) {
                return true;
            }
        }
        return false;
    }

private:
    void _add_state(int state, int list) {
        if (state == -1) {
            return;
        }

        State &ins = _states[state];
        if (ins.c == SPLIT) {
            if (_states[ins.out].last_list != list) {
                _lists[list].push_back(ins.out);
                _states[ins.out].last_list = list;
            }
            _add_state(ins.out1, list);
        } else {
            if (ins.last_list != list) {
                _lists[list].push_back(state);
                ins.last_list = list;
            }
        }
    }

    struct State {
        int c;
        int out;
        int out1;
        int last_list;

        State() {
            c = -1;
            out = -1;
            out1 = -1;
            last_list = -1;
        }
    };

    vector<State> _states;
    vector<int> _lists[2];
    int _clist;
    int _last_state;
};

void test() {
    string ptn = "b*a";
    SimpleRexNFA nfa(ptn);
    string src = "a";
    assert(nfa.is_matched(src));
    src = "bbbbbba";
    assert(nfa.is_matched(src));
    src = "bbbbbbaa";
    assert(!nfa.is_matched(src));
    src = "bbbbbbc";
    assert(!nfa.is_matched(src));
}
int main() {
    test();
    return 0;
}