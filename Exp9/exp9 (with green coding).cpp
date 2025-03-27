#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;
// Function to check precedence of operators
inline int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        default: return 0;
    }
}
// Function to convert infix to postfix
string infixToPostfix(const string& infix) {
    stack<char> s;
    string postfix;
    for (char ch : infix) {
        if (isalnum(ch)) {
            postfix += ch; // Operand
        } else if (ch == '(') {
            s.push(ch);
        } else if (ch == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop(); // Remove '('
        } else {
            while (!s.empty() && precedence(s.top()) >= precedence(ch)) {
                postfix += s.top();
                s.pop();
            }
            s.push(ch);
        }
    }
    
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }
    
    return postfix;
}

// Function to convert infix to prefix
string infixToPrefix(string infix) {
    reverse(infix.begin(), infix.end());
    for (char &ch : infix) {
        ch = (ch == '(') ? ')' : (ch == ')') ? '(' : ch;
    }
    
    string prefix = infixToPostfix(infix);
    reverse(prefix.begin(), prefix.end());
    return prefix;
}
int main() {
    string infix;
    cout << "Enter an infix expression: ";
    cin >> infix;
    cout << "Postfix (Suffix) Expression: " << infixToPostfix(infix) << endl;
    cout << "Prefix Expression: " << infixToPrefix(infix) << endl;
    return 0;
}
