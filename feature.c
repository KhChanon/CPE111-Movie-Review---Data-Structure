/* 
* chainedHashTable.c
*
* Implementation of abstractHashTable.h that uses a chained
* hash table. The calling program supplies the hashing function
* as part of the initialization.
*
* Limitation: this implementation will not work well for duplicate
* keys. It will store both but the returned data is unpredictable.
*
* Limitation: this implementation assumes keys are strings 
* less than 128 chars long
*
*   Created by Sally Goldin on 5 March 2012 for CPE 113
*   Updated to explicity keep track of list head and tail on
*       26 March 2013
*
*
*  Modified with permission by [Chanon, Phetdanai, Fasai, Pokpong]  on 21 May 2021. 
*    - Added including header file (read.h) 
*		- Modified the entire c file to be able to use 2 Hash Table 
*		- Modify BitwiseOpHash function into userBitwiseOpHash and reviewBitwiseOpHash.
* 
*  Add the following function to the file with permission by [Chanon, Phetdanai, Fasai, Pokpong]
*  Last modify 21 May 2021
*  Added:
*		- searchUser
* 	  - searchMovie
* 	  - searchGenre
* 	  - searchDate
* 	  - newFeed
* 	  - displayFollowSuggest
* 	  - editMovie
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "feature.h"
#include "linkedListUtil.h"
#include "read.h"
#include "validation.h"

#define KEYLEN 128

/**************************
 ========= NOTE ==========
                         
    1 is the user table   
   2 is the review table  
**************************/

/* Structure for table elements */
typedef struct _hashItem
{
    char key[KEYLEN];              /* copy of the key */
    void* data;                    /* data */
    struct _hashItem * next;       /* next item in the bucket if any */
} HASH_ITEM_T; 

/* Structure for list with head and tail */
typedef struct _linkedList
{
    HASH_ITEM_T* head;             /* first item in list - null if list empty */
    HASH_ITEM_T* tail;             /* last item in the list */
} LINKED_LIST_T; 


/* Hash function - set by hashTableInit  */
unsigned int (*hashFn)(char* key) = NULL;

static LINKED_LIST_T * userTable = NULL;  /* we will allocate our table based on
* initialization arguments and store it
* here 
*/
	 
static LINKED_LIST_T * reviewTable = NULL;  /* we will allocate our table based on
* initialization arguments and store it
* here 
*/  
static int userTableSize = 0;     /* size of the table */
static int userItemCount = 0;     /* keep track of current number of stored items */

static int reviewTableSize = 0;     /* size of the table */
static int reviewItemCount = 0;     /* keep track of current number of stored items */

char* movieGenre[6] = {"Drama","Romance", "Action", "Comedy", "Horror", "Fantasy"};

/* 
Return the number of slots in the hash table.
*/
int userHashTableSize()
{
    return userTableSize;
}

/* 
Return the number of slots in the hash table.
*/
int reviewHashTableSize()
{
    return reviewTableSize;
}

/* 
Return the number of items currently stored in the hash table.
*/
int userHashTableItemCount()
{
    return userItemCount;
}

/* 
Return the number of items currently stored in the hash table.
*/
int reviewHashTableItemCount()
{
    return userItemCount;
}

/* Initialize the hash table.
* Arguments
*    size                - How many slots in the table
*                          Must be 1 or greater. We assume the caller
*                          has checked this.
*    hashFunction        - Function that takes a string and returns an int
*                          which will be the index into the table.
* Return 1 if successful, 0 if some error occurred.
*/
int hashTableInit(int table, int size, unsigned int (*hashFunction)(char* key))
{
    int bOk = 1;
    /* free the old table, if any */
    if(table == 1)
        {
        hashTableFree(1);
        }
    else 
        {
        hashTableFree(2);
        }
    hashFn = hashFunction;
    if(table == 1)
        {
        userTableSize = size;
        /* try to allocate the table, which will store pointers
        * to LINKED_LIST_T elements.
        */
        userTable = (LINKED_LIST_T*) calloc(size,sizeof(LINKED_LIST_T));
        if (userTable == NULL)
            {
            bOk = 0;
            }
        }
    else
        {
        reviewTableSize = size;
        /* try to allocate the table, which will store pointers
        * to LINKED_LIST_T elements.
        */
        reviewTable = (LINKED_LIST_T*) calloc(size,sizeof(LINKED_LIST_T));
        if (reviewTable == NULL)
            {
            bOk = 0;
            }
        }
    return bOk;
}


/* 
Free the hash table.
*/
void hashTableFree(int table)
{
    int i = 0;
    HASH_ITEM_T * pItem = NULL;
    HASH_ITEM_T * pNextItem = NULL;
    if(table == 1)
        {
        if (userTable != NULL) 
            {		
            for (i = 0; i < userTableSize; i++)
                {
                if (userTable[i].head != NULL)  /* something stored in this slot*/
                    {
                    pItem = userTable[i].head;
                    /* walk the linked list, freeing each item */
                    while (pItem != NULL)
                        {
                        pNextItem = pItem->next;
                        free(pItem);
                        pItem = pNextItem;
                        }
                    userTable[i].head = NULL;
                    userTable[i].tail = NULL;
                    }
                }
            free(userTable);
            userTable = NULL;
            userTableSize = 0;
            userItemCount = 0;
            }
        }
    else
        {
        if (reviewTable != NULL) 
            {
            for (i = 0; i < reviewTableSize; i++)
                {
                if (reviewTable[i].head != NULL)  /* something stored in this slot*/
                    {
                    pItem = reviewTable[i].head;
                    /* walk the linked list, freeing each item */
                    while (pItem != NULL)
                        {
                        pNextItem = pItem->next;
                        free(pItem);
                        pItem = pNextItem;
                        }
                    reviewTable[i].head = NULL;
                    reviewTable[i].tail = NULL;
                    }
                }
            free(reviewTable);
            reviewTable = NULL;
            reviewTableSize = 0;
            reviewItemCount = 0;
            }
        }
}

