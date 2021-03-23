#include <stdio.h>
#include <stdlib.h>
#define MAX 50

struct items{
    char category[10];
    char item[20];
    int qty;
};
int main(){

char commands[MAX];
scanf("%[^\n]%*c", commands);

printf("%s",commands);
return 0;
}