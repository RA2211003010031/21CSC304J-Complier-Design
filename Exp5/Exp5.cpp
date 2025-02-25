#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>

using namespace std;

map<string, set<char>> FIRST, FOLLOW;
map<string, vector<string>> grammar;
map<string, map<char, string>> parsingTable;

void computeFIRST(string nonTerminal);
void computeFOLLOW(string nonTerminal);
void constructParsingTable();

void eliminateLeftRecursion(string nonTerminal, vector<string> productions) {
    vector<string> alpha, beta;
    string newNonTerminal = nonTerminal + "'";

    cout << "\nChecking for Left Recursion in " << nonTerminal << "...\n";
    cout << nonTerminal << " -> ";
    for (size_t i = 0; i < productions.size(); i++) {
        if (i > 0) cout << " | ";
        cout << productions[i];
    }
    cout << endl;

    for (string prod : productions) {
        if (prod[0] == nonTerminal[0])
            alpha.push_back(prod.substr(1));
        else
            beta.push_back(prod);
    }

    if (alpha.empty()) {
        grammar[nonTerminal] = productions;
        return;
    }

    vector<string> newBeta;
    for (const string &b : beta)
        newBeta.push_back(b + newNonTerminal);
    
    vector<string> newAlpha;
    for (const string &a : alpha)
        newAlpha.push_back(a + newNonTerminal);
    newAlpha.push_back("ε");

    grammar[nonTerminal] = newBeta;
    grammar[newNonTerminal] = newAlpha;

    // Print the transformed grammar
    cout << "After Eliminating Left Recursion:\n";
    cout << nonTerminal << " -> ";
    for (size_t i = 0; i < newBeta.size(); i++) {
        if (i > 0) cout << " | ";
        cout << newBeta[i];
    }
    cout << endl;

    cout << newNonTerminal << " -> ";
    for (size_t i = 0; i < newAlpha.size(); i++) {
        if (i > 0) cout << " | ";
        cout << newAlpha[i];
    }
    cout << endl;
}

void leftFactoring(string nonTerminal, vector<string> productions) {
    cout << "\nChecking for Left Factoring in " << nonTerminal << "...\n";

    string prefix = productions[0];
    for (string prod : productions) {
        size_t i = 0;
        while (i < prefix.size() && i < prod.size() && prefix[i] == prod[i])
            i++;
        prefix = prefix.substr(0, i);
    }

    if (prefix.empty()) {
        grammar[nonTerminal] = productions;
        return;
    }

    string newNonTerminal = nonTerminal + "'";
    vector<string> newProductions;
    for (const string &prod : productions) {
        string suffix = prod.substr(prefix.size());
        if (suffix.empty()) suffix = "ε";
        newProductions.push_back(suffix);
    }

    grammar[nonTerminal] = {prefix + newNonTerminal};
    grammar[newNonTerminal] = newProductions;

    // Print the transformed grammar
    cout << "After Left Factoring:\n";
    cout << nonTerminal << " -> " << prefix << newNonTerminal << endl;
    cout << newNonTerminal << " -> ";
    for (size_t i = 0; i < newProductions.size(); i++) {
        if (i > 0) cout << " | ";
        cout << newProductions[i];
    }
    cout << endl;
}

void computeFIRST(string nonTerminal) {
    if (!FIRST[nonTerminal].empty()) return;

    for (const string &prod : grammar[nonTerminal]) {
        if (!isupper(prod[0])) {
            FIRST[nonTerminal].insert(prod[0]);
        } else {
            computeFIRST(string(1, prod[0]));
            FIRST[nonTerminal].insert(FIRST[string(1, prod[0])].begin(), FIRST[string(1, prod[0])].end());
        }
    }
}

void computeFOLLOW(string startSymbol) {
    FOLLOW[startSymbol].insert('$'); // Add $ to FOLLOW of start symbol
    bool changed;
    do {
        changed = false;
        for (auto &[lhs, productions] : grammar) {
            for (const string &prod : productions) {
                for (size_t i = 0; i < prod.length(); i++) {
                    string currentSymbol = string(1, prod[i]);
                    if (!isupper(prod[i])) continue; // Skip terminals

                    // Check if currentSymbol is followed by another symbol
                    if (i + 1 < prod.length()) {
                        string nextSymbol = string(1, prod[i + 1]);
                        if (isupper(nextSymbol[0])) {
                            // Add FIRST(nextSymbol) to FOLLOW(currentSymbol)
                            for (char c : FIRST[nextSymbol]) {
                                if (c != 'ε' && FOLLOW[currentSymbol].insert(c).second) {
                                    changed = true;
                                }
                            }
                            // If FIRST(nextSymbol) contains ε, inherit FOLLOW(lhs)
                            if (FIRST[nextSymbol].count('ε')) {
                                for (char c : FOLLOW[lhs]) {
                                    if (FOLLOW[currentSymbol].insert(c).second) {
                                        changed = true;
                                    }
                                }
                            }
                        } else {
                            // If nextSymbol is a terminal, add it to FOLLOW(currentSymbol)
                            if (FOLLOW[currentSymbol].insert(nextSymbol[0]).second) {
                                changed = true;
                            }
                        }
                    } else {
                        // If currentSymbol is at the end, inherit FOLLOW(lhs)
                        for (char c : FOLLOW[lhs]) {
                            if (FOLLOW[currentSymbol].insert(c).second) {
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void constructParsingTable() {
    for (auto &[nonTerminal, productions] : grammar) {
        for (const string &prod : productions) {
            char firstChar = prod[0];
            if (!isupper(firstChar)) {
                parsingTable[nonTerminal][firstChar] = prod;
            } else {
                for (char terminal : FIRST[string(1, firstChar)]) {
                    parsingTable[nonTerminal][terminal] = prod;
                }
            }
        }
        for (char terminal : FOLLOW[nonTerminal]) {
            if (FIRST[nonTerminal].count('ε')) {
                parsingTable[nonTerminal][terminal] = "ε";
            }
        }
    }
}

int main() {
    string nonTerminal;
    cout << "Enter the non-terminal: ";
    cin >> nonTerminal;
    cin.ignore();

    cout << "Enter productions for " << nonTerminal << " (separated by '|', example: abS|aSb): ";
    string input, prod;
    getline(cin, input);
    stringstream ss(input);

    vector<string> productions;
    while (getline(ss, prod, '|')) {
        productions.push_back(prod);
    }

    eliminateLeftRecursion(nonTerminal, productions);
    leftFactoring(nonTerminal, grammar[nonTerminal]);

    for (auto &[nt, prods] : grammar) {
        computeFIRST(nt);
        computeFOLLOW(nt);
    }

    constructParsingTable();

    cout << "\nFIRST sets:\n";
    for (auto &[nt, firstSet] : FIRST) {
        cout << "FIRST(" << nt << ") = { ";
        for (auto it = firstSet.begin(); it != firstSet.end(); ++it) {
            if (it != firstSet.begin()) cout << ", ";
            cout << *it;
        }
        cout << " }\n";
    }

    cout << "\nFOLLOW sets:\n";
    for (auto &[nt, followSet] : FOLLOW) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (auto it = followSet.begin(); it != followSet.end(); ++it) {
            if (it != followSet.begin()) cout << ", ";
            cout << *it;
        }
        cout << " }\n";
    }

    cout << "\nParsing Table:\n";
    for (auto &[nt, row] : parsingTable) {
        cout << nt << " | ";
        for (auto &[terminal, prod] : row) {
            cout << terminal << " -> " << prod << " | ";
        }
        cout << "\n";
    }

    return 0;
}