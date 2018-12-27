# IP1-Projektová praxe #
DiffKEMP
## Zadanie ##
Napísať llvm-kový pass ktorý bude čítať globálne premenné jednoduchých typov 
a vypisovať ich defaultné hodnoty.

## Riesenie funguje ako ##
Výsledná implementacia funguje ako vypisovanie hodnot:
```bash
Out[test.sh]:
    global_variable_name = type value
```
napr.:
```bash
testInt1 = i32 142134
```

Vyuziva vlastne toho ze trieda **Constant** dedi od nadtriedy **Value** pretypovany
operator _<<_ , ktory vypisuje aj typ aj hodnotu konstanty.
Ale kedze myslim ze toto nie je zrovna co potrebuje, potrebujem blizsie specifikovat
ako to mam presnejsie urobit.

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

## Testing ##
Na testovanie som vytvoril jednoduchy script ktory vypise vsetky globalne hodnoty 
a potom aj vyuziva parameter -var "nazov_globalnej premennej". Vyuzitie parametru
-var funguje iba pri pouziti prikazu `opt`. Napríklad takto:
```bash
$ clang -O3 -emit-llvm testingsource.c -c -o build/testingsource.bc
$ opt -load build/globvars/LLVMGlobVars.so -var "testInt1" -globvar < testingsource.bc > /dev/null
```
Blizsie informacie ako funguje testovanie su v subore **test.sh**:
```bash
$ ./test.sh
```