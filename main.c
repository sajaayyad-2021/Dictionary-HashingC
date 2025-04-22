#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_MAX 50
#define A_IN_ASCII 97
#define MAX_IN_ROW 800
#define LOAD_FACTOR 0.65
#define MAX_IN_FILE 1000
#define WORD_MEANING_MAX 200
#define MAX_DICTIONARY_WORDS 1000


typedef struct dictionary_node{
    char word[WORD_MAX];
    char meaning[WORD_MEANING_MAX];
}dictionary;

typedef struct _buffer{
    int num_element;
    dictionary arr[MAX_DICTIONARY_WORDS];
}buffer;

enum probing {Linear = 1, Quadratic = 2};
enum Status {Empty = 0, Occupied = 1, Deleted = 2};

int isPrime(int);
int choose_method();
int table_size (int);
void displayChoices();
void displayMenu(buffer *b);
int new_size_rehash(int currentSize);
int hashFunction (char word[], int sizeOfTable);
void insertToBuffer(dictionary node, buffer *b);
int process_file(char *, dictionary[], buffer *b);
int checkLoadFactor (int counter, int currentSize);
float computeLoadFactor (int *counter , int tableSize);
int Occupied_slots (int currentSize, dictionary *hashTable, int *currentStatus);
int print_dictionary (int currentSize, dictionary *hashTable, int *currentStatus);
void update_meaning (int currentSize, dictionary *hashTable, int *currentStatus,int type);
void print_in_file(FILE *save_data,dictionary *hashTable, int *currentStatus, int currentSize);
dictionary *deleteWord (int currentSize, dictionary *hashTable, int *currentStatus, int *hashIndex, int type);
dictionary *searchWord (int currentSize, dictionary *hashTable, int *currentStatus, int *hashIndex, int type);
int rehash (buffer *b, dictionary *hashTable, int *currenStatus, int currentSize,int *counter, int type, char *word, char *meaning);
int insert (int *counter, int currentSize, dictionary arr_dictionary[], dictionary *hashTable, int *currentStatus, int type, buffer *b);
int hashCode(int counter, dictionary arr_dictionary[], int currentSize, dictionary *hashTable, int *currentStatus, int type);

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}
char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}
char *trim(char *s)
{
    return rtrim(ltrim(s));
}

void main(){
    int hashIndex;
    int size_table;
    int counter = 0; // Counter = Number of dictionary words////
    char *file_name = "dictionary.txt";///
    dictionary array_dictionary[MAX_IN_FILE];///
    int currentSize;


    buffer *buf = (buffer*)malloc(sizeof(buffer));
    buf->num_element = 0;
    displayMenu(buf);

//        free(currentStatus);
      //  free(hashTable);

    return 0;
}


void displayChoices(){
    printf("Choose from the menu below:\n");
    printf("1) Read dictionary file .\n");
    printf("2) Search for a word and display its meaning.\n");
    printf("3) Update the meaning of an existing word.\n");
    printf("4) Insert a new word from the user with its meaning. \n");
    printf("5) Print all words in the dictionary with their associated meanings. \n");
    printf("6) Print the entire hash table including the empty slots.\n");
    printf("7) Delete a word from the dictionary. \n");
    printf("8) Print the number of occupied slots, hash table size and the load factor.\n");
    printf("9) Save all words back in file  dictionary_modified.txt.\n");
    printf("10) Exit.\n");
}

int process_file(char *file_name, dictionary arr_dictionary[], buffer *b){
    char *key, *value; // Token
    FILE *dictionary_file;
    char line[MAX_IN_ROW];
    int num_dictionary_words = 0;
    const char delimiter[2] = ":";
    dictionary_file = fopen(file_name,"r");//open the file with read type

    //to check if the file is empty
    if (dictionary_file == NULL){
        printf("File does not exist");
        return (-1);
    }

    while (fgets(line, MAX_IN_ROW, dictionary_file)) {
        line[strcspn(line, "\n")] = 0; // Remove \n from line
        //Now parse line by colon
        /* get the first token */
        key = trim(strtok(line, delimiter)); // For sure (key)
        value = trim(strtok(NULL, delimiter)); // For sure (value)

        //Now fill dictionary
        strcpy(arr_dictionary[num_dictionary_words].word, key);
        strcpy(arr_dictionary[num_dictionary_words].meaning, value);
        insertToBuffer(arr_dictionary[num_dictionary_words], b);
        num_dictionary_words++;
    }

    // Close file.
    fclose(dictionary_file);
    return num_dictionary_words;
}

int isPrime(int n) {
  int j, flag = 1;
  for (j = 2; j <= n / 2; ++j) {
    if (n % j == 0) {
      flag = 0;
      break;
    }
  }
  return flag;
}

