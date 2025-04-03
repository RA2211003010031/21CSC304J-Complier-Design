#include <iostream>
#include <vector>
#include <string>

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

vector<Quadruple> quadruples;
vector<Triple> triples;
vector<IndirectTriple> indirectTriples;
vector<string> expressions;

void generateTAC(const string& expr) {
    int tempCount = 1;
    string temp = "t" + to_string(tempCount);
    string op, arg1, arg2;
    
    cout << "\nThree Address Code (TAC):" << endl;
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            arg1 = string(1, expr[i - 1]);
            op = string(1, expr[i]);
            arg2 = string(1, expr[i + 1]);
            temp = "t" + to_string(tempCount++);
            
            // Print TAC
            cout << temp << " = " << arg1 << " " << op << " " << arg2 << endl;
            
            // Store in data structures
            quadruples.push_back({op, arg1, arg2, temp});
            triples.push_back({op, arg1, arg2});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1)});
            expressions.push_back(temp);
        }
    }
}

void displayQuadruples() {
    cout << "\nQuadruple Representation:" << endl;
    cout << "Op\tArg1\tArg2\tResult" << endl;
    for (const auto& quad : quadruples) {
        cout << quad.op << "\t" << quad.arg1 << "\t" << quad.arg2 << "\t" << quad.result << endl;
    }
}

void displayTriples() {
    cout << "\nTriple Representation:" << endl;
    cout << "Index\tOp\tArg1\tArg2" << endl;
    for (size_t i = 0; i < triples.size(); i++) {
        cout << i << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << endl;
    }
}

void displayIndirectTriples() {
    cout << "\nIndirect Triple Representation:" << endl;
    cout << "Index\tReference" << endl;
    for (size_t i = 0; i < indirectTriples.size(); i++) {
        cout << i << "\t" << indirectTriples[i].index << endl;
    }
}

int main() {
    string expression;
    cout << "Enter a simple arithmetic expression (e.g., a+b*c): ";
    cin >> expression;
    
    generateTAC(expression);
    displayQuadruples();
    displayTriples();
    displayIndirectTriples();
    
    return 0;
}