# Chapter 19 — Strings

## 1. Flexible array members

A flexible array member lets `ObjString` keep its character bytes directly after the fixed fields of the struct. Allocate enough space for `sizeof(ObjString)`, the string length, and the terminating null byte in one allocation. Copy the characters into that trailing array, and free the complete allocation together. This removes the separate character-buffer allocation and an extra pointer indirection.

## 2. Character-buffer ownership

The implementation uses an `ownsChars` flag to distinguish strings that own a heap character buffer from literal strings that borrow their characters from retained source text. The collector frees the character buffer only for owned strings, then frees the `ObjString` object. This ownership-based design is kept in the code; it is separate from the flexible-array approach described in section 1.
