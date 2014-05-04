[![Build Status](https://travis-ci.org/mtcomscxstart/Scheme.svg?branch=master)](https://travis-ci.org/mtcomscxstart/Scheme)
# Scheme

Scheme is interpreter implementation for Scheme language.

# Build
Build tested on this configurations:

OS | Compiler
--- | ---
GNU/Linux | GCC 4.7.2
FreeBSD 10.0 | Clang 3.3.0
Windows 8 | MSVC 18 (Visual Studio 2013)

Last version of Visual Studio is highly recommended, becuase it have C99 support.

To build project, firstly run commands in source directory:
```
$ mkdir build
$ cd build
$ cmake ..
```
Next, on Linux or FreeBSD run:
```
$ make
```
On Windows open generated *.sln file, select 'sch' as startup project and build.
