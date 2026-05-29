# Custom Language Compiler CVM++

This project is a compiler and stack-based virtual machine built from scratch in C++. Takes a source file written in the custom language, runs it through a full compiler pipeline — Lexer, Parser, Semantic Analyzer, and Bytecode Compiler — and executes it on a custom Virtual Machine.

Broadly the compiler follows C-style syntax with some differences that should be clear from the given description. 


## Language Features

- Data types: `int` and `bool`
- Strictly typed — no implicit type conversions
- Arithmetic operators: `+`, `-`, `*`, `/`, `%`
- Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical operators: `&&`, `||`, `!`
- Assignment: `=` (including chained assignment `a = b = 5`)
- Control flow: `if`, `if/else`, `while`
- Builtin keyword for taking input from terminal: `input()` - always reads an integer
- Builtin keyword for output: `print()`
- Block scoping with `{` and `}`
- Single-line C-style comments with `//`

---

## Compiler Pipeline
### The steps of compilation

1. Lexical analysis: Breaks source file into tokens
2. Parser: Builds an Abstract Syntax Tree from the token stream
3. Semantic Analyzer: Type checking, scope resolution, error reporting
4. Bytecode Compiler: Converts AST to stack-based bytecode
5. Virtual Machine: Executes bytecode

---

## Compilation and Execution

### Requirements

- `g++` with C++17 support
- `make`

### Build

```bash
make compiler
```
### To clean all build artifacts
```bash
make clean
```

### Run

```bash
./compiler <file_name>
```

### Example

```bash
./compiler program.cvm
```
---

## Project Structure
The directory `src` contains all the source files and the directory `include` contains all the header files, and `Makefile` is in the parent directory.
1. `Lexer.h/.cpp`:  Lexical analyzer — tokenizes source input into stream of tokens
2. `AST.h/.cpp`: AST node definitions and print utilities
3. `Parser.h/.cpp`: Pratt parser — builds AST from token stream
4. `SemanticAnalyzer.h/.cpp`: Type checking and scope resolution
5. `Instruction.h/.cpp`: Contains the instruction set description supported by the Virtual Machine
6. `ByteCodeCompiler.h/.cpp`: Compiles AST to bytecode instructions
7. `VirtualMachine.h/.cpp`: Stack-based VM that executes bytecode
8. `ErrorHandler.h/.cpp`: Centralized error reporting with line numbers
9. `main.cpp`: The compilation pipeline, it uses all the source files to compile the program given by the user

---

## Error Handling

Errors are reported with line numbers and descriptive messages at every stage, here are few examples:
- Lexical Error:  
```txt
Lexical Error (Line 1):
        @
        Unexpected character '@'
```
- Syntax Error:
```txt
Syntax Error (Line 2):
        x=10;
        Expected ';' before 'x'
```
- Semantic Error:
```txt
Semantic Error (Line 3):
        x=10;
        'x' was not declared in this scope
``` 
- Runtime Error:
```txt
Runtime Error:
        Division by zero
```
---

## Sample Program
There are four example programs `example1.cvm`, `example2.cvm`, `example3.cvm` and `example4.cvm` in repository, one can refer to these examples for better understanding of the language.
Here, we are considering `example2.cvm`, which is one of the simplest for explanation.
```
//Example 2
//program to check if a number is in a given range

int low;
int high;
int x;
bool in_range;

low=input();
high=input();
x=input();

if(x>=low && x<=high)
{
      in_range = true;
}
else
{
      in_range=false;
}

//printing whether the number is between low and high
print(in_range);
```

Input: `1 10 5` 
Output: `1`

Input: `1 10 15`
Output: `0`

NOTE: All inputs are space-separated, and print() always outputs the result followed by a new line.