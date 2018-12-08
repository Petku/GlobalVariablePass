# IP1-Projektová praxe #
DiffKEMP
## Zadanie ##
Napísať llvm-kový pass ktorý bude čítať globálne premenné jednoduchých typov 
a vypisovať ich defaultné hodnoty.

## Building a trivial LLVM pass ##
>This text is copied from https://github.com/abenkhadra/llvm-pass-tutorial

To build the skeleton LLVM pass found in `globvars` folder:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
`cmake` needs to find its LLVM configurations in `[LLVM_DIR]`. We automatically
setup `[LLVM_DIR]` based on `$LLVM_HOME` for you. Now the easiest way to run the skeleton pass is to use Clang:
```bash
$ clang -Xclang -load -Xclang build/skeleton/libLLVMGlobVars.* testingsource.c
```
Note that Clang is the compiler front-end of the LLVM project.