/* Insert a value into the hash table.
* Arguments 
*    key                 - character string key
*    data                - data to store in the table
*    pCollision          - set to true if there was a collision storing
*                          the data, else false 
* Returns true (1) unless hash table has not been initialized or
* we can't allocate memory, in which case returns false (0)
*/
int hashTableInsert(int table, char* key, void* data,int* pCollision)
{
    int bOk = 1;
    int hashval = 0;
    HASH_ITEM_T * pItem = NULL;
    HASH_ITEM_T * pTemp = NULL;

    if(table == 1)
        {
        if (userTable == NULL)  /* not initialized */
            {
            return 0;
            }

        pItem = (HASH_ITEM_T*) calloc(1,sizeof(HASH_ITEM_T));
        if (pItem == NULL)
            {
            bOk = 0;  /* can't allocate memory */
            }
        else
            {
            strncpy(pItem->key,key,KEYLEN-1);
            pItem->data = data;
            hashval = hashFn(key);
            if (userTable[hashval].head == NULL)
                {
                userTable[hashval].head = pItem;  /* bucket was empty */
                *pCollision = 0;              /* no collision */
                }
            else
                {
                *pCollision = 1;         /* We have a collision */
                /* put the new item at the end of the bucket list */
                userTable[hashval].tail->next = pItem;
                }
            userTable[hashval].tail = pItem;
            userItemCount++;
            }
        }
    else
        {
        if (reviewTable == NULL)  /* not initialized */
            {
            return 0;
            }

        pItem = (HASH_ITEM_T*) calloc(1,sizeof(HASH_ITEM_T));
        if (pItem == NULL)
            {
            bOk = 0;  /* can't allocate memory */
            }
        else
            {
            strncpy(pItem->key,key,KEYLEN-1);
            pItem->data = data;
            hashval = hashFn(key);
            if (reviewTable[hashval].head == NULL)
                {
                reviewTable[hashval].head = pItem;  /* bucket was empty */
                *pCollision = 0;              /* no collision */
                }
            else
                {
                *pCollision = 1;         /* We have a collision */
                /* put the new item at the end of the bucket list */
                reviewTable[hashval].tail->next = pItem;
                }
            reviewTable[hashval].tail = pItem;
            reviewItemCount++;
            }
        }
    return bOk;
}

/* Look up a value in the hash table.
* Arguments 
*    key                 - character string key
* Returns the data associated with the key, or NULL if 
* data associated with the key is not found.
*/
void* hashTableLookup(int table, char* key)
{
    /* This function is similar to remove but we do not
    * change anything in the hashtable structure 
    */
    void * foundData = NULL;
    HASH_ITEM_T* pPrev = NULL;
    HASH_ITEM_T* pTemp = NULL;
    if(table == 1)
        {
        if (userTable != NULL)    /* initialized */
            {
            int hashval = hashFn(key);
            if (userTable[hashval].head != NULL)   /* in the table */
                {
                pTemp = userTable[hashval].head;
                while (pTemp != NULL)
                    {
                    if (strncmp(pTemp->key,key,KEYLEN-1) == 0)  /* match */
                        {
                        foundData = pTemp->data;
                        pTemp = NULL;  /* this will make us exit loop */
                        }
                    else
                        {
                        pPrev = pTemp;
                        pTemp = pTemp->next;  /* check next item */	  
                        }
                    } /* end loop through items in the bucket */
                } /* end if the key is in the table */
            } /* end if the hash table is initialized */      
        }
    else
        {
        if (reviewTable != NULL)    /* initialized */
            {
            int hashval = hashFn(key);
            if (reviewTable[hashval].head != NULL)   /* in the table */
                {
                pTemp = reviewTable[hashval].head;
                while (pTemp != NULL)
                    {
                    if (strncmp(pTemp->key,key,KEYLEN-1) == 0)  /* match */
                        {
                        foundData = pTemp->data;
                        pTemp = NULL;  /* this will make us exit loop */
                        }
                    else
                        {
                        pPrev = pTemp;
                        pTemp = pTemp->next;  /* check next item */	  
                        }
                    } /* end loop through items in the bucket */
                } /* end if the key is in the table */
            } /* end if the hash table is initialized */ 
        }      
    return foundData;
}

/* Robust hash function that uses bitwise operations to
* modify string values. Adapted from Kyle Loudon,
* "Mastering Algorithms with C"
*/
unsigned int userBitwiseOpHash(char* key)
{
    unsigned int result = 0;
    unsigned int tmp = 0;
    int size = userHashTableSize();
    int i = 0;
    
    for (i = 0; i < strlen(key); i++)
        {
        /* shift up four bits then add in next char */ 
        result = (result << 4) + key[i];
        if (tmp == (result & 0xf0000000))  /* if high bit is set */
            {
            /* XOR result with down shifted tmp */
            result = result ^ (tmp >> 24);
            /* then XOR with tmp itself */
            result = result ^ tmp;
            }
        }
    result = result % size;   /* make it fit in the table size */ 
    return result;
}

/* Robust hash function that uses bitwise operations to
* modify string values. Adapted from Kyle Loudon,
* "Mastering Algorithms with C"
*/
unsigned int reviewBitwiseOpHash(char* key)
{
    unsigned int result = 0;
    unsigned int tmp = 0;
    int size = reviewHashTableSize();
    int i = 0;
    for (i = 0; i < strlen(key); i++)
    {
    /* shift up four bits then add in next char */ 
    result = (result << 4) + key[i];
    if (tmp == (result & 0xf0000000))  /* if high bit is set */
        {
        /* XOR result with down shifted tmp */
        result = result ^ (tmp >> 24);
        /* then XOR with tmp itself */
        result = result ^ tmp;
        }
    }
    result = result % size;   /* make it fit in the table size */ 
    return result;
}

