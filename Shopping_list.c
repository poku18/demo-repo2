#include <stdio.h>
#include <stdlib.h>

#define MAX 50        //maximum command length
#define threshold 100 //maximum array sizes
#define buffer_size 100
#define add "add"
#define remove "remove"
#define exit "exit"
#define print_statement "print"
//Structure definition
struct items
{
    char *category; //string
    char **item;    //array of strings
    int *qty;       //indexing parrallel list
    int len_item;
};
//function prototypes;
char **command_splitter(char *);
int strCompare(char *, char *);
int binarySearch(char **, int, int, char *);
int binarySearch_category(struct items * , int , int , char *);
int insert_category(struct items *, char **, int);
void insert_items(char **, char *, int *, char *, int);
void insert_qty(int *, char *, int);
int qty_update(struct items, char *, char *, int);
void delete_item(struct items, int);
int memory_refill(char **item, int len_item);
void insert(struct items *, char **, int *);
void print(struct items *, int);
void print_category(struct items *, int, char *);
int validate_command(char **);
int isnum(char *);

int main()
{
    char commands[MAX];
    int i = 0;
    int len_list = 0;                                              // this will track the length
    struct items *list = calloc(threshold,sizeof(struct items)); //this is the main array of structs
    char buffer[buffer_size];
    while (1)
    {
        //Input of commands
        //scanf(" %[^\n]%*c", commands);
        fgets(commands, MAX, stdin);
        //commands[strcspn(commands,"\r\n")]=0;
        if(commands[0]=='\n'||strCompare(commands,"")||commands[0]==' '){
            continue;
        }
        char **temp = command_splitter(commands);
        if (!validate_command(temp))
            continue;

        //exit criteria
        if (strCompare(temp[0], "exit"))
        {
            break;
        }

        if (strCompare(temp[0], "print") && !strCompare(temp[1], ""))
        {
            print_category(list, len_list, temp[1]);
            continue;
        }
        if (strCompare(temp[0], "print"))
        {
            print(list, len_list);
            continue;
        }

        insert(list, temp, &len_list);
        free(temp);
    }
    free(list);
    return 0;
}
char **command_splitter(char *command)
{   int i;

    char **processed = calloc(20, sizeof(char *));
    for (i = 0; i < 20; i++)
    {
        processed[i] = calloc(20, sizeof(char));
    }
    int letter = 0, word = 0;
    i=0;
    while (command[i] != '\n')
    {
        if (command[i] == ' ')
        {
            letter = 0; //letter index back to 0 for next word
            word++;     //next word
        }
        else
        {
            processed[word][letter] = command[i];
            letter++;
        }
        i++;
    }
    processed[word + 1][0] = '\0'; //adding a sentinel value to mark end.
    return processed;
}

