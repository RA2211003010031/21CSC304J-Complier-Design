#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Quadruple {
    string op;
    char arg1, arg2;
    string result;
};

struct Triple {
    string op;
    char arg1, arg2;
};

struct IndirectTriple {
    int index;
};

void generateTAC(const string& expr, vector<Quadruple>& quadruples, vector<Triple>& triples, vector<IndirectTriple>& indirectTriples) {
    int tempCount = 1;
    ostringstream output;

    output << "\nThree Address Code (TAC):\n";
    
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            char arg1 = expr[i - 1];
            char op = expr[i];
            char arg2 = expr[i + 1];

            int tempVar = tempCount++;
            string temp = "t" + to_string(tempVar);
            
            // Buffer Output
            output << temp << " = " << arg1 << " " << op << " " << arg2 << "\n";

            // Store in optimized data structures
            quadruples.push_back({string(1, op), arg1, arg2, temp});
            triples.push_back({string(1, op), arg1, arg2});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1)});
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
    cout << "Enter a simple arithmetic expression (e.g., a+b*c): ";
    cin >> expression;

    vector<Quadruple> quadruples;
    vector<Triple> triples;
    vector<IndirectTriple> indirectTriples;

    // Preallocate vector sizes to reduce dynamic resizing
    quadruples.reserve(10);
    triples.reserve(10);
    indirectTriples.reserve(10);

    generateTAC(expression, quadruples, triples, indirectTriples);
    displayQuadruples(quadruples);
    displayTriples(triples);
    displayIndirectTriples(indirectTriples);

    return 0;
}