#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

int tempCount = 0;
int regCount = 1;

vector<string> tacInstructions;
vector<string> machineCode;

string newTemp() {
    tempCount++;
    return "T" + to_string(tempCount);
}

string newRegister() {
    string reg = "R" + to_string(regCount);
    regCount = (regCount % 8) + 1; // Simulate up to 8 registers
    return reg;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void generateTACandMachine(string left, char op, string right) {
    string temp = newTemp();
    string tac = temp + " = " + left + " " + op + " " + right;
    tacInstructions.push_back(tac);

    string reg = newRegister();

    // LOAD reg, left
    machineCode.push_back("LOAD " + reg + ", " + left);

    // OP reg, right
    string opInstr;
    switch (op) {
        case '+': opInstr = "ADD"; break;
        case '-': opInstr = "SUB"; break;
        case '*': opInstr = "MUL"; break;
        case '/': opInstr = "DIV"; break;
    }

    machineCode.push_back(opInstr + " " + reg + ", " + right);

    // STORE temp, reg
    machineCode.push_back("STORE " + temp + ", " + reg);
}

void processStacks(stack<string>& operands, stack<char>& operators) {
    string right = operands.top(); operands.pop();
    string left = operands.top(); operands.pop();
    char op = operators.top(); operators.pop();
    generateTACandMachine(left, op, right);
    operands.push("T" + to_string(tempCount));
}

void parseExpression(string expr) {
    stack<string> operands;
    stack<char> operators;

    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];

        if (isspace(ch)) continue;

        if (isalnum(ch)) {
            operands.push(string(1, ch));
        }
        else if (ch == '(') {
            operators.push(ch);
        }
        else if (ch == ')') {
            while (!operators.empty() && operators.top() != '(') {
                processStacks(operands, operators);
            }
            if (!operators.empty() && operators.top() == '(')
                operators.pop();
        }
        else if (isOperator(ch)) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                processStacks(operands, operators);
            }
            operators.push(ch);
        }
    }

    while (!operators.empty()) {
        processStacks(operands, operators);
    }
}

int main() {
    string input;
    cout << "Enter an arithmetic expression (e.g., (a + b) * c): ";
    getline(cin, input);

    parseExpression(input);

    cout << "\nThree Address Code:\n";
    for (const auto& line : tacInstructions) {
        cout << line << endl;
    }

    cout << "\nMachine Code:\n";
    for (const auto& line : machineCode) {
        cout << line << endl;
    }

    cout << "\nTotal TAC Instructions: " << tacInstructions.size() << endl;
    cout << "Total Temporaries Used: " << tempCount << endl;

    return 0;
}
