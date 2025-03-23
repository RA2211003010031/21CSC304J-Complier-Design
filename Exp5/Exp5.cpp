#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

map<string, vector<string>> grammar;
map<string, set<char>> firstSets, followSets;

void eliminateLeftRecursion(string nonTerminal, vector<string> productions) {
    vector<string> alpha, beta;
    string newNonTerminal = nonTerminal + "'";

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

    vector<string> newBeta, newAlpha;
    for (string b : beta) newBeta.push_back(b + newNonTerminal);
    for (string a : alpha) newAlpha.push_back(a + newNonTerminal);
    newAlpha.push_back("ε");

    grammar[nonTerminal] = newBeta;
    grammar[newNonTerminal] = newAlpha;
}

void leftFactoring(string nonTerminal, vector<string> productions) {
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
    vector<string> newBeta;

    for (string prod : productions) {
        string suffix = prod.substr(prefix.size());
        if (suffix.empty()) suffix = "ε";
        newBeta.push_back(suffix);
    }

    grammar[nonTerminal] = {prefix + newNonTerminal};
    grammar[newNonTerminal] = newBeta;
}

void computeFirst(string nonTerminal, set<char>& visited) {
    if (firstSets[nonTerminal].empty()) {
        for (string prod : grammar[nonTerminal]) {
            if (!isupper(prod[0]) || prod[0] == 'ε') {
                firstSets[nonTerminal].insert(prod[0]);
            } else if (visited.find(prod[0]) == visited.end()) {
                visited.insert(prod[0]);
                computeFirst(string(1, prod[0]), visited);
                firstSets[nonTerminal].insert(firstSets[string(1, prod[0])].begin(),
                                             firstSets[string(1, prod[0])].end());
                visited.erase(prod[0]);
            }
        }
    }
}

void computeFollow(string nonTerminal, set<char>& visited) {
    if (followSets[nonTerminal].empty()) {
        if (nonTerminal == grammar.begin()->first) 
            followSets[nonTerminal].insert('$'); 

        for (auto& rule : grammar) {
            string lhs = rule.first;
            for (string prod : rule.second) {
                for (size_t i = 0; i < prod.size(); i++) {
                    if (string(1, prod[i]) == nonTerminal) {
                        if (i + 1 < prod.size()) {
                            if (!isupper(prod[i + 1])) {
                                followSets[nonTerminal].insert(prod[i + 1]);
                            } else if (visited.find(prod[i + 1]) == visited.end()) {
                                visited.insert(prod[i + 1]);
                                followSets[nonTerminal].insert(firstSets[string(1, prod[i + 1])].begin(),
                                                             firstSets[string(1, prod[i + 1])].end());
                                followSets[nonTerminal].erase('ε');
                                visited.erase(prod[i + 1]);
                            }
                        } else if (lhs != nonTerminal) {
                            if (visited.find(lhs[0]) == visited.end()) {
                                visited.insert(lhs[0]);
                                computeFollow(lhs, visited);
                                followSets[nonTerminal].insert(followSets[lhs].begin(), followSets[lhs].end());
                                visited.erase(lhs[0]);
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int numNonTerminals;

    cout << "Enter the number of non-terminals: ";
    cin >> numNonTerminals;
    cin.ignore();

    for (int i = 0; i < numNonTerminals; i++) {
        string nonTerminal;
        int numProductions;
        cout << "Enter non-terminal: ";
        cin >> nonTerminal;
        cout << "Enter the number of productions: ";
        cin >> numProductions;
        cin.ignore();

        vector<string> productions(numProductions);
        cout << "Enter productions separated by space: ";
        for (int j = 0; j < numProductions; j++) cin >> productions[j];

        eliminateLeftRecursion(nonTerminal, productions);
        leftFactoring(nonTerminal, grammar[nonTerminal]);
    }

    // Compute First sets
    for (auto& rule : grammar) {
        set<char> visited;
        computeFirst(rule.first, visited);
    }

    // Compute Follow sets
    for (auto& rule : grammar) {
        set<char> visited;
        computeFollow(rule.first, visited);
    }

    // Print First sets
    cout << "\nFirst Sets:\n";
    for (auto& rule : firstSets) {
        cout << "First(" << rule.first << ") = { ";
        for (char c : rule.second) cout << c << " ";
        cout << "}\n";
    }

    // Print Follow sets
    cout << "\nFollow Sets:\n";
    for (auto& rule : followSets) {
        cout << "Follow(" << rule.first << ") = { ";
        for (char c : rule.second) cout << c << " ";
        cout << "}\n";
    }

    return 0;
}