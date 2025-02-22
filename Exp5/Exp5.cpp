#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;

struct Production {
    string nonTerminal;
    vector<string> productions;
};

vector<Production> grammar;
map<string, set<string>> FIRST, FOLLOW;
map<pair<string, string>, string> parsingTable;

// Function to print grammar rules
void printGrammar(const vector<Production> &grammar) {
    cout << "\nGrammar Rules:\n";
    for (const auto &prod : grammar) {
        cout << prod.nonTerminal << " -> ";
        for (size_t i = 0; i < prod.productions.size(); i++) {
            cout << prod.productions[i];
            if (i != prod.productions.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

// Left Recursion Elimination
void eliminateLeftRecursion(vector<Production> &grammar) {
    cout << "\nEliminating Left Recursion...\n";
    vector<Production> newGrammar;

    for (auto &prod : grammar) {
        string A = prod.nonTerminal;
        vector<string> alpha, beta;

        for (auto &p : prod.productions) {
            if (p.find(A) == 0) {
                alpha.push_back(p.substr(A.size())); // Left recursion case
            } else {
                beta.push_back(p); // Non-left-recursive case
            }
        }

        if (!alpha.empty()) {
            Production newProd;
            newProd.nonTerminal = A + "'";

            for (auto &a : alpha) {
                newProd.productions.push_back(a + newProd.nonTerminal);
            }
            newProd.productions.push_back("epsilon");

            prod.productions.clear();
            for (auto &b : beta) {
                prod.productions.push_back(b + newProd.nonTerminal);
            }

            newGrammar.push_back(newProd);
        }
        newGrammar.push_back(prod);
    }
    grammar = newGrammar;
    printGrammar(grammar);
}

// Function to find common prefix for left factoring
string commonPrefix(string a, string b) {
    string prefix = "";
    int len = min(a.length(), b.length());
    for (int i = 0; i < len; i++) {
        if (a[i] == b[i])
            prefix += a[i];
        else
            break;
    }
    return prefix;
}

// Left Factoring
void leftFactorGrammar(vector<Production> &grammar) {
    cout << "\nPerforming Left Factoring...\n";
    vector<Production> newGrammar;
    
    for (auto &prod : grammar) {
        string prefix = prod.productions[0];
        for (size_t i = 1; i < prod.productions.size(); i++) {
            prefix = commonPrefix(prefix, prod.productions[i]);
        }
        
        if (prefix.empty()) {
            newGrammar.push_back(prod);
            continue;
        }
        
        Production newProd;
        newProd.nonTerminal = prod.nonTerminal + "'";

        vector<string> remaining;
        for (string rule : prod.productions) {
            if (rule.substr(0, prefix.length()) == prefix) {
                string suffix = rule.substr(prefix.length());
                if (suffix.empty()) suffix = "epsilon";
                newProd.productions.push_back(suffix);
            } else {
                remaining.push_back(rule);
            }
        }
        
        prod.productions.clear();
        prod.productions.push_back(prefix + newProd.nonTerminal);
        for (string r : remaining) {
            prod.productions.push_back(r);
        }
        
        newGrammar.push_back(prod);
        newGrammar.push_back(newProd);
    }

    grammar = newGrammar;
    printGrammar(grammar);
}

// Function to compute FIRST sets
void computeFIRST() {
    cout << "\nComputing FIRST Sets...\n";
    bool changed;
    do {
        changed = false;
        for (auto &prod : grammar) {
            set<string> &firstSet = FIRST[prod.nonTerminal];

            for (auto &rule : prod.productions) {
                if (islower(rule[0]) || rule[0] == 'epsilon') {
                    changed |= firstSet.insert(rule.substr(0, 1)).second;
                } else {
                    for (char ch : rule) {
                        if (isupper(ch)) {
                            set<string> &nextFIRST = FIRST[string(1, ch)];
                            for (auto &f : nextFIRST) {
                                if (f != "epsilon")
                                    changed |= firstSet.insert(f).second;
                            }
                            if (nextFIRST.find("epsilon") == nextFIRST.end())
                                break;
                        } else {
                            changed |= firstSet.insert(string(1, ch)).second;
                            break;
                        }
                    }
                }
            }
        }
    } while (changed);

    for (auto &entry : FIRST) {
        cout << "FIRST(" << entry.first << ") = { ";
        for (auto &val : entry.second) {
            cout << val << " ";
        }
        cout << "}\n";
    }
}

// Function to compute FOLLOW sets
void computeFOLLOW() {
    cout << "\nComputing FOLLOW Sets...\n";
    FOLLOW[grammar[0].nonTerminal].insert("$");

    bool changed;
    do {
        changed = false;
        for (auto &prod : grammar) {
            string A = prod.nonTerminal;
            for (auto &rule : prod.productions) {
                for (size_t i = 0; i < rule.length(); i++) {
                    if (isupper(rule[i])) {
                        set<string> &followSet = FOLLOW[string(1, rule[i])];

                        if (i + 1 < rule.length()) {
                            if (islower(rule[i + 1]))
                                changed |= followSet.insert(string(1, rule[i + 1])).second;
                            else {
                                set<string> &nextFIRST = FIRST[string(1, rule[i + 1])];
                                for (auto &f : nextFIRST) {
                                    if (f != "epsilon")
                                        changed |= followSet.insert(f).second;
                                }
                                if (nextFIRST.find("epsilon") != nextFIRST.end())
                                    for (auto &f : FOLLOW[A])
                                        changed |= followSet.insert(f).second;
                            }
                        } else {
                            for (auto &f : FOLLOW[A])
                                changed |= followSet.insert(f).second;
                        }
                    }
                }
            }
        }
    } while (changed);

    for (auto &entry : FOLLOW) {
        cout << "FOLLOW(" << entry.first << ") = { ";
        for (auto &val : entry.second) {
            cout << val << " ";
        }
        cout << "}\n";
    }
}

// Function to construct parsing table
void constructParsingTable() {
    cout << "\nConstructing Predictive Parsing Table...\n";
    for (auto &prod : grammar) {
        for (auto &rule : prod.productions) {
            set<string> firstSet;
            if (islower(rule[0]) || rule[0] == 'epsilon') {
                firstSet.insert(rule.substr(0, 1));
            } else {
                firstSet = FIRST[string(1, rule[0])];
            }

            for (auto &f : firstSet) {
                if (f != "epsilon")
                    parsingTable[{prod.nonTerminal, f}] = rule;
            }

            if (firstSet.find("epsilon") != firstSet.end()) {
                for (auto &f : FOLLOW[prod.nonTerminal]) {
                    parsingTable[{prod.nonTerminal, f}] = "epsilon";
                }
            }
        }
    }

    for (auto &entry : parsingTable) {
        cout << "M[" << entry.first.first << ", " << entry.first.second << "] = " << entry.second << endl;
    }
}

// Main function
int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();

    grammar.resize(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter the non-terminal: ";
        getline(cin, grammar[i].nonTerminal);

        cout << "Enter the productions (separated by '|'): ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string prod;
        while (getline(ss, prod, '|')) {
            grammar[i].productions.push_back(prod);
        }
    }

    printGrammar(grammar);
    eliminateLeftRecursion(grammar);
    leftFactorGrammar(grammar);
    computeFIRST();
    computeFOLLOW();
    constructParsingTable();

    return 0;
}