int table_size (int counter){
    float size = (float)counter / LOAD_FACTOR;
    while (!isPrime(size))
        size++;
    return size + 20;
}

int new_size_rehash(int currentSize){
    int newSize = 2*currentSize;
    while (!isPrime(newSize))
        newSize++;
    return newSize ;

    }

int checkLoadFactor (int counter, int currentSize){
    if (((float)counter/(float)currentSize) > LOAD_FACTOR) return 0;
    return 1;
}

int choose_method(){
    printf("Which method of collision resolution do you prefer to use ?\n");
    printf("Enter 1 for linear probing, or enter 2 for quadratic probing : \n");
    int choice;
    scanf("%d", &choice);
    return choice;
}

int hashFunction (char word[], int sizeOfTable){
   int sum = word[0] + word[1] + word[2];
   return sum % sizeOfTable;
}

int hashCode(int counter, dictionary arr_dictionary[], int currentSize, dictionary *hashTable, int *currentStatus, int type){
    int i, j;
    int collision_counter = 0 ;

    for (j = 0 ; j < counter; j++){
        i = 0;
        int hashIndex = hashFunction(arr_dictionary[j].word, currentSize);

        if(currentStatus[hashIndex] != Occupied){
            strcpy(hashTable[hashIndex].word, arr_dictionary[j].word);
            strcpy(hashTable[hashIndex].meaning, arr_dictionary[j].meaning);
            currentStatus[hashIndex] = Occupied;
        }
        else{
            while (hashIndex < currentSize && currentStatus[hashIndex] == Occupied){
                i++;
                collision_counter++;
                if(type == Linear)
                    hashIndex += i;
                else
                    hashIndex += (i * i);
            }

            if(hashIndex  < currentSize){
                strcpy(hashTable[hashIndex].word, arr_dictionary[j].word);
                strcpy(hashTable[hashIndex].meaning, arr_dictionary[j].meaning);
                currentStatus[hashIndex] = Occupied;
            }
        }
    }
    return collision_counter;
}

void displayMenu(buffer *b){

    int counter = 0;  // Counter = Number of dictionary words
    int currentSize;
    float loadFactor;
    int *currentStatus;
    dictionary *hashTable;
    int collision, slots;
    int answer, hashIndex, flag = 0;
    char *file_name = "dictionary.txt";
    dictionary array_dictionary[MAX_IN_FILE];
    char* read_file_first = "Please read the file first to create the hash table !";
    int choice = choose_method();

    // Area for initialization
    memset(hashTable, 0, currentSize * sizeof(dictionary));
    for (int j = 0 ; j < currentSize ; j++)
        currentStatus[j]= Empty;

        displayChoices();
        while(1){
            scanf("%d", &answer);
            switch (answer)
            {
                case 1 :
                    flag = 1;
                    counter = process_file(file_name, array_dictionary, b);
                    currentSize = table_size(counter);
                    currentStatus = (int *)malloc(sizeof(int) * currentSize);
                    hashTable = (dictionary *)malloc(sizeof(dictionary) * currentSize);
                    memset(hashTable, 0, currentSize * sizeof(dictionary));
                    for (int j = 0 ; j < currentSize ; j++)
                        currentStatus[j]= Empty;
                    collision = hashCode(counter, array_dictionary, currentSize, hashTable, currentStatus, choice);
                    printf("collision = %d\n",collision);
                    break;
                case 2:
                    if (flag == 0){
                        printf(read_file_first);
                    }
                    else{
                        printf("Please enter the word that you want to search for : \n");
                        dictionary *result = searchWord(currentSize, hashTable, currentStatus, &hashIndex, choice);
                    }
                    break;
                case 3 :
                    if (flag == 0){
                        printf(read_file_first);
                     }
                     else{
                        printf("Enter the word that you want to change its meaning : ");
                        update_meaning(currentSize, hashTable, currentStatus, choice);
                     }
                    break;
                case 4:
                     if (flag == 0){
                        printf(read_file_first);
                     }
                     else{
                        currentSize = insert(&counter, currentSize, array_dictionary, hashTable, currentStatus, choice ,b);
                     }

                    break;
                case 5 :
                     if (flag == 0){
                        printf(read_file_first);
                     }
                     else{
                        print_dictionary (currentSize, hashTable, currentStatus);
                     }
                    break;
                case 6:
                     if (flag == 0){
                        printf(read_file_first);
                     }
                      else{
                        for (int i = 0 ; i < currentSize; i++){
                        if (strcmp(hashTable[i].word, "") == 0){
                            printf("[%d] %s ----- %s \n", i, NULL, NULL);
                            continue;
                        }
                        printf("[%d] %s ----- %s -----%d \n", i, hashTable[i].word, hashTable[i].meaning,currentStatus[i]);
                        }
                     }
                    break;
                case 7 :
                    if (flag == 0){
                        printf(read_file_first);
                     }
                     else{
                         deleteWord(currentSize, hashTable, currentStatus, &hashIndex, choice);
                     }
                    break;
                case 8:
                     if (flag == 0){
                        printf(read_file_first);
                     }
                     else{
                        slots = Occupied_slots (currentSize, hashTable, currentStatus);
                        printf("the occupied slots are : %d \n", slots);
                        printf("The size of the table is : %d \n", currentSize);
                        loadFactor = computeLoadFactor (&counter , currentSize);
                        printf("the load factor is : %.2f \n", loadFactor);
                     }
                    break;
                case 9:
                    if (flag == 0){
                        printf(read_file_first);
                     }
                     else{
                        FILE *save_data;
                        save_data = fopen("dictionary_modified.txt","w");
                        print_in_file(save_data, hashTable, currentStatus, currentSize);
                        fclose(save_data);
                     }
                    break;
                case 10:
                    printf("GoodBye :) ");
                    return ;
                default:
                    printf("Please enter a number from 1 to 10 \n");
            }
            printf("\n\n");
             displayChoices();
        }


    }

