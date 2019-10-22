# ml (Mini-Lisp)
An interpreter for a Lisp-like language.

Prerequisite
------------

You will need `flex`, `bison`, `gcc`, and `make` to build this project.

Currently, the build script is only written for Unix-like environment.

Build
-----

* Run `make` to build the executable `ml` into `bin/`.

* Run `make clean` to clean up the generated files.

* You can add `-DDEBUG` flag to `CFLAGS` in the `Makefile` if you want the debug messages.

Usage
-----

`./bin/ml [-i] [<source>]`

* `-i` Run in **interactive mode**. Return value is echoed immediately after each expression.

* If no source file was provided, the interpreter will read from **stdin**.

* **Hint:** When in non-interactive mode, `EOF` can be simulated by pressing `Ctrl+D` (for *nix) or `Ctrl+Z` (for Windows).

The Language
------------

### Syntax

The syntax of **ml** is much like that of **Lisp**, it uses the [S-expression](https://en.wikipedia.org/wiki/S-expression).

### Operators

* Arithmetic: `+` `-` `*` `/` `mod`

* Comparation: `>` `<` `=`

* Boolean: `and` `or` `not`

* Others: `let` `define` `print` `if` `quote` `list` `lambda`

### Types

* Integer: `-192` `42`

* Boolean: `#t` `#f`

* Symbol: `hello-world`

* Cons: `(1 2 3 4 5)` `() == NIL`

### Comments

* Comments start with a `;` and last to the end of the line.

Example
-------

```lisp
(let
    (
        (fact
            (lambda (n)
                (if (> n 1)
                    (* n (fact (- n 1)))
                    1
                )
            )
        )
        (num 5)
    )
    (print (fact num))    ; print '120'
)
```

Check the files in `testcases/` for more examples.

Caveats
-------

The source is messy and may have some memory leaks and bugs, beware.
