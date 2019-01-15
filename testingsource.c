#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


int testInt1 = 142134;
int testInt2 = 5;
int testInt3;

float testFloat1 = 2.2342;
float testFloat2 = .1;
float testFloat3;

char testChar1 = 'b';
char testChar2 = '\n';
char testChar3;
char testChar4 = '\\';

int testArray1[] = {1, 2, 3, 4, 5};
int testArray2[5] = {6, 7, 8};
double testArray3[2] = {.5, 2.75};

char testString1[] = "Ahoj";
char *testString2 = "Svete";
char *testString3 = "";
char *testString4;

typedef struct {
    int test;
    char test1;
    float test2;
    char *test3;
} TTestStruct;

TTestStruct testStruct1 = { 565465, 'b', .34, NULL};
TTestStruct testStruct2;
TTestStruct testStruct3 = {0,'a',};

int main()
{
    printf("Hello Global World\n");
    return 0;
}



