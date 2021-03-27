#include <stdio.h>
#include <stdlib.h>
#define MAX 50

//function prototypes;
char** command_splitter(char *);
int strCompare(char * , char *);
struct items{
    char *category; //string
    char **item; //array of strings
    int *qty; //indexing parrallel list
    int len_item;
};
int binarySearch(char ** , int , int , char * );
void insert_items(char **, char* , int);
void insert(struct items *, char** , int *);
void print(struct items *,int);

int main(){
    char commands[MAX];
    int i=0;
    int len_list=0; // this will track the length 
    struct items *list=malloc(sizeof(struct items)); //this is the main array of structs
    while(1){
        //Input of commands
        scanf("%[^\n]%*c", commands);
        char** temp = command_splitter(commands);

        //exit criteria/command
        if(strCompare(temp[0],"exit")){
            break;
        }
        insert(list,temp,&len_list);
        print(list,len_list);
        
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
        list[0].item=calloc(5,sizeof(char **));
        list[0].qty=calloc(5,sizeof(int *));
        
        list[0].item[0]=split_command[1];
        if(strCompare("add",split_command[0]))
            list[0].qty[0]=atoi(split_command[2]);
        (*len)++;
        list[0].len_item=1;
        return;
    }
    int i=0,j=0,pos,category_match=0;
    if(*len>0){
        printf("here\n");
        for(i=0;i<(*len);i++){
            if(strCompare(split_command[3],list[i].category)){
                printf("here2\n");
                category_match=1;
                break;
            }
        }
        printf("Category_match:%d\n",category_match);
        if(category_match){            
            printf("realloc??");
            //Let's reallocate memory for insertion process
            
            char **more_items= realloc(list[i].item, sizeof(char **)*(list[i].len_item+1)); 
            if(more_items==NULL){ 
                free(more_items); 
                more_items= realloc(list[i].item, sizeof(char **)*(list[i].len_item+1)); } 
                //list[i].item= more_items; //find if it exists

                printf("realloc??second");
                pos=binarySearch(list[i].item, 0,list[i].len_item-1, split_command[1]);
                list[i].len_item++;
                //inserting in 
                if(pos==-1){ //NEW ITEM_NAME INSERTION
                    insert_items(list[i].item,split_command[1],list[i].len_item);
                }
                
    //free(more_items);
        }   
        
    }
    
    return;
}
//Binary Search for the position of existing item
int binarySearch(char ** item, int low, int high, char * name)
{
    if (high < low)
        return -1;
    int mid = (low + high) / 2; /*low + (high - low)/2;*/
    if (strCompare(name, item[mid]))
        return mid;
    if (name[0] > item[mid][0])
        return binarySearch(item, (mid + 1), high, name);
    return binarySearch(item, low, (mid - 1), name);
}
//Insert while sorting function
void insert_items(char** item, char* new_item, int len_item){
    int i;
    printf("Insert_items-len_item: %d",len_item);
    if(len_item>2){
        for (i = len_item - 1; (i >= 0 && item[i][0] > new_item[0]); i--)
            item[i + 1] = item[i];
        printf("%d insert_ma?\n",len_item);
        item[i + 1] = new_item;
        return;
    }
    if(item[0][0]>new_item[0]){
        item[1]=item[0];
        item[0]=new_item;
    }
    else{
        item[1]=new_item;
    }
    return;
    
}
// Print function
void print(struct items *list, int len_list){
    int i,j=0;
    printf("Len_list: %d\n", len_list);
    for(i=0;i<len_list;i++){
        printf("Category: %s\n",list[i].category);
        printf("Len_item: %d\n", list[i].len_item);
        for(j=0;j<list[i].len_item;j++){
            printf("%s\n",list[i].item[j]);
            //printf("%d\n",list[i].qty[j]);
        }
        
    }
}
//String Compare function
int strCompare(char mj1[], char mj2[])     
{  
    int i = 0, flag = 0;
    while(mj1[i]!='\0' && mj2[i]!='\0') //until atleast one string ends
    {  
        if(mj1[i] != mj2[i])                //Don't iterate if even one is unmatched
        {
            flag = 1;
            break;
        }
        i++;
    }
    //printf("MJ1:%c i:%d\n",mj1[i],i);
    //printf("MJ2:%c i:%d\n",mj2[i],i);
    if(flag == 0 && mj1[i] == '\0' && mj2[i] == '\0')
        return 1;
    else
        return 0;
}


