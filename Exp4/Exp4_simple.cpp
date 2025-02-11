#include <iostream>
#include <string>

using namespace std;

void removeLeftRecursion(string A, string alpha, string beta) {
    cout << "\nAfter removing left recursion:\n";
    cout << A << " -> " << beta << A << "'\n";
    cout << A << "' -> " << alpha << A << "' | ε\n";
}

void performLeftFactoring(string A, string common, string beta1, string beta2) {
    cout << "\nAfter left factoring:\n";
    cout << A << " -> " << common << A << "'\n";
    cout << A << "' -> " << beta1 << " | " << beta2 << endl;
}

bool hasLeftRecursion(string A, string part) {
    return (A == part);
}

bool hasCommonPrefix(string part1, string part2, string &common) {
    int i = 0;
    while (i < part1.length() && i < part2.length() && part1[i] == part2[i]) {
        common += part1[i];
        i++;
    }
    return !common.empty();
}

int main() {
    string A, part1, part2;
    cout << "Enter the non-terminal: ";
    cin >> A;
    cout << "Enter the first production rule part: ";
    cin >> part1;
    cout << "Enter the second production rule part: ";
    cin >> part2;

    if (hasLeftRecursion(A, part1)) {
        string alpha, beta;
        cout << "Detected Left Recursion!\n";
        cout << "Enter α: ";
        cin >> alpha;
        cout << "Enter β: ";
        cin >> beta;
        removeLeftRecursion(A, alpha, beta);
    } 
    else {
        string common = "";
        if (hasCommonPrefix(part1, part2, common)) {
            string beta1 = part1.substr(common.length());
            string beta2 = part2.substr(common.length());
            cout << "Detected Left Factoring!\n";
            performLeftFactoring(A, common, beta1, beta2);
        } 
        else {
            cout << "No transformation needed.\n";
        }
    }

    return 0;
}