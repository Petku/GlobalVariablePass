#!/usr/bin/env bash

clang -O3 -emit-llvm testingsource.c -c -o build/testingsource.bc


echo "Test list all variables"
opt -load build/globvars/LLVMGlobVars.so -globvar < build/testingsource.bc > /dev/null
echo "=================================================="

echo "Test -var option with all variables"
opt -load build/globvars/LLVMGlobVars.so -globvarlist < build/testingsource.bc > /dev/null 2>tmp.txt
GLOBAL_VARIABLES=`cat tmp.txt`
rm tmp.txt
echo -e "$GLOBAL_VARIABLES\n\n"

for var_name in $GLOBAL_VARIABLES
do
    echo "=================================================="
    echo "$var_name "
    opt -load build/globvars/LLVMGlobVars.so -var $var_name -globvar < build/testingsource.bc > /dev/null
    echo "=================================================="
done
