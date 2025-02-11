#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

vector<string> split(const string& str) {
    vector<string> tokens;
    string token;
    for (char c : str) {
        if (c == ' ') {
            if (!token.empty()) tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

void eliminateLeftRecursion(map<char, vector<string>>& grammar) {
    map<char, vector<string>> newGrammar;

    for (auto& [nonTerminal, productions] : grammar) {
        vector<string> alpha;
        vector<string> beta;

        for (const string& prod : productions) {
            if (prod[0] == nonTerminal) {
                alpha.push_back(prod.substr(1));
            } else {
                beta.push_back(prod);
            }
        }

        if (!alpha.empty()) {
            char newNonTerminal = nonTerminal + '\'';
            grammar[nonTerminal].clear();

            
            for (const string& b : beta) {
                grammar[nonTerminal].push_back(b + newNonTerminal);
            }

            
            for (const string& a : alpha) {
                newGrammar[newNonTerminal].push_back(a + newNonTerminal);
            }

           
            newGrammar[newNonTerminal].push_back("#"); 
        }
    }

    
    for (auto& [nonTerminal, productions] : newGrammar) {
        grammar[nonTerminal] = productions;
    }
}

void leftFactoring(map<char, vector<string>>& grammar) {
    map<char, vector<string>> newGrammar;

    for (auto& [nonTerminal, productions] : grammar) {
        map<string, vector<string>> prefixMap;

        for (const string& prod : productions) {
            string prefix = "";
            if (!prod.empty()) {
                prefix += prod[0];
            }
            prefixMap[prefix].push_back(prod);
        }

        for (auto& [prefix, prods] : prefixMap) {
            if (prods.size() > 1) {
                char newNonTerminal = nonTerminal + '\'';
                grammar[nonTerminal].erase(remove(grammar[nonTerminal].begin(), grammar[nonTerminal].end(), prods[0]), grammar[nonTerminal].end());

                grammar[nonTerminal].push_back(prefix + newNonTerminal);

                for (const string& prod : prods) {
                    string suffix = prod.substr(prefix.size());
                    if (suffix.empty()) suffix = "#";
                    newGrammar[newNonTerminal].push_back(suffix);
                }
            }
        }
    }
    for (auto& [nonTerminal, productions] : newGrammar) {
        grammar[nonTerminal] = productions;
    }
}

void printGrammar(const map<char, vector<string>>& grammar) {
    for (auto& [nonTerminal, productions] : grammar) {
        cout << nonTerminal << " -> ";
        for (size_t i = 0; i < productions.size(); ++i) {
            cout << productions[i];
            if (i != productions.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

int main() {
    map<char, vector<string>> grammar = {
        {'E', {"E+T", "T"}},
        {'T', {"T*F", "F"}},
        {'F', {"(E)", "id"}}
    };

    cout << "Original Grammar:" << endl;
    printGrammar(grammar);

    eliminateLeftRecursion(grammar);
    cout << "\nAfter Eliminating Left Recursion:" << endl;
    printGrammar(grammar);

    leftFactoring(grammar);
    cout << "\nAfter Left Factoring:" << endl;
    printGrammar(grammar);

    return 0;
}