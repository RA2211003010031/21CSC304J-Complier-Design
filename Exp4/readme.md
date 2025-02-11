# Experiment 4: Elimination of Left Recursion and Left Factoring in C++

This experiment involves writing programs in C++ to eliminate left recursion and perform left factoring on given grammar productions. The goal is to transform the grammar into a form that is suitable for parsing by LL(1) parsers.

## Files

### [Exp4_simple.cpp](#file:Exp4_simple.cpp-context)

This file contains a simple implementation to detect and eliminate left recursion and perform left factoring on a given grammar production.

#### Functions

- `removeLeftRecursion(string A, string alpha, string beta)`: Eliminates left recursion from the given production.
- `performLeftFactoring(string A, string common, string beta1, string beta2)`: Performs left factoring on the given production.
- `hasLeftRecursion(string A, string part)`: Checks if the given production has left recursion.
- `hasCommonPrefix(string part1, string part2, string &common)`: Checks if the given productions have a common prefix.

#### Main Function

- Prompts the user to enter a non-terminal and two parts of a production rule.
- Detects and eliminates left recursion if present.
- Detects and performs left factoring if common prefixes are found.
- Outputs the transformed grammar.

### [Exp4_Medium.cpp](#file:Exp4_Medium.cpp-context)

This file contains a more comprehensive implementation to eliminate left recursion and perform left factoring on a given grammar represented as a map of non-terminals to their productions.

#### Functions

- `vector<string> split(const string& str)`: Splits a string into tokens based on spaces.
- `void eliminateLeftRecursion(map<char, vector<string>>& grammar)`: Eliminates left recursion from the given grammar.
- `void leftFactoring(map<char, vector<string>>& grammar)`: Performs left factoring on the given grammar.
- `void printGrammar(const map<char, vector<string>>& grammar)`: Prints the given grammar in a readable format.

#### Main Function

- Initializes a sample grammar.
- Prints the original grammar.
- Eliminates left recursion and prints the transformed grammar.
- Performs left factoring and prints the final transformed grammar.

## Usage

1. Compile the C++ files using a C++ compiler.
2. Run the executables to see the transformations applied to the given grammar.

## Example

For `Exp4_simple.cpp`, the program will prompt for input and display the transformed grammar based on the input provided.

For `Exp4_Medium.cpp`, the program will display the original grammar, the grammar after eliminating left recursion, and the grammar after performing left factoring.