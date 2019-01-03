# IP1-Projektová praxe #
DiffKEMP
## Zadanie ##
Napísať llvm-kový pass ktorý bude čítať globálne premenné jednoduchých typov 
a vypisovať ich defaultné hodnoty.

## Riesenie funguje ako ##
In testsource.c is line as follows:
```c
int testInt1 = 141242;
```
Calling in bash:
```bash
opt -load pass.so -var testInt1 < testsource.bc > /dev/null
```
Output of command above:
```bash
141242
```


## Building a trivial LLVM pass ##
>This text is copied from https://github.com/abenkhadra/llvm-pass-tutorial

>**Toto treba urobit predtym nez spustite testy !!!**

To build the skeleton LLVM pass found in `globvars` folder:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
`cmake` needs to find its LLVM configurations in `[LLVM_DIR]`. We automatically
setup `[LLVM_DIR]` based on `$LLVM_HOME` for you. Now the easiest way to run the skeleton pass is to use Clang:

## Testing ##
Na testovanie som vytvoril jednoduchy script ktory vypise vsetky globalne hodnoty 
a potom aj vyuziva parameter -var "nazov_globalnej premennej". Vyuzitie parametru
-var funguje iba pri pouziti prikazu `opt`. Napríklad takto:
```bash
$ clang -O3 -emit-llvm testingsource.c -c -o build/testingsource.bc
$ opt -load build/globvars/LLVMGlobVars.so -var "testInt1" -globvar < testingsource.bc > /dev/null
```
Subor `test.sh` je treba volat s jednym z parametrov v nasledujucej tabulke:

| Prikaz       | Funkcionalita                                    |
|--------------|--------------------------------------------------|
| test.sh      | Vypise funckionalitu parametrov                  |
| test.sh -all | Vypise hodnotu vsetkych globalnych premennych   |
| test.sh -listids | Vypise identifikatory globalnych premennych |
| test.sh -allinfo | Vypise riadok kodu kde je definovana globalna premenna a jej hodnotu |
| test.sh -var <variable_name> | Vypise iba hodnotu premennej ktora je zadana ako *variable_name* |

Blizsie informacie ako funguje testovanie su v subore **test.sh**:
```bash
$ ./test.sh
```