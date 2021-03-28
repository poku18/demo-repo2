#include <stdio.h>
#include <stdlib.h>
#define MAX 50
#define threshold 5
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
int insert_category(struct items* ,char** , int );
void insert_items(char **, char* , int*, char*, int);
void insert_qty(int*, char* , int );
void qty_update(struct items , char* , char* , int);
int memory_refill(char **item,int len_item);
void insert(struct items *, char** , int *);
void print(struct items *,int);

int main(){
    char commands[MAX];
    int i=0;
    int len_list=0; // this will track the length 
    struct items *list=malloc(sizeof(struct items)*threshold); //this is the main array of structs
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
        list[0].item=calloc(threshold,sizeof(char **));
        list[0].qty=calloc(threshold,sizeof(int *));
        
        list[0].item[0]=split_command[1];
        if(strCompare("add",split_command[0]))
            list[0].qty[0]=atoi(split_command[2]);
        (*len)++;
        list[0].len_item=1;
        return;
    }
    int i=0,j=0,pos,category_match=0;
    if(*len>0){
        for(i=0;i<(*len);i++){
            if(strCompare(split_command[3],list[i].category)){
                category_match=1;
                break;
            }
        }
        if(category_match){            
            //Let's reallocate memory if length exceeded previous initialization
                pos=binarySearch(list[i].item, 0,list[i].len_item-1, split_command[1]);
                //inserting new items
                if(pos==-1){ //NEW ITEM_NAME INSERTION
                    if(list[i].len_item>=threshold){
                        int status=memory_refill(list[i].item,list[i].len_item);
                        if(!status){
                            printf("Error reallocating. Exiting now.");
                        return;
                        }
                    }
                    insert_items(list[i].item,split_command[1],
                    list[i].qty, split_command[2],
                    list[i].len_item);
                    list[i].len_item++;
                }
                else{
                    qty_update(list[i], split_command[0], split_command[2],pos);
                }
        }
        else{
            //New Category
            
            if(strCompare("remove",split_command[0]))
            {   //first command on a category should not be "remove"
                printf("You cannot remove what never was");
                return;
            }
            (*len)++;
            int i= (*len);
            i= insert_category(list,split_command, i);
            //list[i].category=split_command[3];
            printf("Value??: %d", i);
            list[i].item=calloc(threshold,sizeof(char **));
            list[i].qty=calloc(threshold,sizeof(int *));
            list[i].item[0]=split_command[1];
            if(strCompare("add",split_command[0]))
                list[i].qty[0]=atoi(split_command[2]);
            
            list[i].len_item=1;
            return;

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
//Inserting new category
int insert_category(struct items *list,char** split_command, int len_list){
    int i;
    char* new_category=split_command[3];
    if(len_list>2){
        for (i = len_list - 2; (i >= 0 && list[i].category[0] > new_category[0]); i--){
            list[i+1] = list[i];
        }
        list[i+1].category = new_category;
        return i+1;
    }
    if(list[0].category[0]>new_category[0]){
        list[1]=list[0];
        list[0].category=new_category;
        return 0;
        
    }
    else{
        list[1].category=new_category;
        return 1;
    }
}
//Insert while sorting function in Items and qty
void insert_items(char** item, char* new_item, int* qty, char* new_qty, int len_item){
    int i;
    int new= atoi(new_qty);
    if(len_item>2){
        for (i = len_item - 2; (i >= 0 && item[i][0] > new_item[0]); i--){
            item[i + 1] = item[i];
            qty[i+1]=qty[i];
        }
        item[i+1] = new_item;
        qty[i+1]=new;
        return;
    }
    if(item[0][0]>new_item[0]){
        item[1]=item[0];
        item[0]=new_item;
        //doing the same for qty
        qty[1]=qty[0];
        qty[0]=new;
        
    }
    else{
        item[1]=new_item;
        qty[1]=new;
    }
    return;
}
//Qty update if Item exists
void qty_update(struct items list, char* command, char* qty, int pos){
    int qty_update=atoi(qty);
    if(strCompare(command, "add")){
        list.qty[pos]+=qty_update;
    }
    else if(strCompare(command, "remove")){
        list.qty[pos]-=qty_update;
        if(list.qty[pos]==0){
            //delete_item(list.item, pos);
        }
    }
}

// Print function
void print(struct items *list, int len_list){
    int i,j=0;
    for(i=0;i<len_list;i++){
        printf("Category: %s\n",list[i].category);
        for(j=0;j<list[i].len_item;j++){
            printf("%s: ",list[i].item[j]);
            printf("%d\n",list[i].qty[j]);
        }
        printf("\n");
    }
}
//Memory refill reallocation
int memory_refill(char **item,int len_item){
    char **more_items= realloc(item, sizeof(char **)*(len_item+5)); 
        if(more_items==NULL) return 0; 
                 
    item= more_items; 
    return 1;
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


