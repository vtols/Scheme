[![Build Status](https://travis-ci.org/mtcomscxstart/Scheme.svg?branch=master)](https://travis-ci.org/mtcomscxstart/Scheme)
# Scheme

Scheme is interpreter implementation for Scheme language.

# Build
Build tested on this configurations:

OS | Compiler
--- | ---
GNU/Linux | GCC 4.7.2
FreeBSD 10.0 | Clang 3.3.0
OpenBSD 5.4 | GCC 4.2.1
Windows 8 | MSVC 18 (Visual Studio 2013)

Last version of Visual Studio is highly recommended, becuase it have C99 support.
Also now Windows version doesn't support command-line options.

To build project, firstly run commands in source directory:
```
$ mkdir build
$ cd build
$ cmake ..
```
Next, on Linux (or FreeBSD/OpenBSD) run:
```
$ make
```
On Windows open generated *.sln file, select 'sch' as startup project and build.

# Run
Example of expression evaluation:
```
$ ./sch -e '((lambda (x) (* x x)) 5)'
25
```
Example of interactive execution:
```
$ ./sch 
> (define add2 (lambda (x) (define y 2) (+ x y)))
> (add2 5)
7
```

#Notes
Interpreter may unexpectedly crash, because it does not have error handling mechanism yet.
