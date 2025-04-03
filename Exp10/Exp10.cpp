#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>

using namespace std;

struct Quadruple {
    string op;
    string arg1, arg2;
    string result;
};

struct Triple {
    string op;
    string arg1, arg2;
};

struct IndirectTriple {
    int index;
};

// Function to get operator precedence
int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Convert infix to postfix using Shunting Yard Algorithm
vector<string> infixToPostfix(const string& expr) {
    vector<string> postfix;
    stack<char> operators;
    string operand;

    for (size_t i = 0; i < expr.length(); i++) {
        if (isalnum(expr[i])) {
            operand += expr[i];
        } else {
            if (!operand.empty()) {
                postfix.push_back(operand);
                operand.clear();
            }
            if (expr[i] == '(') {
                operators.push(expr[i]);
            } else if (expr[i] == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix.push_back(string(1, operators.top()));
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // Pop '('
            } else if (precedence(expr[i]) > 0) {
                while (!operators.empty() && precedence(operators.top()) >= precedence(expr[i])) {
                    postfix.push_back(string(1, operators.top()));
                    operators.pop();
                }
                operators.push(expr[i]);
            }
        }
    }
    if (!operand.empty()) postfix.push_back(operand);
    while (!operators.empty()) {
        postfix.push_back(string(1, operators.top()));
        operators.pop();
    }
    return postfix;
}

void generateTAC(const string& expr, vector<Quadruple>& quadruples, vector<Triple>& triples, vector<IndirectTriple>& indirectTriples) {
    vector<string> postfix = infixToPostfix(expr);
    stack<string> operands;
    int tempCount = 1;
    ostringstream output;

    output << "\nThree Address Code (TAC):\n";
    for (const string& token : postfix) {
        if (isalnum(token[0])) {
            operands.push(token);
        } else {
            if (operands.size() < 2) {
                cerr << "Error: Invalid expression (insufficient operands).\n";
                return;
            }
            string arg2 = operands.top(); operands.pop();
            string arg1 = operands.top(); operands.pop();
            string temp = "t" + to_string(tempCount++);

            output << temp << " = " << arg1 << " " << token << " " << arg2 << "\n";
            quadruples.push_back({token, arg1, arg2, temp});
            triples.push_back({token, arg1, arg2});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1)});
            operands.push(temp);
        }
    }
    cout << output.str();
}

void displayQuadruples(const vector<Quadruple>& quadruples) {
    cout << "\nQuadruple Representation:\n";
    cout << "Op\tArg1\tArg2\tResult\n";
    for (const auto& quad : quadruples) {
        cout << quad.op << "\t" << quad.arg1 << "\t" << quad.arg2 << "\t" << quad.result << "\n";
    }
}

void displayTriples(const vector<Triple>& triples) {
    cout << "\nTriple Representation:\n";
    cout << "Index\tOp\tArg1\tArg2\n";
    for (size_t i = 0; i < triples.size(); i++) {
        cout << i << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << "\n";
    }
}

void displayIndirectTriples(const vector<IndirectTriple>& indirectTriples) {
    cout << "\nIndirect Triple Representation:\n";
    cout << "Index\tReference\n";
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