/*****************************************************
This function will go through the User hash table
and write all the data along with the new 
information of user register new account.
******************************************************/
void writeUserHash()
{
    FILE * pOut = NULL;
    FILE * pLikeOut = NULL;
    FILE * pFollowingOut = NULL;
    HASH_ITEM_T * oneHash = NULL;
    USER_T * oneUser = NULL;
    int j = 1;

    pOut = fopen("userInformation.txt","w");
    pLikeOut = fopen("likeuser.txt","w");
    pFollowingOut = fopen("followinguser.txt","w");

    for(int i = 0; i < userTableSize; i++)
        {
        if (userTable[i].head != NULL)
            {
            oneHash = userTable[i].head;
            oneUser = oneHash->data;
            fprintf(pOut,"#%d\n",j);
            fprintf(pOut,"Username         : %s\n",oneUser->username);
            fprintf(pOut,"Password         : %s\n",oneUser->password);
            fprintf(pOut,"Email            : %s\n",oneUser->email);
            fprintf(pOut,"Genre            : %d %d %d\n",oneUser->favGenre[0], oneUser->favGenre[1], oneUser->favGenre[2]);
            fprintf(pOut,"Last Login       : %d/%d/%d\n\n",oneUser->lastLoginDay,oneUser->lastLoginMonth,oneUser->lastLoginYear);
            writeLikedUser(oneUser);
            writeFollowingUser(oneUser);
            oneHash = oneHash->next;

            while(oneHash != NULL)
                {
                j++;	
                oneUser = oneHash->data;
                fprintf(pOut,"#%d\n",j);
                fprintf(pOut,"Username         : %s\n",oneUser->username);
                fprintf(pOut,"Password         : %s\n",oneUser->password);
                fprintf(pOut,"Email            : %s\n",oneUser->email);
                fprintf(pOut,"Genre            : %d %d %d\n\n",oneUser->favGenre[0], oneUser->favGenre[1], oneUser->favGenre[2]);
                writeLikedUser(oneUser);
                writeFollowingUser(oneUser);
                oneHash = oneHash->next;
                }
            j++;
            }
        }
    fclose(pOut);
}

/********************************************
this function is for writing review from the
struct into a text file
*********************************************/
void writeReview()
{
    FILE * pOut = NULL;
    char * filename = "review.txt";
    HASH_ITEM_T * oneHash = NULL;
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;

    pOut = fopen(filename,"w");
    /*loop for the size of table size*/
    for(int i = 0; i < reviewTableSize; i++)
        {
        if (reviewTable[i].head != NULL)
            {
            oneHash = reviewTable[i].head;
            while(oneHash != NULL)
                {
                oneMovie = oneHash->data;
                oneReview = listGetNext(oneMovie->review);
                /*loop until struct is NULL and write into file*/
                while(oneReview != NULL)
                    {	
                    fprintf(pOut,"#%s\n",oneReview->moviename);
                    fprintf(pOut,"Username      : %s\n",oneReview->username);
                    fprintf(pOut,"Date          : %d/%d/%d\n",oneReview->day,oneReview->month,oneReview->year);
                    fprintf(pOut,"Genre         : %d %d %d %d %d %d\n",oneReview->genre[0],oneReview->genre[1], oneReview->genre[2],oneReview->genre[3], oneReview->genre[4], oneReview->genre[5]);
                    fprintf(pOut,"Like          : %d\n",oneReview->likeReview);
                    fprintf(pOut,"Rating(1-10)  : %.1lf\n",oneReview->rating);
                    fprintf(pOut,"Detail        : %s\n\n",oneReview->detail);
                    oneReview = listGetNext(oneMovie->review);
                    }
                oneHash = oneHash->next;
                listReset(oneMovie->review);
                }
            }
        }
     fclose(pOut);
}

/******************************************
This function is for displaying the review
*******************************************/
void printReview(REVIEW_T* oneReview)
{
    int genreNum = 0;

    printf("\n#%s\n",oneReview->moviename);
    printf("Username      : %s\n",oneReview->username);
    printf("Date          : %d/%d/%d\n",oneReview->day,oneReview->month,oneReview->year);
    /*loop to display all item in genre array*/
    for(int i = 0; i < 6 ;i++)
        {
        if(oneReview->genre[i] == oneReview->genre[i+1])
            {
            genreNum = i+1;
            break;
            }
        }
    if(genreNum == 0)
        {
        genreNum = 6;
        }
    printf("Genre         : %s ",movieGenre[oneReview->genre[0]]);	

    for(int i = 1; i < genreNum ;i++)
        {
        printf("%s ", movieGenre[oneReview->genre[i]]);	
        }
    printf("\n");	
    printf("Like          : %d\n",oneReview->likeReview);
    printf("Rating(1-10)  : %.1lf\n",oneReview->rating);
    printf("Detail        : %s\n",oneReview->detail);
}

/****************************************
This function is for handling the liking
feature
*****************************************/
void likeUser(USER_T* loginUser, REVIEW_T* oneReview)
{
    int foundlike = 0;
    USER_T* oneUser = NULL;

    listReset(loginUser->likedUser);
    oneUser = listGetNext(loginUser->likedUser);
    oneReview->likeReview++;
    while(oneUser != NULL)
        {
        if(strcmp(oneUser->username, oneReview->username) == 0 || strcmp(oneReview->username, loginUser->username) == 0)
            {
            foundlike++;
            break;
            }
        oneUser = listGetNext(loginUser->likedUser);
        }
        listReset(loginUser->likedUser);

    if(foundlike == 0)
        {
        loginUser->likeCount++;
        if(listAtEnd(loginUser->likedUser) == -1)
            {
            loginUser->likedUser = newList();
            }
        listInsertEnd(loginUser->likedUser, hashTableLookup(1, oneReview->username));
        }
}

