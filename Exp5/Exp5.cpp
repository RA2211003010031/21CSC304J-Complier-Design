#include <iostream>
#include <vector>
#include <map>
#include <set>
<<<<<<< HEAD
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
=======
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
>>>>>>> d17d9354565f3ef4c651af869d9ec02227a12a2f
                        }
                    }
                }
            }
        }
    } while (changed);
<<<<<<< HEAD

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
=======
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
>>>>>>> d17d9354565f3ef4c651af869d9ec02227a12a2f
    }
}

int main() {
<<<<<<< HEAD
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

=======
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

>>>>>>> d17d9354565f3ef4c651af869d9ec02227a12a2f
    return 0;
}