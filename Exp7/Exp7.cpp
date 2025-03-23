#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

int nt, t, top = 0;
string s[50], NT[10], T[10], st[50], pr[30];
char l[10][10], tr[10][10];

int searchnt(char a) {
    for (int i = 0; i < nt; i++)
        if (NT[i][0] == a)
            return i;
    return -1;
}

int searchter(string a) {
    for (int i = 0; i < t; i++)
        if (T[i] == a)
            return i;
    return -1;
}

void push(string a) { s[top++] = a; }
string pop() { return s[--top]; }

void installl(int a, int b) {
    if (l[a][b] == 'f') {
        l[a][b] = 't';
        push(T[b]);
        push(NT[a]);
    }
}

void installt(int a, int b) {
    if (tr[a][b] == 'f') {
        tr[a][b] = 't';
        push(T[b]);
        push(NT[a]);
    }
}

int main() {
    int i, k, j, n;
    char c;

    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter the productions one by one (e.g., E->E+T):\n";

    for (i = 0; i < n; i++) {
        getline(cin, pr[i]);
    }

    nt = 0;
    t = 0;

    for (i = 0; i < n; i++) {
        if (searchnt(pr[i][0]) == -1)
            NT[nt++] = pr[i][0];
    }

    for (i = 0; i < n; i++) {
        size_t pos = pr[i].find("->");
        if (pos == string::npos) {
            cout << "Error: Invalid production format. Use '->' as separator.\n";
            return 1;
        }
        for (j = pos + 2; j < pr[i].size(); j++) {
            if (pr[i][j] == 'i' && pr[i][j + 1] == 'd') {
                if (searchter("id") == -1)
                    T[t++] = "id";
                j++;
            }
            else if (searchnt(pr[i][j]) == -1 && searchter(string(1, pr[i][j])) == -1) {
                T[t++] = string(1, pr[i][j]);
            }
        }
    }

    for (i = 0; i < nt; i++)
        for (j = 0; j < t; j++)
            l[i][j] = tr[i][j] = 'f';
    for (i = 0; i < nt; i++) {
        for (j = 0; j < n; j++) {
            size_t pos = pr[j].find("->");
            if (pos == string::npos) continue;

            if (NT[searchnt(pr[j][0])] == NT[i]) {
                if (pr[j][pos + 2] == 'i' && pr[j][pos + 3] == 'd')  // Handle "id"
                    installl(searchnt(pr[j][0]), searchter("id"));
                else if (searchter(string(1, pr[j][pos + 2])) != -1)
                    installl(searchnt(pr[j][0]), searchter(string(1, pr[j][pos + 2])));
                else {
                    for (k = pos + 2; k < pr[j].size(); k++) {
                        if (searchnt(pr[j][k]) == -1) {
                            installl(searchnt(pr[j][0]), searchter(string(1, pr[j][k])));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (top != 0) {
        string b = pop(), c = pop();
        for (i = 0; i < n; i++)
            if (pr[i].find("->") != string::npos && pr[i][pr[i].find("->") + 2] == b[0])
                installl(searchnt(pr[i][0]), searchter(c));
    }

    for (i = 0; i < nt; i++) {
        cout << "Leading[" << NT[i] << "] = { ";
        bool first = true;
        for (j = 0; j < t; j++) {
            if (l[i][j] == 't') {
                if (!first) cout << ", ";
                cout << T[j];
                first = false;
            }
        }
        cout << " }\n";
    }

    for (i = 0; i < nt; i++) {
        for (j = 0; j < n; j++) {
            size_t pos = pr[j].find("->");
            if (pos == string::npos) continue;

            if (NT[searchnt(pr[j][0])] == NT[i]) {
                int len = pr[j].size();
                if (pr[j][len - 2] == 'i' && pr[j][len - 1] == 'd')  // Handle "id"
                    installt(searchnt(pr[j][0]), searchter("id"));
                else if (searchter(string(1, pr[j][len - 1])) != -1)
                    installt(searchnt(pr[j][0]), searchter(string(1, pr[j][len - 1])));
                else {
                    for (k = len - 1; k >= pos + 2; k--) {
                        if (searchnt(pr[j][k]) == -1) {
                            installt(searchnt(pr[j][0]), searchter(string(1, pr[j][k])));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (top != 0) {
        string b = pop(), c = pop();
        for (i = 0; i < n; i++)
            if (pr[i].find("->") != string::npos && pr[i][pr[i].find("->") + 2] == b[0])
                installt(searchnt(pr[i][0]), searchter(c));
    }

    for (i = 0; i < nt; i++) {
        cout << "Trailing[" << NT[i] << "] = { ";
        bool first = true;
        for (j = 0; j < t; j++) {
            if (tr[i][j] == 't') {
                if (!first) cout << ", ";
                cout << T[j];
                first = false;
            }
        }
        cout << " }\n";
    }

    return 0;
}