/*****************************************************
Find reviews of movies by movie name, movie type, 
review date or user who wrote the review.
******************************************************/
int searchUser(char* finduser, char* loginUsername)
{
    HASH_ITEM_T * oneHash = NULL;
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;
    USER_T * oneUser = NULL;
    int findUserCount = 0;
    int genreNum = 0;
    char stringInput[128];
    int yn;
    int foundlike = 0;
    USER_T* loginUser = hashTableLookup(1, loginUsername);
    /*loop through whole hash table*/
    for(int i = 0; i < reviewTableSize; i++)
        {
        if (reviewTable[i].head != NULL)
            {
            oneHash = reviewTable[i].head;
            while(oneHash != NULL)
                {
                oneMovie = oneHash->data;
                oneReview = listGetNext(oneMovie->review);
                while(oneReview != NULL)
                    {	
                    if(strcmp(finduser,oneReview->username) == 0)
                        {
                        findUserCount++;
                        printReview(oneReview);
                        while(1)
                            {
                            printf("\n\nDo you want to\n");
                            printf("[1] - Like the review\n");
                            printf("[2] - See next review\n");
                            printf("[3] - Exit to main menu\n\n");
                            printf("Enter here : ");
                            fgets(stringInput,sizeof(stringInput),stdin);
                            sscanf(stringInput, "%d", &yn);
                            if(yn <= 3 && yn >= 1)
                                {
                                break;
                                }
                            else
                                {
                                printf("\tplease type 1, 2 or 3 only\n");
                                }
                            }
                        if(yn == 1)
                            {
                            likeUser(loginUser,oneReview);
                            }
                        }
                    oneReview = listGetNext(oneMovie->review);		
                    if(yn == 3)
                        {
                        break;
                        }					
                    }
                oneHash = oneHash->next;
                listReset(oneMovie->review);
                if (yn == 3)
                    {
                    break;
                    }
                }
            }
        if(yn == 3)
            {
            break;
            }
        }
    return findUserCount;
}

/***************************************************
This function is for searching up the review by
movie title
****************************************************/
void searchMovie(char* findmovie, char* loginUsername)
{
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;
    int genreNum = 0;
    char stringInput[128];
    int yn;
    int foundlike = 0;
    USER_T* loginUser = hashTableLookup(1, loginUsername);
    USER_T * oneUser = NULL;
    /*search for movie by putting findmovie lookup function*/
    oneMovie = hashTableLookup(2, findmovie);
    oneReview = listGetNext(oneMovie->review);

    while(oneReview != NULL)
        {	
        printReview(oneReview);

        while(1)
            {
            printf("\nDo you want to\n");
            printf("[1] - Like the review\n");
            printf("[2] - See next review\n");
            printf("[3] - Exit to main menu\n");
            printf("Enter here : ");
            fgets(stringInput,sizeof(stringInput),stdin);
            sscanf(stringInput, "%d", &yn);

            if(yn <= 3 && yn >= 1)
                {
                break;
                }
            else
                {
                printf("\tplease type 1, 2 or 3 only\n");
                }
            }

        if(yn == 1)
            {
            likeUser(loginUser,oneReview);
            }		
        if(yn == 3)
            {
            break;
            }
        oneReview = listGetNext(oneMovie->review);
        }						
    listReset(oneMovie->review);
}

/***************************************************
This function is for searching up the review by
genre of the movie
****************************************************/
void searchGenre(int findgenre, char* loginUsername)
{
    HASH_ITEM_T * oneHash = NULL;
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;
    int genreNum = 0;
    int genrefound = 0;
    int yn;
    int foundlike = 0;
    char stringInput[64];
    USER_T* loginUser = hashTableLookup(1, loginUsername);
    USER_T* oneUser = NULL;

    for(int i = 0; i < reviewTableSize; i++)
        {
        if (reviewTable[i].head != NULL)
            {
            oneHash = reviewTable[i].head;
            while(oneHash != NULL)
                {
                oneMovie = oneHash->data;
                oneReview = listGetNext(oneMovie->review);

                while(oneReview != NULL)
                    {	
                    genrefound = 0;
                    for(int i = 0; i < 6; i++)
                        {
                        if(findgenre == oneReview->genre[i])
                            {
                            genrefound = 1;
                            break;
                            }
                        }
                    if(genrefound == 1)
                        {
                        printReview(oneReview);

                    while(1)
                        {
                        printf("\nDo you want to\n");
                        printf("[1] - Like the review\n");
                        printf("[2] - See next review\n");
                        printf("[3] - Exit to main menu\n");
                        printf("Enter here : ");
                        fgets(stringInput,sizeof(stringInput),stdin);
                        sscanf(stringInput, "%d", &yn);

                        if(yn <= 3 && yn >= 1)
                            {
                            break;
                            }
                        else
                            {
                            printf("\tplease type 1, 2 or 3 only\n");
                            }
                        }

                    if(yn == 1)
                        {
                        likeUser(loginUser,oneReview);
                        }
                    }
                    oneReview = listGetNext(oneMovie->review);		
                    if(yn == 3)
                        {
                        break;
                        }					
                    }
                oneHash = oneHash->next;
                listReset(oneMovie->review);
                if (yn == 3)
                    {
                    break;
                    }
                }
            }
        if(yn == 3)
            {
            break;
            }
        }
}

