#include <iostream>
#include <stack>
#include <string>
using namespace std;

int prec(char c) {
    if (c == '^'){
        return 3;
    }
    else if (c == '/' || c == '*'){
        return 2;
    }
    else if (c == '+' || c == '-'){
        return 1;
    }
    else{
        return -1;
    }
}

string infix_postfix(string s){
    stack<char> st;
    string result;
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
            result += c;
        }
        else if (c == '('){
            st.push('(');
        }
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                st.pop();
            }
            st.pop(); 
        }
        else {
            while (!st.empty() && prec(c) <= prec(st.top())) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    return result;
}

int evaluate(string a){
    stack<int> st;
    for(int i = 0; i < a.size(); ++i){
        if (isdigit(a[i])){
            st.push(a[i] - '0');
        }
        else {
            int val1 = st.top();
            st.pop();
            int val2 = st.top();
            st.pop();
            switch (a[i]) {
                case '+':
                    st.push(val2 + val1);
                    break;
                case '-':
                    st.push(val2 - val1);
                    break;
                case '*':
                    st.push(val2 * val1);
                    break;
                case '/':
                    st.push(val2 / val1);
                    break;
            }
        }
    }
    return st.top();
}

int main(){
    string expression;
    cout<<"Enter the expression to be evaluated: "<<endl;
    cin>>expression;
    string convert = infix_postfix(expression);
    int answer = evaluate(convert);
    cout << "The postfix expression is: " << convert << endl;
    cout << "The result is: " << answer << endl;
    return 0;
}