#include <iostream>
#include <vector>
#include <stack>
#include <cctype>

using namespace std;

struct Quadruple {
    string op, arg1, arg2, result;
};

struct Triple {
    string op, arg1, arg2;
};

struct IndirectTriple {
    int index;
};

// Function to get operator precedence
int precedence(char op) {
    return (op == '*' || op == '/') ? 2 : (op == '+' || op == '-') ? 1 : 0;
}

// Convert infix to postfix (optimized version)
void infixToPostfix(const string& expr, vector<string>& postfix) {
    stack<char> operators;
    string operand;

    for (char ch : expr) {
        if (isalnum(ch)) {
            operand += ch;
        } else {
            if (!operand.empty()) {
                postfix.push_back(move(operand));
                operand.clear();
            }
            if (ch == '(') {
                operators.push(ch);
            } else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix.emplace_back(1, operators.top());
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // Pop '('
            } else if (precedence(ch) > 0) {
                while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                    postfix.emplace_back(1, operators.top());
                    operators.pop();
                }
                operators.push(ch);
            }
        }
    }
    if (!operand.empty()) postfix.push_back(move(operand));
    while (!operators.empty()) {
        postfix.emplace_back(1, operators.top());
        operators.pop();
    }
}

// Generate Three Address Code
void generateTAC(const string& expr, vector<Quadruple>& quadruples, vector<Triple>& triples, vector<IndirectTriple>& indirectTriples) {
    vector<string> postfix;
    infixToPostfix(expr, postfix);

    stack<string> operands;
    int tempCount = 1;

    cout << "\nThree Address Code (TAC):\n";
    for (const string& token : postfix) {
        if (isalnum(token[0])) {
            operands.push(token);
        } else {
            if (operands.size() < 2) {
                cerr << "Error: Invalid expression (insufficient operands).\n";
                return;
            }
            string arg2 = move(operands.top()); operands.pop();
            string arg1 = move(operands.top()); operands.pop();
            string temp = "t" + to_string(tempCount++);

            cout << temp << " = " << arg1 << " " << token << " " << arg2 << "\n";
            quadruples.push_back({token, arg1, arg2, temp});
            triples.push_back({token, arg1, arg2});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1)});
            operands.push(temp);
        }
    }
}

// Display functions (optimized with `const&`)
void displayQuadruples(const vector<Quadruple>& quadruples) {
    cout << "\nQuadruple Representation:\nOp\tArg1\tArg2\tResult\n";
    for (const auto& q : quadruples) {
        cout << q.op << "\t" << q.arg1 << "\t" << q.arg2 << "\t" << q.result << "\n";
    }
}

void displayTriples(const vector<Triple>& triples) {
    cout << "\nTriple Representation:\nIndex\tOp\tArg1\tArg2\n";
    for (size_t i = 0; i < triples.size(); i++) {
        cout << i << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << "\n";
    }
}

void displayIndirectTriples(const vector<IndirectTriple>& indirectTriples) {
    cout << "\nIndirect Triple Representation:\nIndex\tReference\n";
    for (size_t i = 0; i < indirectTriples.size(); i++) {
        cout << i << "\t" << indirectTriples[i].index << "\n";
    }
}

int main() {
    string expression;
    cout << "Enter a simple arithmetic expression (e.g., (a+b)*c): ";
    getline(cin, expression);

    vector<Quadruple> quadruples;
    vector<Triple> triples;
    vector<IndirectTriple> indirectTriples;

    generateTAC(expression, quadruples, triples, indirectTriples);
    displayQuadruples(quadruples);
    displayTriples(triples);
    displayIndirectTriples(indirectTriples);

    return 0;
}