/***************************************************
This function is for searching up the review by
date of the movie
****************************************************/
int searchDate(char* findDate, char* loginUsername)
{
    HASH_ITEM_T * oneHash = NULL;
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;
    USER_T * oneUser = NULL;
    int findDateCount = 0;
    int genreNum = 0;
    char stringInput[128];
    int yn;
    int foundlike = 0;
    USER_T* loginUser = hashTableLookup(1, loginUsername);
    int day;
    int month;
    int year;
    sscanf(findDate,"%d/%d/%d",&day,&month,&year);

    for(int i = 0; i < reviewTableSize; i++)
        {
        if (reviewTable[i].head != NULL)
            {
            oneHash = reviewTable[i].head;
            while(oneHash != NULL)
                {
                oneMovie = oneHash->data;
                oneReview = listGetNext(oneMovie->review);

                while(oneReview != NULL)
                    {	
                    if(day == oneReview->day && month == oneReview->month && year == oneReview->year)
                        {
                        findDateCount++;
                        printReview(oneReview);

                        while(1)
                            {
                            printf("\n\nDo you want to\n");
                            printf("[1] - Like the review\n");
                            printf("[2] - See next review\n");
                            printf("[3] - Exit to main menu\n\n");
                            printf("Enter here : ");
                            fgets(stringInput,sizeof(stringInput),stdin);
                            sscanf(stringInput, "%d", &yn);

                            if(yn <= 3 && yn >= 1)
                                {
                                break;
                                }
                            else
                                {
                                printf("\tplease type 1, 2 or 3 only\n");
                                }
                            }
                        if(yn == 1)
                            {
                            likeUser(loginUser,oneReview);
                            }
                        }
                    oneReview = listGetNext(oneMovie->review);		
                    if(yn == 3)
                        {
                        break;
                        }					
                    }
                oneHash = oneHash->next;
                listReset(oneMovie->review);
                if (yn == 3)
                    {
                    break;
                    }
                }
            }
        if(yn == 3)
            {
            break;
            }
        }
    return findDateCount;
}

/****************************************
This function is for displaying new feed 
to the user
*****************************************/
void newFeed(char* loginUsername)
{
    HASH_ITEM_T * oneHash = NULL;
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;
    USER_T * currentUser = hashTableLookup(1, loginUsername);
    int reviewcount = 0;

    printf("\n**********************************************\n");
    printf("                  NEW FEED \n");
    printf("**********************************************\n\n");

    for(int i = 0; i < reviewTableSize; i++)
        {
        if (reviewTable[i].head != NULL)
            {
            oneHash = reviewTable[i].head;
            while(oneHash != NULL)
                {
                oneMovie = oneHash->data;
                oneReview = listGetNext(oneMovie->review);
                while(oneReview != NULL)
                    {	
                    if((oneReview->year > currentUser->lastLoginYear) || (oneReview->year == currentUser->lastLoginYear && oneReview->month > currentUser->lastLoginMonth) || (oneReview->year == currentUser->lastLoginYear && oneReview->month == currentUser->lastLoginMonth && oneReview->day > currentUser->lastLoginDay))
                        {
                        reviewcount++;
                        printf("#%s\n",oneReview->moviename);
                        printf("Username      : %s\n",oneReview->username);
                        printf("Date          : %d/%d/%d\n",oneReview->day,oneReview->month,oneReview->year);
                        printf("Genre         : %d %d %d %d %d %d\n",oneReview->genre[0],oneReview->genre[1], oneReview->genre[2],oneReview->genre[3], oneReview->genre[4], oneReview->genre[5]);
                        printf("Like          : %d\n",oneReview->likeReview);
                        printf("Rating(1-10)  : %.1lf\n",oneReview->rating);
                        printf("Detail        : %s\n\n",oneReview->detail);
                        }
                    oneReview = listGetNext(oneMovie->review);
                    }
                oneHash = oneHash->next;
                listReset(oneMovie->review);
                }
            }
        }
    if(reviewcount == 0)
        {
        printf("\tYou are up to date\n\n");
        }
}

