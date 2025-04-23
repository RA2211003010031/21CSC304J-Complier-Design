#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;

int labelCount = 100;  // Start label numbering from 100
int getLabel() {
    return labelCount++;
}

int main() {
    ifstream file("C:\\Users\\Admin\\Desktop\\input.txt"); // For Windows
    string line;
    vector<string> beforeBackpatching;
    vector<string> afterBackpatching;

    int elseLabel = 0, endLabel = 0;
    int currentLabel = 100;  // Keep track of current label

    while (getline(file, line)) {
        // Trim leading spaces (indentation)
        int indent = 0;
        while (indent < line.size() && isspace(line[indent])) indent++;
        string trimmed = line.substr(indent);

        if (trimmed.substr(0, 2) == "if") {
            // e.g., if a < b
            int trueLabel = getLabel();
            int falseLabel = getLabel();
            int afterLabel = getLabel();
            elseLabel = falseLabel;
            endLabel = afterLabel;

            string cond = trimmed.substr(3); // get 'a < b'

            // Before backpatching (with '?' for goto)
            beforeBackpatching.push_back("if " + cond + " goto ?");
            beforeBackpatching.push_back("goto ?");
            beforeBackpatching.push_back(to_string(trueLabel) + ":");

            // Add the corresponding backpatched line
            afterBackpatching.push_back("if " + cond + " goto " + to_string(trueLabel));
            afterBackpatching.push_back("goto " + to_string(falseLabel));
            afterBackpatching.push_back(to_string(trueLabel) + ":");
        }
        else if (trimmed.substr(0, 4) == "else") {
            // After backpatching
            afterBackpatching.push_back("goto " + to_string(endLabel));
            afterBackpatching.push_back(to_string(elseLabel) + ":");

            // Before backpatching (with '?' for goto)
            beforeBackpatching.push_back("goto ?");
            beforeBackpatching.push_back(to_string(elseLabel) + ":");
        }
        else {
            // treat as simple assignment or statement
            beforeBackpatching.push_back("   " + trimmed);
            afterBackpatching.push_back("   " + trimmed);
        }
    }

    if (endLabel > 0) {
        // Adding the end label
        afterBackpatching.push_back(to_string(endLabel) + ":");
    }

    // Output Before Backpatching
    cout << "--- Intermediate Code (Before Backpatching) ---\n";
    for (auto &line : beforeBackpatching) {
        cout << line << "\n";
    }

    // Output After Backpatching
    cout << "\n--- Intermediate Code (After Backpatching) ---\n";
    for (auto &line : afterBackpatching) {
        cout << line << "\n";
    }

    return 0;
}
