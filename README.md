# maths_static_compiler

An experiment on how we could optimize mathematical expressions during compilation.

Builds a syntax tree when parsing an expression and optimizes it (Copy propagation, Algebraic Simplification, Dead Code Elimination)

> To be honest, in reality just use **[constant propagation](https://en.wikipedia.org/wiki/Constant_folding)**.

Example of execution (artifact [example.json](example.json) available):
```
➜ ./build/maths_static_compiler -o example.json -i "(10+20) / (20+10) + 150*32*(150*2-300) + (3 * x)"
%3 = 10
%4 = 20
%8 = 150
%9 = 32
%11 = 2
%13 = 300
%17 = 3
Give a value to the variable "x" = 10
%18 = 10
%5 = %3 + %4 = 30
%7 = %5 / %5 = 1
%10 = %8 * %9 = 4800
%12 = %8 * %11 = 300
%14 = %12 - %13 = 0
%15 = %10 * %14 = 0
%16 = %7 + %15 = 1
%19 = %17 * %18 = 30
%20 = %16 + %19 = 31
Result: 31
```

# Building and installing

See the [BUILDING](BUILDING.md) document.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

Apache License 2.0
See the [LICENSE](LICENSE) document.


