//
// Created by nikolaspatrik on 27.12.2018.
//


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


int testInt1 = 142134;
int testInt2;

float testFloat1 = 2.2342;
float testFloat2 = .1;
float testFloat3;

char testChar1 = 'b';
char testChar2 = '\n';
char testChar3;
char testChar4 = '\\';

char *testString1 = "Ahoj";
char *testString2 = "Svete";
char *testString3 = "";
char *testString4;

typedef struct {
    int test;
    char test1;
    float test2;
    char *test3;
} TTestStruct;

TTestStruct testStruct1 = { 1, 'b', .34, NULL};
TTestStruct testStruct2;

int main()
{
    printf("Hello Global World\n");
    return 0;
}