dictionary *searchWord (int currentSize, dictionary *hashTable,int *currentStatus, int *hashIndex,int type){
    char *word_to_search = (char *)malloc(sizeof(char) * 30);
    scanf("%s", word_to_search);
    int i = 0 ;
    *hashIndex = hashFunction(word_to_search, currentSize);

    if (currentStatus[*hashIndex] != Occupied){
        if (currentStatus[*hashIndex] == Deleted)
            printf("The word that you are searching for has been already deleted. \n");
        else
            printf("That word (%s) does not exist int this dictionary.\n ", word_to_search);
    }
    else{
        while (currentStatus[*hashIndex] == Occupied && (*hashIndex ) < currentSize){
            if (strcmp(hashTable[*hashIndex].word, word_to_search) == 0){
                printf("The word that you are searching for is found!\n");
                printf("The meaning of this word (%s) is : %s\n", hashTable[*hashIndex].word, hashTable[*hashIndex].meaning);
                printf("The hashIndex is : %d \n", *hashIndex);
                free(word_to_search);
                return &hashTable[*hashIndex];
            }
            else{
                i++;
                if (type == Linear)
                    *hashIndex += i;
                else if (type == Quadratic)
                    *hashIndex += (i * i);
            }
        }
        printf("The word that you are searching for is not found !\n");
    }
    free(word_to_search);
    return NULL;
}

void update_meaning (int currentSize, dictionary *hashTable, int *currentStatus, int type){
    char *new_meaning = (char *)malloc(sizeof(char) * 300);
    int hashIndex;
    dictionary *result;
    if(type == Linear){
        result = searchWord(currentSize, hashTable, currentStatus, &hashIndex, Linear);
    }
    else if(type == Quadratic){
        result = searchWord(currentSize, hashTable, currentStatus, &hashIndex, Quadratic);
    }
    if (result){
        printf("Enter the new meaning : \n");
        scanf("%s", new_meaning);
        strcpy(result->meaning, new_meaning);
        printf("After updating the meaning of the word (%s) , the new meaning is : %s\n", result->word,result->meaning);
    }
    free(new_meaning);
}

int insert(int *counter,int currentSize, dictionary arr_dictionary[]/*didn't use it*/, dictionary *hashTable, int *currentStatus, int type, buffer *b){
    int i = 0 ;
    char *word = (char *)malloc(sizeof(char) * 30);
    char *meaning = (char *)malloc(sizeof(char) * 300);
    printf("Enter the new word that you want to insert into the dictionary : \n");
    scanf("%s", word);
    printf("Enter the meaning of this new word : \n");
    scanf("%s", meaning);

    int hashIndex = hashFunction(word, currentSize);
    if (checkLoadFactor(*counter, currentSize)){
        if (currentStatus[hashIndex] != Occupied){
            strcpy(hashTable[hashIndex].word, word);
            strcpy(hashTable[hashIndex].meaning, meaning);
            currentStatus[hashIndex] = Occupied;
            *counter = *counter + 1;
            insertToBuffer(hashTable[hashIndex], b);
        }
        else
        {
            while(currentStatus[hashIndex] == Occupied && (hashIndex + i) < currentSize){
                i++;
                if (type == Linear)
                    hashIndex += i;
                else if(type == Quadratic)
                    hashIndex += (i * i);
            }

            if (hashIndex < currentSize){
                strcpy(hashTable[hashIndex].word, word);
                strcpy(hashTable[hashIndex].meaning, meaning);
                currentStatus[hashIndex] = Occupied;
                *counter = *counter + 1;
                insertToBuffer(hashTable[hashIndex], b);
            }
            else{
                printf("-----%d-------", hashIndex);//test
                printf("rehashing will be done to the hash table.\n");
                *counter = *counter + 1;
                int newSize = rehash(b, hashTable, currentStatus,currentSize, counter, type, word, meaning);
                return newSize;
            }
        }
    }
    else{
        printf("The load factor of this hash table exceeded the desired value (0.65).\n");
        printf("rehashing will be done to the hash table.\n");
        *counter = *counter + 1;
        int newSize = rehash(b, hashTable, currentStatus, currentSize, counter, type, word, meaning);
        return newSize;
    }

    free(word);
    free(meaning);
    return currentSize;
}

