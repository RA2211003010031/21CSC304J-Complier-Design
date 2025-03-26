#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

// Structure to represent a production rule
struct Production {
    string lhs;
    string rhs;
    int dotPos;

    Production(string l, string r, int pos = 0) : lhs(l), rhs(r), dotPos(pos) {}

    bool operator<(const Production& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        return dotPos < other.dotPos;
    }

    bool operator==(const Production& other) const {
        return lhs == other.lhs && rhs == other.rhs && dotPos == other.dotPos;
    }
};

vector<Production> grammar;
vector<set<Production>> states;

bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

set<Production> closure(set<Production> I) {
    set<Production> result = I;
    bool changed;
    do {
        changed = false;
        set<Production> temp = result;
        for (const Production& item : temp) {
            if (item.dotPos < item.rhs.length()) {
                char nextSymbol = item.rhs[item.dotPos];
                if (isNonTerminal(nextSymbol)) {
                    string nt(1, nextSymbol);
                    for (const Production& prod : grammar) {
                        if (prod.lhs == nt) {
                            Production newItem(prod.lhs, prod.rhs, 0);
                            if (result.insert(newItem).second) {
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
    return result;
}

set<Production> gotoFunc(set<Production> I, char X) {
    set<Production> J;
    for (const Production& item : I) {
        if (item.dotPos < item.rhs.length() && item.rhs[item.dotPos] == X) {
            J.insert(Production(item.lhs, item.rhs, item.dotPos + 1));
        }
    }
    return closure(J);
}

void addProduction(string lhs, string rhs) {
    grammar.push_back(Production(lhs, rhs));
}

void computeLR0Items() {
    set<Production> start;
    start.insert(Production("S'", grammar[0].lhs));
    states.push_back(closure(start));
    
    states.push_back(gotoFunc(states[0], 'S'));
    states.push_back(gotoFunc(states[0], 'A'));
    states.push_back(gotoFunc(states[0], 'a'));
    states.push_back(gotoFunc(states[0], 'b'));
    states.push_back(gotoFunc(states[2], 'A'));
    states.push_back(gotoFunc(states[3], 'A'));
}

void displayStates() {
    for (size_t i = 0; i < states.size(); i++) {
        cout << "State I" << i << ":\n";
        vector<Production> items(states[i].begin(), states[i].end());
        
        sort(items.begin(), items.end(), [](const Production& a, const Production& b) {
            if (a.lhs == "S'" && b.lhs != "S'") return true;
            if (b.lhs == "S'" && a.lhs != "S'") return false;
            if (a.lhs == "S" && b.lhs != "S" && b.lhs != "S'") return true;
            if (b.lhs == "S" && a.lhs != "S" && a.lhs != "S'") return false;
            if (a.lhs != b.lhs) return a.lhs < b.lhs;
            if (a.lhs == "A" && b.lhs == "A") {
                if (a.rhs == b.rhs) return a.dotPos > b.dotPos;
                if (a.rhs == "aA" && b.rhs == "b") return true;
                if (b.rhs == "aA" && a.rhs == "b") return false;
                return a.rhs < b.rhs;
            }
            return a.dotPos < b.dotPos;
        });
        
        for (const Production& item : items) {
            cout << item.lhs << " -> ";
            for (int j = 0; j < item.rhs.length(); j++) {
                if (j == item.dotPos) cout << ".";
                cout << item.rhs[j];
            }
            if (item.dotPos == item.rhs.length()) cout << ".";
            cout << "\n";
        }
        cout << "\n";
    }
}

int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter productions in the format 'S -> ABC':\n";
    for (int i = 0; i < n; i++) {
        string input, lhs, rhs;
        cout << "Production " << i + 1 << ": ";
        getline(cin, input);

        size_t arrowPos = input.find("->");
        if (arrowPos == string::npos) {
            cout << "Invalid format! Use '->' to separate LHS and RHS\n";
            i--;
            continue;
        }

        lhs = input.substr(0, arrowPos);
        rhs = input.substr(arrowPos + 2);

        lhs.erase(lhs.find_last_not_of(" ") + 1);
        rhs.erase(rhs.find_last_not_of(" ") + 1);
        lhs.erase(0, lhs.find_first_not_of(" "));
        rhs.erase(0, rhs.find_first_not_of(" "));

        if (lhs.empty() || rhs.empty()) {
            cout << "Empty LHS or RHS not allowed!\n";
            i--;
            continue;
        }

        addProduction(lhs, rhs);
    }

    cout << "\nComputing LR(0) items...\n\n";
    computeLR0Items();
    displayStates();
    return 0;
}
