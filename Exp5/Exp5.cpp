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

void printGrammar(const vector<Production>& grammar) {
    cout << "\nGrammar Rules:\n";
    for (const auto& prod : grammar) {
        cout << prod.nonTerminal << " -> ";
        for (size_t i = 0; i < prod.productions.size(); i++) {
            cout << prod.productions[i];
            if (i != prod.productions.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

void eliminateLeftRecursion(vector<Production>& grammar) {
    cout << "\nEliminating Left Recursion...\n";
    vector<Production> newGrammar;

    for (auto prod : grammar) {
        string A = prod.nonTerminal;
        vector<string> alpha, beta;

        for (auto& p : prod.productions) {
            if (p.empty()) continue;
            if (p.substr(0, A.length()) == A) {
                alpha.push_back(p.substr(A.length()));
            } else {
                beta.push_back(p);
            }
        }

        if (!alpha.empty() && !beta.empty()) {
            Production newProd;
            newProd.nonTerminal = A + "'";
            for (auto& a : alpha) {
                newProd.productions.push_back(a + newProd.nonTerminal);
            }
            newProd.productions.push_back("epsilon");

            prod.productions.clear();
            for (auto& b : beta) {
                prod.productions.push_back(b + newProd.nonTerminal);
            }
            newGrammar.push_back(prod);
            newGrammar.push_back(newProd);
        } else {
            newGrammar.push_back(prod);
        }
    }
    grammar = newGrammar;
    printGrammar(grammar);
}

string commonPrefix(string a, string b) {
    string prefix;
    size_t len = min(a.length(), b.length());
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) break;
        prefix += a[i];
    }
    return prefix;
}

void leftFactorGrammar(vector<Production>& grammar) {
    cout << "\nPerforming Left Factoring...\n";
    vector<Production> newGrammar;

    for (auto prod : grammar) {
        if (prod.productions.size() <= 1) {
            newGrammar.push_back(prod);
            continue;
        }

        string prefix = prod.productions[0];
        for (size_t i = 1; i < prod.productions.size(); i++) {
            prefix = commonPrefix(prefix, prod.productions[i]);
        }

        if (prefix.empty() || prefix == prod.productions[0]) {
            newGrammar.push_back(prod);
            continue;
        }

        Production newProd;
        newProd.nonTerminal = prod.nonTerminal + "'";

        vector<string> factored;
        for (auto& rule : prod.productions) {
            if (rule.substr(0, prefix.length()) == prefix) {
                string suffix = rule.substr(prefix.length());
                newProd.productions.push_back(suffix.empty() ? "epsilon" : suffix);
            } else {
                factored.push_back(rule);
            }
        }

        prod.productions.clear();
        prod.productions.push_back(prefix + newProd.nonTerminal);
        for (auto& r : factored) {
            prod.productions.push_back(r);
        }

        newGrammar.push_back(prod);
        newGrammar.push_back(newProd);
    }
    grammar = newGrammar;
    printGrammar(grammar);
}

void computeFIRST() {
    cout << "\nComputing FIRST Sets...\n";
    for (auto& prod : grammar) {
        FIRST[prod.nonTerminal];
    }
    
    bool changed;
    do {
        changed = false;
        for (auto& prod : grammar) {
            set<string>& firstSet = FIRST[prod.nonTerminal];
            for (auto& rule : prod.productions) {
                if (rule.empty()) continue;
                if (rule == "epsilon") {
                    changed |= firstSet.insert("epsilon").second;
                } else if (!isupper(rule[0])) {
                    changed |= firstSet.insert(string(1, rule[0])).second;
                } else {
                    size_t i = 0;
                    bool hasEpsilon = true;
                    while (i < rule.length() && hasEpsilon) {
                        if (!isupper(rule[i])) {
                            changed |= firstSet.insert(string(1, rule[i])).second;
                            hasEpsilon = false;
                        } else {
                            string nt(1, rule[i]);
                            auto& nextFirst = FIRST[nt];
                            for (auto& f : nextFirst) {
                                if (f != "epsilon")
                                    changed |= firstSet.insert(f).second;
                            }
                            hasEpsilon = (nextFirst.count("epsilon") > 0);
                            i++;
                        }
                    }
                    if (hasEpsilon && i >= rule.length()) {
                        changed |= firstSet.insert("epsilon").second;
                    }
                }
            }
        }
    } while (changed);

    for (auto& entry : FIRST) {
        cout << "FIRST(" << entry.first << ") = { ";
        for (auto& val : entry.second) {
            cout << val << " ";
        }
        cout << "}\n";
    }
}

// Fixed FOLLOW computation
void computeFOLLOW() {
    cout << "\nComputing FOLLOW Sets...\n";
    
    // Initialize FOLLOW sets for all non-terminals
    for (auto& prod : grammar) {
        FOLLOW[prod.nonTerminal];
    }
    
    // Add $ to start symbol (assuming first non-terminal is start symbol)
    if (!grammar.empty()) {
        FOLLOW[grammar[0].nonTerminal].insert("$");
    }

    bool changed;
    do {
        changed = false;
        for (auto& prod : grammar) {
            string A = prod.nonTerminal;
            for (auto& rule : prod.productions) {
                if (rule.empty()) continue;
                
                // Process each character in the production
                for (size_t i = 0; i < rule.length(); i++) {
                    if (isupper(rule[i])) {
                        // Extract the non-terminal (might be more than one character, e.g., S')
                        string nt;
                        size_t j = i;
                        while (j < rule.length() && (isupper(rule[j]) || rule[j] == '\'')) {
                            nt += rule[j];
                            j++;
                        }
                        
                        if (!nt.empty()) {
                            set<string>& followSet = FOLLOW[nt];
                            
                            // Look ahead to next symbols
                            bool hasEpsilon = true;
                            size_t k = j;
                            
                            while (k < rule.length() && hasEpsilon) {
                                if (!isupper(rule[k])) {
                                    // Terminal found
                                    changed |= followSet.insert(string(1, rule[k])).second;
                                    hasEpsilon = false;
                                } else {
                                    // Extract next non-terminal
                                    string nextNT;
                                    while (k < rule.length() && (isupper(rule[k]) || rule[k] == '\'')) {
                                        nextNT += rule[k];
                                        k++;
                                    }
                                    auto& nextFirst = FIRST[nextNT];
                                    for (auto& f : nextFirst) {
                                        if (f != "epsilon") {
                                            changed |= followSet.insert(f).second;
                                        }
                                    }
                                    hasEpsilon = (nextFirst.count("epsilon") > 0);
                                }
                            }
                            
                            // If epsilon is possible or we reached the end,
                            // add FOLLOW(A) to FOLLOW(nt)
                            if (hasEpsilon || j >= rule.length()) {
                                for (auto& f : FOLLOW[A]) {
                                    changed |= followSet.insert(f).second;
                                }
                            }
                            i = j - 1; // Move past the non-terminal we just processed
                        }
                    }
                }
            }
        }
    } while (changed);

    for (auto& entry : FOLLOW) {
        cout << "FOLLOW(" << entry.first << ") = { ";
        for (auto& val : entry.second) {
            cout << val << " ";
        }
        cout << "}\n";
    }
}

void constructParsingTable() {
    cout << "\nConstructing Predictive Parsing Table...\n";
    parsingTable.clear();

    for (auto& prod : grammar) {
        string A = prod.nonTerminal;
        for (auto& rule : prod.productions) {
            if (rule.empty()) continue;

            set<string> firstOfRule;
            if (rule == "epsilon") {
                firstOfRule.insert("epsilon");
            } else {
                size_t i = 0;
                bool hasEpsilon = true;
                while (i < rule.length() && hasEpsilon) {
                    if (!isupper(rule[i])) {
                        firstOfRule.insert(string(1, rule[i]));
                        hasEpsilon = false;
                    } else {
                        string nt;
                        while (i < rule.length() && (isupper(rule[i]) || rule[i] == '\'')) {
                            nt += rule[i];
                            i++;
                        }
                        for (auto& f : FIRST[nt]) {
                            if (f != "epsilon") {
                                firstOfRule.insert(f);
                            }
                        }
                        hasEpsilon = (FIRST[nt].count("epsilon") > 0);
                        i--; // adjust for loop increment
                    }
                    i++;
                }
                if (hasEpsilon && i >= rule.length()) {
                    firstOfRule.insert("epsilon");
                }
            }

            for (auto& f : firstOfRule) {
                if (f != "epsilon") {
                    if (parsingTable.count({A, f}) && parsingTable[{A, f}] != rule) {
                        cout << "Warning: Conflict detected at M[" << A << ", " << f << "]\n";
                    }
                    parsingTable[{A, f}] = rule;
                }
            }

            if (firstOfRule.count("epsilon") > 0) {
                for (auto& f : FOLLOW[A]) {
                    if (parsingTable.count({A, f}) && parsingTable[{A, f}] != rule) {
                        cout << "Warning: Conflict detected at M[" << A << ", " << f << "]\n";
                    }
                    parsingTable[{A, f}] = rule;
                }
            }
        }
    }

    cout << "\nParsing Table:\n";
    for (auto& entry : parsingTable) {
        cout << "M[" << entry.first.first << ", " << entry.first.second << "] = " 
             << entry.second << endl;
    }
}

int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    if (n <= 0) {
        cout << "Invalid number of productions\n";
        return 1;
    }
    cin.ignore();

    grammar.resize(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter the non-terminal: ";
        getline(cin, grammar[i].nonTerminal);
        if (grammar[i].nonTerminal.empty() || !isupper(grammar[i].nonTerminal[0])) {
            cout << "Invalid non-terminal\n";
            return 1;
        }

        cout << "Enter the productions (separated by '|'): ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string prod;
        while (getline(ss, prod, '|')) {
            prod.erase(0, prod.find_first_not_of(" \t"));
            prod.erase(prod.find_last_not_of(" \t") + 1);
            if (!prod.empty())
                grammar[i].productions.push_back(prod);
        }
        if (grammar[i].productions.empty()) {
            cout << "No valid productions entered\n";
            return 1;
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