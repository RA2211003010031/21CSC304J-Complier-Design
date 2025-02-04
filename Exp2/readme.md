Below is a well-structured `README.md` file for the provided code. This README explains the purpose of the program, its functionality, and how to use it.

---

# Lexical Analyzer (Tokenizer) for C Code

## 📌 Overview

This project implements a **Lexical Analyzer** (or Tokenizer) using **Lex** (a lexical analyzer generator). The program reads a C source code file and tokenizes it into various categories such as keywords, identifiers, data types, operators, numbers, and special symbols. It also handles preprocessor directives and header files.

The output includes:
- **Token Number**: A unique identifier for each token.
- **Line Number**: The line in the source code where the token appears.
- **Token Type**: The category of the token (e.g., Keyword, Identifier, Operator, etc.).
- **Lexeme**: The actual text of the token.

This tool is useful for understanding the structure of C code and can serve as a foundation for building compilers or interpreters.

---

## 🏗 Project Workflow

1. **Input**: The program takes a C source code file as input.
2. **Tokenization**: It scans the file line by line and categorizes each token based on predefined rules.
3. **Output**: The tokens are printed in a tabular format with their token number, line number, token type, and lexeme.

---

## 🛠 Installation

### ⿡ Prerequisites
- **Flex**: Ensure that Flex (Fast Lexical Analyzer Generator) is installed on your system. You can install it using the following commands:

  - **Ubuntu/Debian**:
    ```bash
    sudo apt-get update
    sudo apt-get install flex
    ```

  - **macOS**:
    ```bash
    brew install flex
    ```

  - **Windows**: Install Flex through [Cygwin](https://www.cygwin.com/) or [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

- **GCC Compiler**: Required to compile the generated C code from Flex.

### ⿡ Steps to Run the Program

1. **Clone the Repository** (if applicable):
   ```bash
   git clone https://github.com/your-repo/lexical-analyzer.git
   cd lexical-analyzer
   ```

2. **Generate the Lexer**:
   Use Flex to generate the lexer from the `.l` file:
   ```bash
   flex lexical_analyzer.l
   ```

3. **Compile the Generated Code**:
   Compile the generated `lex.yy.c` file using GCC:
   ```bash
   gcc lex.yy.c -o lexical_analyzer -lfl
   ```

4. **Run the Program**:
   Execute the compiled program and provide the path to your C source code file:
   ```bash
   ./lexical_analyzer
   ```
   Example:
   ```bash
   Enter the filename:
   example.c
   ```

5. **View the Output**:
   The program will display the tokens in a tabular format:
   ```plaintext
   TOKEN NO   LINE NO   TOKEN TYPE           LEXEME
   1          1         Header File          #include<stdio.h>
   2          2         Data Type            int
   3          2         Identifier           main
   4          2         Special Symbol       (
   ...
   ```

---

## 🚀 Usage

### ⿡ Input File Example
Here’s an example of a simple C file (`example.c`) that you can use as input:

```c
#include <stdio.h>

int main() {
    int a = 10;
    float b = 20.5;
    printf("Hello, World!\n");
    return 0;
}
```

### ⿡ Sample Output
For the above input, the program will produce the following output:

```plaintext
TOKEN NO   LINE NO   TOKEN TYPE           LEXEME
1          1         Header File          #include<stdio.h>
2          3         Data Type            int
3          3         Identifier           main
4          3         Special Symbol       (
5          3         Special Symbol       )
6          3         Special Symbol       {
7          4         Data Type            int
8          4         Identifier           a
9          4         Assignment Operator  =
10         4         Number (int)         10
...
```

---

## 🧩 Features

- **Token Categories**:
  - Keywords: `if`, `else`, `while`, `for`, `return`, etc.
  - Data Types: `int`, `float`, `char`, `double`, `void`, etc.
  - Identifiers: Variables, function names, etc.
  - Numbers: Integer and floating-point literals.
  - Operators: Arithmetic (`+`, `-`, `*`, `/`), relational (`==`, `<`, `>`), and assignment (`=`).
  - Special Symbols: `{`, `}`, `(`, `)`, `[`, `]`, `,`, `;`.
  - Preprocessor Directives: `#include`, `#define`, etc.
  - Header Files: `<stdio.h>`, `"myheader.h"`, etc.

- **Error Handling**:
  - Unrecognized characters are flagged as "Unknown".
  - Missing or invalid files are handled gracefully.