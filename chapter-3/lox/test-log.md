# Chapter 3 Lox Test Log

Date: 2026-08-27

Interpreter used: `../craftinginterpreters/clox`

## Run Command

```bash
cd /Users/Jonny/cs4080/ConceptsOfProgramming
for file in \
  chapter-3/lox/truthiness.lox \
  chapter-3/lox/equality.lox \
  chapter-3/lox/short_circuit.lox \
  chapter-3/lox/scope.lox \
  chapter-3/lox/functions.lox \
  chapter-3/lox/closures.lox \
  chapter-3/lox/classes.lox
do
  ../craftinginterpreters/clox "$file"
done
```

## Results

### truthiness.lox

Observed that `true`, `0`, the empty string, and a nonempty string are truthy, while `false` and `nil` are falsey.

```text
true
truthy
false
falsey
nil
falsey
0
truthy
empty string
truthy
nonempty string
truthy
```

### equality.lox

Observed expected value equality rules, string equality and inequality, and reference-style equality for class instances.

```text
true
true
false
true
false
true
true
false
true
```

### short_circuit.lox

Observed short-circuit behavior for `and` and `or`, plus operand-return behavior. The function call text prints twice for the `true and ...` and `false or ...` cases because the function itself prints once and the outer `print` prints the returned string again.

```text
false
true
true and functionCall() should run
true and functionCall() should run
false or functionCall() should run
false or functionCall() should run
right
fallback
```

### scope.lox

Observed lexical block scope and shadowing: the inner block variable hides the global only inside the block.

```text
global
block
global
```

### functions.lox

Observed parameter passing, calculated return values, and implicit `nil` from a function with no explicit `return`.

```text
5
Hello, lox
nil
```

### closures.lox

Observed that each closure keeps its own captured `count` state.

```text
1
2
1
3
2
```

### classes.lox

Observed class construction, initializer behavior, instance fields, `this`, inheritance, `super`, and calling a saved bound method later.

```text
alpha
alpha
beta
loud
alpha
beta
loud
```
