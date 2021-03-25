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

        /*while(temp[i][0]!='\0'){
        printf("%s\n",temp[i]);
        printf("%d",i);
        i++;
    }*/
        //exit criteria/command
        if(strCompare(temp[0],"exit")){
            break;
        }
        process(list,temp,&pos);
        //printing parts to remove
        printf("%s\n",list[pos].item);
        printf("%d\n",list[pos].qty);
        printf("%s\n",list[pos].category);
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
/*void process(struct items *list,char **split_command, int *pos){
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
}*/
// Category insert function


//String Compare function
int strCompare(char mj1[], char mj2[])  // function definition
{
    int i = 0, flag = 0;
    while(mj1[i] != '\0' && mj2[i] != '\0') // until atleast one string ends
    {
    //Don't iterate if even one is unmatched
        if(mj1[i] != mj2[i]) 
        {
            flag = 1;
            break;
        }
        i++;
    }
    /*
        If all the characters are sequentially same as well as 
        both strings have ended
    */
    if(flag == 0 && mj1[i] == '\0' && mj2[i] == '\0')
        return 1;
    else
        return 0;
}