/***********************************
This function is for displaying the
suggested people to follow
************************************/
void displayFollowSuggest(char loginUsername[])
{
    USER_T *usersearch = NULL;
    USER_T *userSuggest = NULL;
    USER_T *userSuggestLike = NULL;
    USER_T *currentUserLike = NULL;
    USER_T *currentUserfollowing = NULL;
    HASH_ITEM_T * oneHash = NULL;
    USER_T * currentUser = hashTableLookup(1, loginUsername);
    int suggest = 0;
    int counter = 0;
    int follow = 1;
    int yn;
    char stringInput[64];
    char usernameSearch[64];
    char nameKeep[5][64];
    int nameval = 0;
    USER_T* followUser = NULL;

    printf("\e[1;1H\e[2J");
    printf("\n**********************************************\n");
    printf("             SUGGESTED USER LIST \n");
    printf("**********************************************\n\n");
    for(int i = 0; i < userTableSize; i++)
        {
        if (userTable[i].head != NULL)
            {
            oneHash = userTable[i].head;
            while(oneHash != NULL)
                {
                userSuggest = oneHash->data;
                follow = 1;
                suggest = 0;

                for(int i = 0; i<3;i++)
                    {
                    for(int j = 0; j<3;j++)
                        {	
                        /*if tey have the same genre the suggest*/
                        if(currentUser->favGenre[i] == userSuggest->favGenre[j])
                            {
                            suggest = 1;
                            break;
                            }
                        }

                        if(suggest == 1)
                            {
                            break;
                            }
                        }

                    listReset(userSuggest->likedUser);
                    userSuggestLike = listGetNext(userSuggest->likedUser);
                    while(userSuggestLike != NULL)
                        {
                        if(suggest == 1)
                            {
                            break;
                            }

                        listReset(currentUser->likedUser);
                        currentUserLike = listGetNext(currentUser->likedUser);
                        while(currentUserLike != NULL)
                        {
                    if(suggest == 1)
                        {
                        break;
                        }

                    if(strcmp(currentUserLike->username, userSuggestLike->username) == 0)
                        {
                        suggest = 1;
                        break;
                        }
                    currentUserLike = listGetNext(currentUser->likedUser);
                    }

                userSuggestLike = listGetNext(userSuggest->likedUser);
                }
    
            listReset(currentUser->likedUser);
            currentUserLike = listGetNext(currentUser->likedUser);
            while(currentUserLike != NULL)
                {
                if(suggest == 1)
                    {
                    break;
                    }

                if(currentUserLike == userSuggest)
                    {
                    suggest = 1;
                    break;
                    }
                currentUserLike = listGetNext(currentUser->likedUser);
                }
            listReset(currentUser->likedUser);
            listReset(currentUser->followingUser);
            currentUserfollowing = listGetNext(currentUser->followingUser);
            while(currentUserfollowing != NULL)
                {
                if(currentUserfollowing == userSuggest)
                    {
                    follow = 0;
                    break;
                    }
                currentUserfollowing = listGetNext(currentUser->followingUser);
                }
            listReset(currentUser->followingUser);

            if ((suggest == 1) && (currentUser != userSuggest) && (follow == 1))
                {
                strcpy(nameKeep[counter%5], userSuggest->username);
                counter++;
                printf("Username         : %s\n",userSuggest->username);
                }

            if(counter%5 == 0)
                {
                while(1)
                    {
                    printf("\n\nDo you want to\n");
                    printf("[1] - Follow user\n");
                    printf("[2] - See more suggestion\n");
                    printf("[3] - Exit to main menu\n\n");
                    printf("Enter here : ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput, "%d", &yn);

                    if(yn <= 3 && yn >= 1)
                        {
                        break;
                        }
                    else
                        {
                        printf("\tplease type 1, 2 or 3 only\n");
                        }
                    }

                if(yn == 1)
                    {
                    printf("\n");
                    while(1)
                        {
                        printf("\nEnter username you want to follow: ");
                        fgets(stringInput,sizeof(stringInput),stdin);
                        sscanf(stringInput,"%s",usernameSearch);

                        for(int i= 0; i<5 ; i++)
                            {
                            if(strcmp(usernameSearch,nameKeep[i]) == 0)
                                {
                                nameval = 1;
                                break;
                                }
                            }
    
                        if(nameval == 1)
                            {
                            currentUser->followingCount++;
                            if(listAtEnd(currentUser->followingUser) == -1)
                                {
                                currentUser->followingUser = newList();
                                }
                            followUser = hashTableLookup(1, usernameSearch);
                            listInsertEnd(currentUser->followingUser, followUser);
                            printf("\tYou have follwed %s\n",usernameSearch);
                            break;
                            }
                        else
                            {
                            printf("\t%s is not a valid user\n",usernameSearch);
                            }
                        }
                    }
                else if (yn == 2)
                    {
                    memset(nameKeep, 0, 0);
                    }
                else if(yn == 3)
                    {
                    break;
                    }

                }
                oneHash = oneHash->next;
                if(yn ==3)
                    {
                    break;
                    }
                }
            if(yn ==3)
                {
                break;
                }
            }
        if(yn ==3)
            {
            break;
            }
        }
    if(counter == 0)
        {
        printf("The user have no suggested user to follow\n");
        }
    else if(yn != 3)
        {
        while(1)
            {
            printf("\n\nDo you want to\n");
            printf("[1] - Follow user\n");
            printf("[2] - Exit to main menu\n\n");
            printf("Enter here : ");
            fgets(stringInput,sizeof(stringInput),stdin);
            sscanf(stringInput, "%d", &yn);

            if(yn <= 2 && yn >= 1)
                {
                break;
                }
            else
                {
                printf("\tplease type 1 or 2 only\n");
                }
            }

        if(yn == 1)
            {
            while(1)
                {
                printf("\nEnter username you want to follow: ");
                fgets(stringInput,sizeof(stringInput),stdin);
                sscanf(stringInput,"%s",usernameSearch);

                for(int i= 0; i<5 ; i++)
                    {
                    if(strcmp(usernameSearch,nameKeep[i]) == 0)
                        {
                        nameval = 1;
                        break;
                        }
                    }

                if(nameval == 1)
                    {
                    currentUser->followingCount++;
                    if(listAtEnd(currentUser->followingUser) == -1)
                        {    
                        currentUser->followingUser = newList();
                        }
                    followUser = hashTableLookup(1, usernameSearch);
                    listInsertEnd(currentUser->followingUser, followUser);
                    printf("\tYou have follwed %s\n",usernameSearch);
                    break;
                    }
                else
                    {
                    printf("\t%s is not a valid user\n",usernameSearch);
                    }
                }
            }
        }
}

/***************************************************
This function is use in combination with function
edit movie where it allow user to edit their review
and or delete the review
****************************************************/
int searchAndEdit(char* findmovie, char* loginUsername)
{
    char stringInput[2004];
    char checkinput[64];
    char reviewDetail[2004];
    double rating = 0;
    int choice = 0;
    int genrecount = 0;
    int genreRepeat = 1;
    int genrebuff = 0;
    int genre[6];
    REVIEW_T * oneReview = NULL;
    MOVIE_T * oneMovie = NULL;
    int position = 0;

    oneMovie = hashTableLookup(2, findmovie);
    listReset(oneMovie->review);
    oneReview = listGetNext(oneMovie->review);
    while(1)
        {
        /*displaying only the review that the current user wrote*/  
        if (strcmp(oneReview->username,loginUsername) != 0)
            {
            /*position is for later if the user decide to delete*/
            position++;
            oneReview = listGetNext(oneMovie->review);
            if (oneReview == NULL)
                {
                return 2;
                }
            }
        else
            {
            while(1)
                {
                printReview(oneReview);
                printf("\nWhat do you want to do?\n");
                printf("[1] - Edit genre\n");
                printf("[2] - Edit rating\n");
                printf("[3] - Edit detail\n");
                printf("[4] - Delete review\n");
                printf("[5] - Exit\n");

                printf("Enter here:");
                fgets(stringInput,sizeof(stringInput),stdin);
                sscanf(stringInput,"%d",&choice);
                if (choice == 1)
                    {
                    while(1)
                        {
                        printf("\nHow many genre are in this movie?[Maximum of 6 genre]: ");
                        fgets(stringInput,sizeof(stringInput),stdin);
                        sscanf(stringInput,"%s",checkinput);
                        sscanf(stringInput,"%d",&genrecount);
                        if(checkDigit(checkinput) == 0)
                            {
                            printf("\n\tInvalid value, numbers only\n");
                            }
                        else if (genrecount > 6 || genrecount < 1)
                            {
                            printf("\n\tInvalid value, 1 to 6 genres only\n");
                            }
                        else		
                            break;
                        }

                        printf("\n[1] Drama\n");
                        printf("[2] Romance\n");
                        printf("[3] Action\n");
                        printf("[4] Comedy\n");
                        printf("[5] Horror\n");
                        printf("[6] Fantasy\n");

                        for (int i = 0; i < genrecount; ++i)
                            {
                            while(1)
                                {
                                genreRepeat = 1;
                                printf("genre #%d:", i + 1);
                                fgets(stringInput,sizeof(stringInput),stdin);
                                sscanf(stringInput,"%s",checkinput);
                                sscanf(stringInput,"%d",&genrebuff);
                                if(checkDigit(checkinput) == 0)
                                    {
                                    printf("\n\tInvalid value, numbers only\n");
                                    }
                                else if (genrebuff > 6 || genrebuff < 1)
                                    {
                                    printf("\nInvalid value, 1 to 6 only \n");
                                    }
                                else 
                                    {
                                    genrebuff--;
                                    for (int j = 0; j < i; j++)
                                        {
                                        if (genrebuff == genre[j])
                                        {
                                    printf("repeated genre\n");
                                    genreRepeat = 0;
                                    break;
                                    }
                                }
                            if(genreRepeat == 1)
                                {
                                genre[i] = genrebuff;
                                break;
                                }
                            }
                        }
                    }
                for(int i = genrecount; i < 6; i++)
                    {
                    genre[i] = genrebuff;
                    }
                for (int i = 0; i < 6; ++i)
                    {
                    oneReview->genre[i] = genre[i];
                    }
                listReset(oneMovie->review);
                }
            else if (choice == 2)
                {
                while(1)
                    {
                    printf("\nHow would you rate this movie out of ten: ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput,"%s",checkinput);
                    sscanf(stringInput,"%lf",&rating);
                    if(checkDigit(checkinput) == 0)
                        {
                        printf("\n\tInvalid value, numbers only\n");
                        }
                    else if (rating > 10 || rating < 0)
                        {
                        printf("\nPlease try again. (0 - 10) \n");
                        }
                    else
                        {
                        break;
                        }
                    }
                oneReview->rating = rating;
                listReset(oneMovie->review);
                }
            else if (choice == 3)
                {
                while(1)
                    {
                    printf("What is the detail of your review?: ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput,"%[^\n]",reviewDetail);
                    if (strlen(reviewDetail) > 1000)
                        {
                        printf("\nMaximum amount of characters in a review is 1000.\nPlease try again.:");
                        }
                    else if (strlen(reviewDetail) == 0)
                        {
                        printf("\nYou haven't written anything yet.\nPlease try again.:");
                        }
                    else
                        {	
                        break;
                        }
                    }
                strcpy(oneReview->detail,reviewDetail);
                listReset(oneMovie->review);
                }
            else if (choice == 4)
                {
                listRemove(oneMovie->review,position);
                listReset(oneMovie->review);
                return 1;
                }
            else if(choice == 5)
                {
                return 1;
                }
            else
                {
                printf("Invalid input, Please try again.\n");
                }
            }
        }
    }
}

/***********************************************
This function is to ask for the movie name
and pass that name to search and edit function
************************************************/
int editMovie(char * username)
{
    MOVIE_T* moviesearch = NULL;
    int returnVal = 0;
    char inputline[128];
    char movieNameSearch[128];
    char answer;

    printf("\n**********************************************\n");
    printf("                  EDIT REVIEW \n");
    printf("**********************************************");

    while(1)
        {
        /*enter a movie name and pass to searchAndEdit*/
        printf("\nEnter moive name : ");
        fgets(inputline,sizeof(inputline),stdin);
        sscanf(inputline,"%[^\n]",movieNameSearch);
        moviesearch = hashTableLookup(2, movieNameSearch);
        if (moviesearch != NULL)
            {
            returnVal = searchAndEdit(movieNameSearch, username);
            if (returnVal == 1)
                {
                return 1;
                }
            else
                {
                printf("\tThe user did not review in this movie\n");
                while(1)
                    {
                    printf("do you want to try again?(Y/N): ");
                    fgets(inputline,sizeof(inputline),stdin);
                    sscanf(inputline,"%c",&answer);
                    if (answer == 'y' || answer == 'Y')
                        {
                        break;
                        }
                    else if(answer == 'n' || answer == 'N')
                        {	
                        return 1;
                        }
                    else
                        {
                        printf("\tPlease input y or n only\n");
                        }
                    }
                }
            }    
        else
            {
            printf("\tThere are no review in this movie\n");
            while(1)
                {
                printf("do you want to try again?(Y/N): ");
                fgets(inputline,sizeof(inputline),stdin);
                sscanf(inputline,"%c",&answer);
                if (answer == 'y' || answer == 'Y')
                    {
                    break;
                    }
                else if(answer == 'n' || answer == 'N')
                    {	
                    return 1;
                    }
                else
                    {
                    printf("\tPlease input y or n only\n");
                    }
                }
            }
        }
}

/********************************************
 * This function is for writting new Review
 * return 1 if write review successfully
 *				0 if there are errors
 ********************************************/
int writeNewReview(char username[])
{
    REVIEW_T *oneReview = NULL;
    MOVIE_T *oneMovie = NULL;
    USER_T *user = hashTableLookup(1, username);
    REVIEW_T *reviewsearch = NULL;
    MOVIE_T *moviesearch = NULL;
    int bOk = 0;
    int genrecount = 0;
    double rating = 0;
    int genrebuff = 0;
    int genre[6];
    int pReviewCollision = 0;
    char stringInput[2004];
    char movieName[64];
    char reviewDetail[2004];
    int digitval;
    char checkinput[64];
    int genreRepeat = 1;
    int movienameval = 0;

    printf("\e[1;1H\e[2J");
    printf("\n**********************************************\n");
    printf("                WRITE NEW REVIEW \n");
    printf("**********************************************\n\n");

    /*ask what movie to review*/
    while(1)
    {
        printf("What movie do you want to review: ");
        memset(movieName, 0, sizeof (movieName));
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%[^\n]",movieName);
        if (strlen(movieName) == 0)
        {
            printf("\n\tPlease type a movie name");
        }
        else
        {
            moviesearch = hashTableLookup(2,movieName);
            if(moviesearch == NULL)
            {
                break;
            }
            listReset(moviesearch->review);
            reviewsearch = listGetNext(moviesearch->review);
            while(reviewsearch != NULL)
            {
                if(strcmp(reviewsearch->username,username) == 0)
                {
                    movienameval = 1;
                    break;
                }
                reviewsearch = listGetNext(moviesearch->review);
            }
            listReset(moviesearch->review);

            if(movienameval == 0)
            {
                break;
            }
            else
            {
                printf("\n\tUser cannot have to review of the same movie\n");
            }
        }
    }
    /*what genre is in the movie*/
    while(1)
    {
        printf("\nHow many genre are in this movie?[Maximum of 6 genre]: ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",checkinput);
        sscanf(stringInput,"%d",&genrecount);
        if(checkDigit(checkinput) == 0)
        {
            printf("\n\tInvalid value, numbers only\n");
        }
        else if (genrecount > 6 || genrecount < 1)
        {
            printf("\n\tInvalid value, 1 to 6 genres only\n");
        }
        else
		{			
			break;
		}
    }
    printf("\n[1] Drama\n");
    printf("[2] Romance\n");
    printf("[3] Action\n");
    printf("[4] Comedy\n");
    printf("[5] Horror\n");
    printf("[6] Fantasy\n");
     
	for (int i = 0; i < genrecount; ++i)
    {
        while(1)
        {
            genreRepeat = 1;
            printf("genre #%d:", i + 1);
            fgets(stringInput,sizeof(stringInput),stdin);
            sscanf(stringInput,"%s",checkinput);
            sscanf(stringInput,"%d",&genrebuff);
            if(checkDigit(checkinput) == 0)
            {
                printf("\n\tInvalid value, numbers only\n");
            }
            else if (genrebuff > 6 || genrebuff < 1)
            {
                printf("\n\tInvalid value, 1 to 6 only \n");
            }
            else 
            {
                genrebuff--;
                for (int j = 0; j < i; j++)
                {
                    if (genrebuff == genre[j])
                    {
                        printf("\trepeated genre\n");
                        genreRepeat = 0;
                        break;
                    }
                }

                if(genreRepeat == 1)
                {
                    genre[i] = genrebuff;
                    break;
                }
            }
        }
    }

    for(int i = genrecount; i < 6; i++)
    {
        genre[i] = genrebuff;
    }

    while(1)
    {
        printf("\nHow would you rate this movie out of ten: ");
        memset(stringInput,0,sizeof(stringInput));
        memset(checkinput,0,sizeof(checkinput));
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",checkinput);
        sscanf(stringInput,"%lf",&rating);
        if (strlen(checkinput) == 0)
        {
            printf("\tPlease type a number");
        }
        else
        {
            if(checkDigit(checkinput) == 0)
            {
                printf("\n\tInvalid value, numbers only\n");
            }
            else if (rating > 10 || rating < 0)
            {
                printf("\n\tPlease try again. (0 - 10) \n");
            }
            else
            {
                break;
            }
        }
    }
    /*Write the info into struct*/
    while(1)
    {
        memset(stringInput, 0, sizeof(stringInput));
        memset(reviewDetail, 0, sizeof (reviewDetail));
        printf("\nPlease write a review about the movie (only 1000 characters): ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%[^\n]",reviewDetail);
        if (strlen(reviewDetail) > 1000)
        {
            printf("\n\tMaximum amount of characters in a review is 1000.Please try again.");
        }
        else if (strlen(reviewDetail) == 0)
        {
            printf("\n\tYou haven't written anything yet.Please try again.");
        }
        else
        {	
            break;
        }
    }
    oneReview = calloc(1, sizeof(REVIEW_T));
    sscanf(movieName,"%[^\n]",oneReview->moviename);
    oneMovie = hashTableLookup(2,oneReview->moviename);
    if(oneMovie == NULL)
    {
        oneMovie = calloc(1, sizeof(MOVIE_T));
        if(oneMovie == NULL)
        {
            printf("oneMovie calloc error");
            return 0;
        }
        strcpy(oneMovie->moviename,oneReview->moviename);
        hashTableInsert(2, oneMovie->moviename, oneMovie, &pReviewCollision);
        oneMovie->review = newList();
    }
	strcpy(oneReview->username,username);
    for (int i = 0; i < 6; ++i)
    {
        oneReview->genre[i] = genre[i];
    }
    dateToday(&oneReview->day,&oneReview->month,&oneReview->year);
    oneReview->likeReview = 0;
    oneReview->rating = rating;
    strcpy(oneReview->detail,reviewDetail);
    listInsertEnd(oneMovie->review, oneReview);
    return 1;
}
