#include <stdio.h>
#include <stdlib.h>
#define MAX 50

//function prototypes;
char** command_splitter(char *);
int strCompare(char * , char *);
struct items{
    char *category;
    char **item;
    int *qty;
};
void insert(struct items *, char** , int *);

int main(){
    char commands[MAX];
    int i=0;
    int len=0; // this will track the length 
    struct items *list=malloc(sizeof(struct items)); //this is the main array of structs
    while(1){
        //Input of commands
        scanf("%[^\n]%*c", commands);
        char** temp = command_splitter(commands);

        /*while(temp[i][0]!='\0'){
        printf("%s\n",temp[i]);
        printf("%d",i);
        i++;
    }*/
        //exit criteria/command
        if(strCompare(temp[0],"exit")){
            break;
        }
        insert(list,temp,&len);
        printf("%d",len);
        for(int i=0;i<len;i++){
            printf("%s\n",list[i].category);
            printf("%s\n",list[i].item[0]);
            printf("%d\n",list[i].qty[0]);
        }
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

// Category and item insert function
void insert(struct items *list, char** split_command, int *len){
    if(*len==0){
        if(strCompare("remove",split_command[0])){//first command on a category should not be "remove"
            printf("You cannot remove what never was");
            return;
        }
        list[0].category=split_command[3];
        list[0].item=calloc(1,sizeof(char **));
        list[0].qty=calloc(1,sizeof(int *));
        
        list[0].item[0]=split_command[1];
        if(strCompare("add",split_command[0]))
            list[0].qty[0]=atoi(split_command[2]);
    }
    /*int i,j=0;
    if(*len>1){
        for(i=0;i<*len;i++){
            if(split_command[3]==list[i].category){
                while(list[i].item[j]!=""){
                    //insert after sorting here.
                    j++;
                }
            }
        }
    }*/
    (*len)++;
    return;
}

//String Compare function
int strCompare(char mj1[], char mj2[])     
{
    int i = 0, flag = 0;
    while(mj1[i] != '\0' && mj2[i] != '\0') //until atleast one string ends
    {
        if(mj1[i] != mj2[i])                //Don't iterate if even one is unmatched
        {
            flag = 1;
            break;
        }
        i++;
    }
    if(flag == 0 && mj1[i] == '\0' && mj2[i] == '\0')
        return 1;
    else
        return 0;
}


