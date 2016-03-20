#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

class ReversePolishExpression {
public:
    void set_exp(const string &exp) {
        _exp_stk.clear();
        if (exp.empty()) {
            return;
        }
        
        vector<string> elems = _split_exp(exp);
        _exp_stk.reserve(elems.size());
        vector<char> ops;
        ops.reserve(elems.size());
        for (string &elem : elems) {
            if (_is_number(elem, 0)) {
                _exp_stk.push_back(move(elem));
            } else {
                char op = elem[0];
                switch (op) {
                    case '(':
                        ops.push_back(op);
                        break;
                    case ')':
                        while (ops.back() != '(') {
                            _exp_stk.push_back(string(1, ops.back()));
                            ops.pop_back();
                        }
                        ops.pop_back();
                        break;
                    case '+':
                    case '-':
                        if (ops.empty() || ops.back() == '(' || ops.back() == '+' || ops.back() == '-') {
                            ops.push_back(op);
                        } else {
                            while (!ops.empty() && (ops.back() == '*' || ops.back() == '/')) {
                                _exp_stk.push_back(string(1, ops.back()));
                                ops.pop_back();
                            }
                            ops.push_back(op);
                        }
                        break;
                    case '*':
                    case '/':
                        ops.push_back(op);
                        break;
                    default:
                        cout<<"Input error!!"<<endl;
                        break;
                }
            }
        }
        while (!ops.empty()) {
            _exp_stk.push_back(string(1, ops.back()));
            ops.pop_back();
        }
    }
    
    double calculate() {
        vector<double> res_stk;
        for (const string &elem : _exp_stk) {
            if (_is_number(elem, 0)) {
                res_stk.push_back(stod(elem));
            } else {
                double operand0 = res_stk.back();
                res_stk.pop_back();
                double operand1 = res_stk.back();
                res_stk.pop_back();
                char op = elem[0];
                switch (op) {
                    case '+':
                        res_stk.push_back(operand1 + operand0);
                        break;
                    case '-':
                        res_stk.push_back(operand1 - operand0);
                        break;
                    case '*':
                        res_stk.push_back(operand1 * operand0);
                        break;
                    case '/':
                        res_stk.push_back(operand1 / operand0);
                        break;
                    default:
                        cout<<"Invalid operator!!"<<endl;
                        break;
                }
            }
        }
        
        return res_stk.back();
    }
    
private:
    static inline bool _is_number(const string &str, int start) {
        return str[start] >= '0' && str[start] <= '9';
    }
    
    static inline bool _op_less_than(char op0, char op1) {
        if (op0 == '*' || op0 == '/') {
            return false;
        } else {
            return op1 == '*' || op1 == '/';
        }
    }
    
    vector<string> _split_exp(const string &exp) {
        const int len = (int)exp.size();
        vector<string> res;
        int i = 0;
        while (i < len) {
            string str;
            if (_is_number(exp, i)) {
                while (_is_number(exp, i)) {
                    str += exp[i++];
                }
            } else {
                str += exp[i++];
            }
            
            res.push_back(move(str));
        }
        
        return res;
    }
    
    vector<string> _exp_stk;
};

int main(int argc, const char * argv[]) {
    ReversePolishExpression exp;
    exp.set_exp("(1+2)/10+6/3+7+8");
    assert((1.0+2)/10+6/3+7+8 == exp.calculate());
}
