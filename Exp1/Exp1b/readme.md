# Lab Exercise: Implementation of Lexical Analyzer using FLEX

## Objective
The objective of this lab exercise is to implement a **Lexical Analyzer** using **FLEX**. The exercise includes a series of programs that demonstrate different lexical analysis tasks such as counting characters, words, lines, identifying tokens, and checking conditions.

## Lab Exercise Sets

### **SET 1: Flex Program to Find a Vowel or Not**
- **Description**: Checks if a given character is a vowel (a, e, i, o, u) or not.
- **Objective**: Use regular expressions to match vowels.

### **SET 2: Flex Program to Count the Number of Lines and Characters**
- **Description**: Counts the total number of lines and characters in a file.
- **Objective**: Process files and count specific text elements.

### **SET 3: Flex Code to Count Total Number of Characters**
- **Description**: Counts the total number of characters in the input file.
- **Objective**: Traverse input and count characters.

### **SET 4: Flex Program to Count Number of Words**
- **Description**: Counts the total number of words in the input file.
- **Objective**: Define word boundaries and process them.

### **SET 5: Flex Program to Count the Number of Lines, Spaces, and Tabs**
- **Description**: Counts the number of lines, spaces, and tabs in the input.
- **Objective**: Differentiate between whitespace characters.

### **SET 6: Flex Program to Count the Frequency of the Given Word in a File**
- **Description**: Counts how many times a specific word appears in the file.
- **Objective**: Search for and count occurrences of a word.

### **SET 7: Flex Program to Add Line Numbers to a Given File**
- **Description**: Adds line numbers to each line in the file.
- **Objective**: Manipulate file content and add line numbers.

### **SET 8: Flex Program to Find an Uppercase and Lowercase Letter**
- **Description**: Checks if a given character is uppercase or lowercase.
- **Objective**: Classify characters based on case.

### **SET 9: Flex Program to Check if the Given String is a Digit or a Word**
- **Description**: Checks if a string is a number (digit) or a word (alphabetic).
- **Objective**: Differentiate between numbers and words.

### **SET 10: Flex Program to Check if It is a Well-Formed Parenthesis or Not**
- **Description**: Checks if the parentheses in the input are balanced and well-formed.
- **Objective**: Check for balanced symbols using FLEX.

### **SET 11: Flex Program to Implement Positive Closure**
- **Description**: Demonstrates the use of positive closure in regular expressions.
- **Objective**: Implement regular expression features like positive closure.

### **SET 12: Flex Code to Count Total Number of Tokens**
- **Description**: Counts the total number of tokens in the file.
- **Objective**: Identify and count different tokens.

### **SET 13: Flex Program to Identify and Count Positive and Negative Numbers**
- **Description**: Identifies and counts positive and negative numbers.
- **Objective**: Match and count numbers with different signs.

### **SET 14: Flex Program to Identify Operators or Not**
- **Description**: Identifies if characters are operators (+, -, *, /, etc.).
- **Objective**: Identify operators in a text.

### **SET 15: Flex Program to Identify the Identifier or Not**
- **Description**: Checks if a string is a valid identifier (starting with a letter).
- **Objective**: Check for valid identifiers.

## Requirements

- **FLEX**: A tool for generating lexical analyzers.
- **GCC**: A C/C++ compiler.
- **Linux/Unix**: Recommended environment for running FLEX programs.

## Instructions

1. Install **FLEX** and **GCC** on your system.
2. Navigate to the directory containing the source files.
3. For each set, follow these steps:
   - Run `flex <filename>.l` to generate the lexical analyzer.
   - Compile the C code with `gcc lex.yy.c -o <output_name>`.
   - Execute the program with `./<output_name>`.

## Example

For **SET 1**, to check if a character is a vowel:
```bash
flex vowel_check.l
gcc lex.yy.c -o vowel_check
./vowel_check