#include <bits/stdc++.h>
using namespace std;

map<string, vector<string>> grammar;
map<string, set<char>> firstSet, followSet;
map<string, map<char, string>> parsingTable;

// Compute FIRST set
void computeFIRST(string nonTerminal) {
    if (!firstSet[nonTerminal].empty()) return; // Already computed

    for (string prod : grammar[nonTerminal]) {
        if (isupper(prod[0])) {
            computeFIRST(string(1, prod[0]));
            firstSet[nonTerminal].insert(firstSet[string(1, prod[0])].begin(), firstSet[string(1, prod[0])].end());
        } else {
            firstSet[nonTerminal].insert(prod[0]); // Add terminal directly
        }
    }
}

// Compute FOLLOW set
void computeFOLLOW(string nonTerminal) {
    if (!followSet[nonTerminal].empty()) return; // Already computed

    if (nonTerminal == grammar.begin()->first) {
        followSet[nonTerminal].insert('$'); // Start symbol gets $
    }

    for (auto &entry : grammar) {
        string A = entry.first;
        for (string rule : entry.second) {
            for (size_t i = 0; i < rule.length(); i++) {
                if (rule[i] == nonTerminal[0]) {
                    // Case: A → αBβ
                    if (i + 1 < rule.length()) {
                        char nextChar = rule[i + 1];

                        if (isupper(nextChar)) {
                            followSet[nonTerminal].insert(firstSet[string(1, nextChar)].begin(), firstSet[string(1, nextChar)].end());
                            followSet[nonTerminal].erase('ε'); // Remove epsilon if present
                        } else {
                            followSet[nonTerminal].insert(nextChar);
                        }
                    } else {
                        // Case: A → αB (FOLLOW(A) → FOLLOW(B))
                        computeFOLLOW(A);
                        followSet[nonTerminal].insert(followSet[A].begin(), followSet[A].end());
                    }
                }
            }
        }
    }
}

// Construct Parsing Table
void constructParsingTable() {
    for (auto prod : grammar) {
        string A = prod.first;

        for (string rule : prod.second) {
            char firstChar = rule[0];

            if (!isupper(firstChar)) {
                parsingTable[A][firstChar] = rule;
            } else {
                for (char f : firstSet[string(1, firstChar)]) {
                    parsingTable[A][f] = rule;
                }
            }
        }
    }

    // Print Parsing Table
    cout << "\nParsing Table:\n";
    for (auto &entry : parsingTable) {
        for (auto &inner : entry.second) {
            cout << "M[" << entry.first << ", " << inner.first << "] = " << inner.second << endl;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();

    string nonTerminal, line;
    for (int i = 0; i < n; i++) {
        cout << "Enter the non-terminal: ";
        getline(cin, nonTerminal);
        cout << "Enter the productions (separated by '|'): ";
        getline(cin, line);

        stringstream ss(line);
        string prod;
        while (getline(ss, prod, '|')) {
            grammar[nonTerminal].push_back(prod);
        }
    }

    // Compute FIRST sets
    for (auto &entry : grammar) {
        computeFIRST(entry.first);
    }

    // Compute FOLLOW sets
    for (auto &entry : grammar) {
        computeFOLLOW(entry.first);
    }

    // Print FIRST sets
    cout << "\nFIRST Sets:\n";
    for (auto &entry : firstSet) {
        cout << "FIRST(" << entry.first << ") = { ";
        for (char f : entry.second) {
            cout << f << " ";
        }
        cout << "}\n";
    }

    // Print FOLLOW sets
    cout << "\nFOLLOW Sets:\n";
    for (auto &entry : followSet) {
        cout << "FOLLOW(" << entry.first << ") = { ";
        for (char f : entry.second) {
            cout << f << " ";
        }
        cout << "}\n";
    }

    // Construct Parsing Table
    constructParsingTable();

    return 0;
}