// Category and item insert function
void insert(struct items *list, char **split_command, int *len)
{   
    if (*len == 0)
    {
        if (strCompare("remove", split_command[0]))
        { //first command on a category should not be "remove"
            char buffer[buffer_size];
            sprintf(buffer,"Nothing to remove. Try adding some items first.\n");
            printf("%s",buffer);
            return;
        }
        list[0].category = split_command[3];
        list[0].item = calloc(threshold, sizeof(char **));
        list[0].qty = calloc(threshold, sizeof(int *));

        list[0].item[0] = split_command[1];
        if (strCompare("add", split_command[0]))
            list[0].qty[0] = atoi(split_command[2]);
        (*len)++;
        list[0].len_item = 1;
        return;
    }
    int i = 0, j = 0, pos, category_match = 0;
    if (*len > 0)
    {   
        category_match = binarySearch_category(list, 0, (*len) - 1, split_command[3]);
        i= category_match;
        if (category_match!=-1)
        {
            pos = binarySearch(list[i].item, 0, list[i].len_item - 1, split_command[1]);
            //inserting new items
            if (pos == -1)
            {   char buffer[buffer_size];
                if (strCompare(split_command[0], "remove"))
                {   
                    sprintf(buffer,"The item does not exist in this category\n");
                    printf("%s",buffer);
                    return;
                }
                if (list[i].len_item >= threshold)
                {
                    int status = memory_refill(list[i].item, list[i].len_item);
                    if (!status)
                    {   
                        sprintf(buffer,"Error reallocating. Exiting now.");
                        printf("%s",buffer);
                        return;
                    }
                }
                list[i].len_item++;
                insert_items(list[i].item, split_command[1],
                             list[i].qty, split_command[2],
                             list[i].len_item);
            }
            else
            {
                list[i].len_item = qty_update(list[i], split_command[0], split_command[2], pos);
            }
        }
        else
        {
            //New Category
            if (strCompare("remove", split_command[0]))
            { //first command on a category should not be "remove"
                char buffer[buffer_size];
                sprintf(buffer, "This category does not exist.\n");
                printf("%s",buffer);
                return;
            }
            (*len)++;
            int i = (*len);
            i = insert_category(list, split_command, i);
            //list[i].category=split_command[3];
            //sprintf("Value??: %d", i);
            list[i].item = calloc(threshold, sizeof(char **));
            list[i].qty = calloc(threshold, sizeof(int *));
            list[i].item[0] = split_command[1];
            if (strCompare("add", split_command[0]))
                list[i].qty[0] = atoi(split_command[2]);

            list[i].len_item = 1;
            return;
        }
    }
    return;
}
//Binary Search for the position of existing item
int binarySearch(char **item, int low, int high, char *name)
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
//Binary Search for the position of existing Category
int binarySearch_category(struct items* list, int low, int high, char *name)
{
    if (high < low)
        return -1;
    int mid = (low + high) / 2; /*low + (high - low)/2;*/
    if (strCompare(name, list[mid].category))
        return mid;
    if (name[0] > list[mid].category[0])
        return binarySearch_category(list, (mid + 1), high, name);
    return binarySearch_category(list, low, (mid - 1), name);
}
//Inserting new category
int insert_category(struct items *list, char **split_command, int len_list)
{
    int i;
    char *new_category = split_command[3];
    if (len_list > 2)
    {
        for (i = len_list - 2; (i >= 0 && list[i].category[0] > new_category[0]); i--)
        {
            list[i + 1] = list[i];
        }
        list[i + 1].category = new_category;
        return i + 1;
    }
    if (list[0].category[0] > new_category[0])
    {
        list[1] = list[0];
        list[0].category = new_category;
        return 0;
    }
    else
    {
        list[1].category = new_category;
        return 1;
    }
}
//Insert while sorting function in Items and qty
void insert_items(char **item, char *new_item, int *qty, char *new_qty, int len_item)
{
    int i;
    int new = atoi(new_qty);
    if (len_item > 2)
    {
        for (i = len_item - 2; (i >= 0 && item[i][0] > new_item[0]); i--)
        {
            item[i + 1] = item[i];
            qty[i + 1] = qty[i];
        }
        item[i + 1] = new_item;
        qty[i + 1] = new;
        return;
    }
    if (item[0][0] > new_item[0])
    {
        item[1] = item[0];
        item[0] = new_item;
        //doing the same for qty
        qty[1] = qty[0];
        qty[0] = new;
    }
    else
    {
        item[1] = new_item;
        qty[1] = new;
    }
    return;
}
//Quantity update if Item exists
int qty_update(struct items list, char *command, char *qty, int pos)
{
    int qty_update = atoi(qty);
    if (strCompare(command, "add"))
    {
        list.qty[pos] += qty_update;
    }
    else if (strCompare(command, "remove"))
    {
        list.qty[pos] -= qty_update;
        if (list.qty[pos] <= 0)
        {
            delete_item(list, pos);
            list.len_item--;
        }
    }
    return list.len_item;
}
//Delete item function
void delete_item(struct items list, int pos)
{
    int i;
    for (i = pos; i < list.len_item; i++)
    {
        list.item[i] = list.item[i + 1];
        list.qty[i] = list.qty[i + 1]; //similarly for parallel list
    }
    return;
}
// Print function
void print(struct items *list, int len_list)
{   char buffer[buffer_size];
    int i, j = 0;
    for (i = 0; (i < len_list && list[i].len_item > 0); i++)
    {
        sprintf(buffer,"\nCategory: %s\n", list[i].category);
        printf("%s",buffer);
        for (j = 0; j < list[i].len_item; j++)
        {   
            sprintf(buffer,"%s ", list[i].item[j]);
            printf("%s",buffer);
            sprintf(buffer,"%d\n", list[i].qty[j]);
            printf("%s", buffer);
        }
        
    }
    printf("\n");
}
//Print category
void print_category(struct items *list, int len_list, char *category)
{
    int i = 0, j = 0, match = 0;
    while (i < len_list && list[i].len_item > 0)
    {
        if (strCompare(category, list[i].category))
        {
            match = 1;
            break;
        }
        i++;
    }
    char buffer[buffer_size];
    if (match)
    {   
        sprintf(buffer,"\nCategory: %s\n", list[i].category);
        printf("%s", buffer);
        for (j = 0; j < list[i].len_item; j++)
        {
            sprintf(buffer,"%s %d\n", list[i].item[j],list[i].qty[j]);
            printf("%s",buffer);
        }
        printf("\n");
        return;
    }
    printf("No such category exists.\n");
    return;
}

//Memory refill reallocation
int memory_refill(char **item, int len_item)
{
    char **more_items = realloc(item, sizeof(char **) * (len_item + 5));
    if (more_items == NULL)
        return 0;

    item = more_items;
    return 1;
}
//String Compare function
int strCompare(char mj1[], char mj2[])
{
    int i = 0, flag = 0;
    while (mj1[i] != '\0' && mj2[i] != '\0') //until atleast one string ends
    {
        if (mj1[i] != mj2[i]) //Don't iterate if even one is unmatched
        {
            flag = 1;
            break;
        }
        i++;
    }
    if (flag == 0 && mj1[i] == '\0' && mj2[i] == '\0')
        return 1;
    else
        return 0;
}

int validate_command(char **input)
{
    //validating input and giving appropriate error messages
    if (strCompare(input[0], print_statement) || strCompare(input[0], exit))
    {
        return 1;
    }
    else if (strCompare(input[0], add) || strCompare(input[0], remove))
    {
        if (strCompare(input[1], "") && strCompare(input[1], "\0"))
        {
            printf("No item specified. Please enter an item name.\n");
            return 0;
        }
        else
        {
            if (strCompare(input[2], ""))
            {
                printf("No quantity specified. Please enter a quantity to proceed.\n");
                return 0;
            }
            else if (!isnum(input[2]))
            {
                printf("Invalid quantity. Please enter a numeric value.\n");
                return 0;
            }
            else
            {
                if (strCompare(input[3], "") && strCompare(input[3], "\0"))
                {
                    printf("No Category specified. Please enter a category.\n");
                    return 0;
                }
            }
        }
    }
    else if (strCompare(input[0], ""))
    {
        return -1;
    }
    else
    {
        printf("Invalid command. Please try add, remove, print or exit.\n");
        return 0;
    }
    return 1;
}

int isnum(char *input)
{
    int i = 0, isDigit = 0, count = 0;
    while (input[i] != '\0')
    {
        if (input[i] >= 48 && input[i] <= 57)
        {
            i++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}