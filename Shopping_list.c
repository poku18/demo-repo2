#include <stdio.h>
#include <stdlib.h>
#define MAX 50

//function prototypes;
char** command_splitter(char *);

struct items{
    char category[20];
    char item[20];
    int qty;
    //struct items *next; //Linked List implementation
};

int main(){
    char commands[MAX];
    int i=0;
    scanf("%[^\n]%*c", commands);
    char** temp = command_splitter(commands);
    
    while(temp[i][0]!='\0'){
        printf("%s\n",temp[i]);
        i++;
    }
    return 0;
}
char** command_splitter(char *command){
    char** processed=malloc(20 * sizeof(char*));
    for (int i =0 ; i < 20; i++){
        processed[i] = malloc(20 * sizeof(char));
        }
    int i=0,letter=0,word=0;

    while(command[i]!='\0'){
        if(command[i]==' '){
            letter=0; //letter index back to 0 for next word
            word++; //next word
        }
        else{
            processed[word][letter]=command[i];
            letter++;
        }
        i++;
    }
    processed[word+1][0]='\0'; //adding a sentinel value to mark end.
    return processed;
}
//changes made here. 