int rehash(buffer *b, dictionary *hashTable, int *currentStatus, int currentSize, int *counter, int type, char *word, char *meaning){
    int i = 0, index = 0;
    int sizeTable = new_size_rehash(currentSize);

    dictionary item ;
    strcpy(item.word, word);
    strcpy(item.meaning, meaning);

    hashTable = realloc (hashTable, sizeof(dictionary) * sizeTable);
    currentStatus = realloc (currentStatus, sizeof(int) * sizeTable);

    // Resetting data in both arrays (hashTable, currentStatus)
    memset(hashTable, 0, sizeTable * sizeof(dictionary));
    for (i = 0; i < sizeTable; i++)
        currentStatus[i] = Empty;

    insertToBuffer(item , b);
    hashCode(*counter,b->arr, sizeTable, hashTable, currentStatus, type);

    for (int i = 0 ; i < sizeTable; i++){
            if (strcmp(hashTable[i].word, "") == 0)
                printf("[%d] %s ----- %s----%d\n", i, NULL, NULL, currentStatus[i]);
            else
                printf("[%d] %s ----- %s -----%d\n", i, hashTable[i].word, hashTable[i].meaning, currentStatus[i]);
    }// Steps:
    // create buffer and save the data from the previous hash table into this buffer
    // resize currentStatus with the new size
    // resize hashtable with the new size
    // reset all the previous data of the hashtable using memset with zero
    // hashcode for this new array

    return sizeTable;
}

int print_dictionary (int currentSize, dictionary *hashTable, int *currentStatus){

    int occupied_slots = 0;
    for(int i = 0 ; i < currentSize ; i ++){
        if (currentStatus[i] == Occupied){
            printf("(%d) %s ----- %s\n",i, hashTable[i].word, hashTable[i].meaning);
            occupied_slots = occupied_slots + 1;
        }
    }
    return occupied_slots;
}

dictionary *deleteWord (int currentSize, dictionary *hashTable,int *currentStatus,int *hashIndex,int type){

    printf("Please Enter the word that you want to delete : \n");
    dictionary *result = (dictionary *)malloc(sizeof(dictionary));
    result = searchWord(currentSize, hashTable, currentStatus, hashIndex, type);
    if (currentStatus[*hashIndex] == Occupied){
        currentStatus[*hashIndex] = Deleted;
        return &result;
    }
    else if(currentStatus[*hashIndex] == Deleted)
     printf("It has already been deleted. \n");

    return NULL;
}

void insertToBuffer(dictionary node, buffer *b){
    strcpy(b->arr[b->num_element].word, node.word);
    strcpy(b->arr[b->num_element].meaning, node.meaning);
    b->num_element = b->num_element + 1;
}

int Occupied_slots (int currentSize, dictionary *hashTable, int *currentStatus){

    int occupied_slots = 0;
    for(int i = 0 ; i < currentSize ; i ++){
        if (strcmp(hashTable[i].word, "") != 0 && currentStatus[i] != Deleted){
         //   printf("(%d) %s ----- %s\n",i, hashTable[i].word, hashTable[i].meaning);
            occupied_slots = occupied_slots + 1;
        }
    }
    return occupied_slots;
}

float computeLoadFactor (int *counter , int tableSize){
    return ((float)(*counter)/(float) tableSize);
}

void print_in_file(FILE *save_data,dictionary *hashTable, int *currentStatus, int currentSize){
    for(int i = 0 ; i < currentSize ; i ++){
        if (strcmp(hashTable[i].word, "") != 0 && currentStatus[i] != Deleted){
            fprintf(save_data,"(%d) %s: %s\n",i, hashTable[i].word, hashTable[i].meaning);
        }
    }
}
