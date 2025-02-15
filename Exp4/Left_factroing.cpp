#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

struct Production {
    string nonTerminal;
    vector<string> productions;
};

// Function to find the common prefix of two strings
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

// Function to perform left factoring
void leftFactorGrammar(vector<Production> &grammar) {
    vector<Production> newGrammar;
    
    for (auto &prod : grammar) {
        string prefix = prod.productions[0];
        for (size_t i = 1; i < prod.productions.size(); i++) {
            prefix = commonPrefix(prefix, prod.productions[i]);
        }
        
        if (prefix.length() == 0) {
            newGrammar.push_back(prod);
            continue;
        }
        
        Production newProd;
        newProd.nonTerminal = prod.nonTerminal + "'";
        vector<string> remaining;
        
        for (string rule : prod.productions) {
            if (rule.substr(0, prefix.length()) == prefix) {
                string suffix = rule.substr(prefix.length());
                if (suffix == "") suffix = "epsilon";
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
    
    // Print the left-factored grammar
    cout << "\nLeft Factored Grammar:\n";
    for (auto &prod : newGrammar) {
        cout << prod.nonTerminal << " -> ";
        for (size_t i = 0; i < prod.productions.size(); i++) {
            cout << prod.productions[i];
            if (i != prod.productions.size() - 1) cout << " | ";
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
            grammar[i].productions.push_back(production);
        }
    }

    leftFactorGrammar(grammar);
    return 0;
}
