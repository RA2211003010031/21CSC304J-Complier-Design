#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Production {
    string nonTerminal;
    vector<string> productions;
};

// Function to trim spaces from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return ""; // No content
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to check if a production has left recursion
bool hasLeftRecursion(const Production& prod) {
    for (const auto& p : prod.productions) {
        if (p.find(prod.nonTerminal) == 0) {
            return true; // Starts with the same non-terminal → left recursion
        }
    }
    return false;
}

// Function to eliminate direct left recursion
void eliminateLeftRecursion(vector<Production>& grammar) {
    unordered_map<string, Production> processed;

    for (size_t i = 0; i < grammar.size(); ++i) {
        Production& prod = grammar[i];

        // If this production is left-recursive
        if (hasLeftRecursion(prod)) {
            cout << "Left recursion detected in: " << prod.nonTerminal << endl;

            // If the next production exists and its non-terminal matches a recursive dependency
            if (i + 1 < grammar.size() && prod.productions[0][0] == grammar[i + 1].nonTerminal[0]) {
                cout << "Processing next non-terminal first: " << grammar[i + 1].nonTerminal << endl;
                eliminateLeftRecursion(grammar); // Recursive call for next non-terminal
            }

            string A = prod.nonTerminal;
            vector<string> alpha, beta;

            // Classify alpha and beta
            for (const auto& p : prod.productions) {
                string trimmedP = trim(p);
                if (trimmedP.find(A) == 0) { // Direct left recursion
                    alpha.push_back(trimmedP.substr(A.size()));
                } else {
                    beta.push_back(trimmedP);
                }
            }

            // Rewrite grammar
            if (!alpha.empty()) { // If left recursion exists
                prod.productions.clear();
                string newNonTerminal = A + "'"; // New helper non-terminal

                for (const auto& b : beta) {
                    prod.productions.push_back(b + newNonTerminal);
                }

                Production newProd;
                newProd.nonTerminal = newNonTerminal;
                for (const auto& a : alpha) {
                    newProd.productions.push_back(a + newNonTerminal);
                }
                newProd.productions.push_back("epsilon"); // Replace ε with "epsilon"

                grammar.push_back(newProd); // Add the new production
            }
        }
    }
}

// Function to print the grammar
void printGrammar(const vector<Production>& grammar) {
    for (const auto& prod : grammar) {
        cout << prod.nonTerminal << " -> ";
        for (size_t i = 0; i < prod.productions.size(); ++i) {
            cout << prod.productions[i];
            if (i < prod.productions.size() - 1) {
                cout << " | ";
            }
        }
        cout << endl;
    }
}

int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();

    vector<Production> grammar(n);
    for (int i = 0; i < n; ++i) {
        cout << "Enter the non-terminal for production " << i + 1 << ": ";
        getline(cin, grammar[i].nonTerminal);

        cout << "Enter the productions for " << grammar[i].nonTerminal << " (separated by '|'): ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string production;
        while (getline(ss, production, '|')) {
            grammar[i].productions.push_back(trim(production));
        }
    }

    eliminateLeftRecursion(grammar);
    cout << "\nGrammar after eliminating left recursion:\n";
    printGrammar(grammar);

    return 0;
}
