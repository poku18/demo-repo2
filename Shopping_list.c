#include <stdio.h>
#include <stdlib.h>
#define MAX 50

//function prototypes;
char** command_splitter(char *);

struct items{
    char *category;
    char *item;
    int qty;
};

void process(struct items *,char **, int *);

int main(){
    char commands[MAX];
    int i=0;
    int pos=0;
    struct items *list=malloc(sizeof(struct items)*20);
    while(1){
        //Input of commands
        scanf("%[^\n]%*c", commands);
        char** temp = command_splitter(commands);
        printf("%s\n",temp[0]);
        //exist criteria/command
        if(temp[0]=="exit"){
            printf("anytime?");
            break;
        }
        //process(list,temp,&pos);
        //printing parts to remove
        /*printf("%s\n",list[pos].item);
        printf("%d\n",list[pos].qty);
        printf("%s\n",list[pos].category);
        */

        free(temp);

    }
    free(list);
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

//Function to add items in array of struct
void process(struct items *list,char **split_command, int *pos){
    list[*pos].item=split_command[1];
    list[*pos].category=split_command[3];
    if(split_command[0]=="add"){
        list[*pos].qty+= (int)split_command[2];
    }
    else if(split_command[0]=="add"){
        list[*pos].qty-= (int)split_command[2];
    }
    *pos++;
    return;
}

