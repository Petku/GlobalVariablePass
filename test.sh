#!/usr/bin/env bash

if [ -z $1 ]; then
    echo -e "Use some of paramaters below:"
    echo -e "\t-all\t\t\t Prints only values of tested global variable from testingsource.c"
    echo -e "\t-listids\t\t Prints identificators of all variables"
    echo -e "\t-allinfo\t\t Prints all values of globabl variables and the line of defintion from source code"
    echo -e "\t-var <variable_name>\t Print only value of _variable_name_"
    exit 1
fi

clang -O3 -emit-llvm testingsource.c -c -o build/testingsource.bc

if [ $1 = "-all" ]; then
    echo "=================================================="
    echo "Test list all values of variables:"
    opt -load build/globvars/LLVMGlobVars.so -globvar < build/testingsource.bc > /dev/null
    echo "=================================================="
fi


opt -load build/globvars/LLVMGlobVars.so -globvarlist < build/testingsource.bc > /dev/null 2>tmp.txt
GLOBAL_VARIABLES=`cat tmp.txt`
rm tmp.txt
if [ $1 = "-listids" ]; then
    for var_id in $GLOBAL_VARIABLES
    do
        echo $var_id
    done
fi

if [ $1 = "-allinfo" ]; then
    echo "Test -var option with all variables"
    for var_name in $GLOBAL_VARIABLES
    do
        echo "=================================================="
        cat testingsource.c | grep $var_name
        opt -load build/globvars/LLVMGlobVars.so -var $var_name -globvar < build/testingsource.bc > /dev/null
        echo "=================================================="
    done
fi

if [ $1 = "-var" ]; then
    opt -load build/globvars/LLVMGlobVars.so -var $2 -globvar < build/testingsource.bc > /dev/null
fi
