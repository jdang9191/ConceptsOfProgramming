# Programming Language Challenges

This repository collects my solutions to the coding challenges from *Crafting Interpreters*.

## Repository Layout

```text
chapter-1/
  java/
    Hello.java
  c/
    linked_list.c
chapter-3/
  lox/
    truthiness.lox
    equality.lox
    short_circuit.lox
    scope.lox
    functions.lox
    closures.lox
    classes.lox
```

The Java and C files are my own challenge solutions. The Lox files are small test programs for the official `clox` interpreter, which lives in a separate copy of the book repository.

Official repository: https://github.com/munificent/craftinginterpreters

## Chapter 1

### Java

Compile and run the Java challenge:

```bash
cd chapter-1/java
javac Hello.java
java Hello
```

Expected output:

```text
Hello, world!
```

### C

Compile and run the linked-list challenge:

```bash
cd chapter-1/c
clang -std=c17 -Wall -Wextra -Wpedantic linked_list.c -o linked_list
./linked_list
```

## Chapter 3

The Lox programs in `chapter-3/lox/` are meant to be run with a separately built `clox` interpreter from the official `craftinginterpreters` repository.

Example from this repository, assuming the repositories are siblings:

```bash
../craftinginterpreters/clox chapter-3/lox/truthiness.lox
../craftinginterpreters/clox chapter-3/lox/equality.lox
../craftinginterpreters/clox chapter-3/lox/short_circuit.lox
../craftinginterpreters/clox chapter-3/lox/scope.lox
../craftinginterpreters/clox chapter-3/lox/functions.lox
../craftinginterpreters/clox chapter-3/lox/closures.lox
../craftinginterpreters/clox chapter-3/lox/classes.lox
```

## Lox Test Files

- `truthiness.lox` checks which values are truthy or falsey.
- `equality.lox` checks equality behavior for values, strings, and class instances.
- `short_circuit.lox` checks short-circuiting and operand return values for `and` and `or`.
- `scope.lox` checks block scope and variable shadowing.
- `functions.lox` checks parameters, return values, and implicit `nil` returns.
- `closures.lox` checks closures and captured state.
- `classes.lox` checks classes, `init()`, fields, `this`, inheritance, `super`, and bound methods.
