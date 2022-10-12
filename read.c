/******************************************************
	This file is for reading functions. For reading files
	and use those information for later
	created by:
	Pechdanai Saepong (Nine) 63070503434 
	Fasai Sae-Tae (Atom) 63070503436 
	Chanon Khanijoh (Sean) 63070503408 
	Pokpong jitjomnongmate (Paul) 63070503425
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linkedListUtil.h"
#include "feature.h"
#include "read.h"
#include "validation.h"

/*************************************
 * This is to define all the designated
 * genre of movie with number
 *************************************/
#define DRAMA 0
#define ROMANCE 1
#define ACTION 2
#define COMEDY 3
#define HORROR 4
#define FANTASY 5
/***************************
 * Set value for true or false
 ***************************/
#define TRUE 1
#define FALSE 0

int hashSize = 503;

/*****************************************************
 * this function will read the userInformation.txt and 
 * store the information into user hash table
 *****************************************************/
void readUser()
{
    FILE * pIn = NULL;
    char * filename = "userInformation.txt";
    char inputline[128];
    USER_T *oneUser = NULL;
    int pCollision = 0;
    /*open userInformation.txt*/
    pIn = fopen(filename,"r");
    /*if the filedoesn't exist then exit the program*/
    if(pIn == NULL)
    {
        printf("User database not found\n");
        exit(0);
    }
    /*to initialized the table*/
    hashTableInit(1, hashSize, &userBitwiseOpHash);
    /*while there is line in the file*/
    while(fgets(inputline,sizeof(inputline),pIn) != NULL)
    {
        /*if there is "Username" then allocate 1 new block and put in oneUser->username*/
        if(strncmp(inputline,"Username",8) == 0)
        {
            oneUser = (USER_T*) calloc(1,sizeof(USER_T));
    
            if(oneUser == NULL)
            {
                printf("oneUser allocation error");
                exit(0);
            }
      
            sscanf(inputline,"Username         : %s",oneUser->username);
        }
        /*if there is "Password" then put in oneUser->password*/
        else if(strncmp(inputline,"Password",8) == 0)
        {
            sscanf(inputline,"Password         : %s",oneUser->password);
        }
        /*if there is "Email" then put in oneUser->email*/
        else if(strncmp(inputline,"Email",5) == 0)
        {
            sscanf(inputline,"Email            : %s",oneUser->email);
        }
        /*if there is "Genre" then put in oneUser->genre*/
        else if(strncmp(inputline,"Genre",5) == 0)
        {
            sscanf(inputline,"Genre            : %d %d %d",&oneUser->favGenre[0], &oneUser->favGenre[1], &oneUser->favGenre[2]);
        }
        else if(strncmp(inputline,"Last Login",10) == 0)
        {
            sscanf(inputline,"Last Login       : %d/%d/%d",&oneUser->lastLoginDay,&oneUser->lastLoginMonth,&oneUser->lastLoginYear);
            hashTableInsert(1, oneUser->username, oneUser, &pCollision);
        }
    }
    fclose(pIn);
}

/***********************************************
 * this function will read the likeuser.txt and 
 * store the information into user hash table
 ***********************************************/
void readLikeUser()
{
    FILE * pIn = NULL;
    char * filename = "likeuser.txt";
    char inputline[128];
    USER_T* oneUser = NULL;
    USER_T* likeUser = NULL;
    char username[128];
    char likeUsername[128];
    /*read file likeuser.txt*/
    pIn = fopen(filename,"r");
    /*if the file doesn't exist then exit*/
    if(pIn == NULL)
    {
        printf("LikedUser database not found\n");
        exit(0);
    }

    while (fgets(inputline,sizeof(inputline),pIn) != NULL)
    {
        if (strncmp(inputline,"#",1) == 0)
        {
            sscanf(inputline,"#%s",username);
            oneUser = hashTableLookup(1, username);
        }
        else if (strncmp(inputline,"LikeCount",9) == 0)
        {
            sscanf(inputline,"LikeCount        : %d",&oneUser->likeCount);
        }
        else if(strncmp(inputline,"Like",4) == 0)
        {
            sscanf(inputline,"Like             : %s", likeUsername);
            likeUser = hashTableLookup(1, likeUsername);
            oneUser->likedUser = newList();
            listInsertEnd(oneUser->likedUser, likeUser);

            for(int i = 1; i < oneUser->likeCount; i++)
            {
                fgets(inputline,sizeof(inputline),pIn);
                sscanf(inputline,"|                  %s",likeUsername);
                likeUser = hashTableLookup(1, likeUsername);
                listInsertEnd(oneUser->likedUser, likeUser);
            }
        }
    }
    fclose(pIn);
}
/***************************************************
 * this function will read the followinguser.txt and 
 * store the information into user hash table
 ***************************************************/
void readFollowingUser()
{
    FILE * pIn = NULL;
    char * filename = "followinguser.txt";
    char inputline[128];
    USER_T* oneUser;
    USER_T* followingUser;
    int followingCount;
    char username[128];
    char followingUsername[128];
    /*read followinguser.txt*/
    pIn = fopen(filename,"r");
    /*while there is no next line left*/
    while (fgets(inputline,sizeof(inputline),pIn)!=NULL)
    {
        /*if the line contain "#" put number into hashtableloopup*/
        if (strncmp(inputline,"#",1) == 0)
        {
            sscanf(inputline,"#%s",username);
            oneUser = hashTableLookup(1, username);
        }
        /*else if the line contain "followingCount" then put the count into following count struct*/
        else if(strncmp(inputline,"FollowingCount",14) == 0)
        {
            sscanf(inputline,"FollowingCount    : %d", &oneUser->followingCount);
        }
        /*else if the line contain "followingUser" then put the count into following user struct*/
        else if(strncmp(inputline,"FollowingUser",13) == 0)
        {
            sscanf(inputline,"FollowingUser    : %s",followingUsername);
            followingUser = hashTableLookup(1, followingUsername);
            oneUser->followingUser = newList();
            listInsertEnd(oneUser->followingUser, followingUser);
            /*until i is more than following count put the sscanf the username into the hash table*/
            for(int i = 1; i < oneUser->followingCount; i++)
            {
                fgets(inputline,sizeof(inputline),pIn);
                sscanf(inputline,"|                  %s",followingUsername);
                followingUser = hashTableLookup(1, followingUsername);
                listInsertEnd(oneUser->followingUser, followingUser);
            }
        }
    }
    fclose(pIn);
}

/**********************************
 * This function is for writing down
 * the liked user
 **********************************/
void writeLikedUser(USER_T* user)
{
    FILE * pOut = NULL;
    char filename[128] = "likeuser.txt";
    pOut = fopen(filename,"a");
    /*set the struct to NULL*/
    USER_T *pDummy = NULL;
    USER_T *pCurrentUser = user;
    /*if the likecount is more than 0 then call listgetnext*/
	
    if(pCurrentUser->likeCount > 0)
    {
        listReset(pCurrentUser->likedUser);
        pDummy = listGetNext(pCurrentUser->likedUser);
        fprintf(pOut, "#%s\n",pCurrentUser->username);
        fprintf(pOut, "LikeCount        : %d\n",pCurrentUser->likeCount);
        fprintf(pOut, "Like             : %s\n",pDummy->username);
        while(1)
        {
            pDummy = listGetNext(pCurrentUser->likedUser);
            /*if pDummy is NULL then break the loop*/
            if(pDummy == NULL)
            {
                break;
            }
            fprintf(pOut, "|                  %s\n",pDummy->username);
        }
        fprintf(pOut,"\n");
    }
    fclose(pOut);
}

/***********************************
 * This function is for writing down
 * the following of a user
 **********************************/
void writeFollowingUser(USER_T* user)
{
    FILE * pOut = NULL;
    char filename[128] = "followinguser.txt";
    /*open file following user*/
    pOut = fopen(filename,"a");

    /*set the struct to NULL*/
    USER_T *pDummy = NULL;
    USER_T *pCurrentUser = user;
    /*if the following count is more than zero then call listgetnext*/
    if(pCurrentUser->followingCount > 0)
    {
        listReset(pCurrentUser->followingUser);
        pDummy = listGetNext(pCurrentUser->followingUser);
        fprintf(pOut, "#%s\n", pCurrentUser->username);
        fprintf(pOut, "FollowingCount    : %d\n", pCurrentUser->followingCount);
        fprintf(pOut, "FollowingUser     : %s\n", pDummy->username);
        /*loop until the pDummy is NULL*/
        while(1)
        {
            pDummy = listGetNext(pCurrentUser->followingUser);
            if(pDummy == NULL)
            {
                break;
            }
            /*write the username*/
            fprintf(pOut, "|                   %s\n", pDummy->username);
        }
        fprintf(pOut,"\n");
    }
    fclose(pOut);
}

/*********************************
 * This function is for reading the
 * review text
 **********************************/
void readReview()
{
    FILE * pIn = NULL;
    char * filename = "review.txt";
    char inputline[1004];
    REVIEW_T *oneReview = NULL;
    MOVIE_T *oneMovie = NULL;
    int pReviewCollision = 0;

    pIn = fopen(filename,"r");

    /*if the file doesn't exist print error*/
    if(pIn == NULL)
    {
        printf("Review database not found\n");
        exit(0);
    }

    hashTableInit(2, hashSize, &userBitwiseOpHash);

    /*while there are still lines in the text*/
    while(fgets(inputline,sizeof(inputline),pIn) != NULL)
    {
        /*if there are # allocate one new space*/
        if(strncmp(inputline,"#",1) == 0)
        {
            oneReview = calloc(1, sizeof(REVIEW_T));
            if(oneReview == NULL)
            {
                printf("oneReview calloc error");
                exit(0);
            }

            sscanf(inputline,"#%[^\n]",oneReview->moviename);
            oneMovie = hashTableLookup(2,oneReview->moviename);
            if(oneMovie == NULL)
            {
                oneMovie = calloc(1, sizeof(MOVIE_T));
                if(oneMovie == NULL)
                {
                    printf("oneMovie calloc error");
                    exit(0);
                }
				
                strcpy(oneMovie->moviename,oneReview->moviename);
                hashTableInsert(2, oneMovie->moviename, oneMovie, &pReviewCollision);
                oneMovie->review = newList();
            }
        }
        /*else if there are "username" put the username in the struct*/
        else if(strncmp(inputline,"Username",8) == 0)
        {
            sscanf(inputline,"Username      : %s",oneReview->username);
        }
        /*else if there are "date" put the date in the struct*/
        else if(strncmp(inputline,"Date",4) == 0)
        {
            sscanf(inputline,"Date          : %d/%d/%d\n",&oneReview->day,&oneReview->month,&oneReview->year);
        }
        /*else if there are "genre" put the genre in the struct*/
        else if (strncmp(inputline,"Genre",5) == 0)
        {
            sscanf(inputline,"Genre         : %d %d %d %d %d %d",&oneReview->genre[0], &oneReview->genre[1], &oneReview->genre[2], &oneReview->genre[3], &oneReview->genre[4], &oneReview->genre[5]);
        }
        else if(strncmp(inputline,"Like",4) == 0)
        {
            sscanf(inputline,"Like          : %d",&oneReview->likeReview);
        }
        else if(strncmp(inputline, "Rating(1-10)", 12) == 0)
        {
            sscanf(inputline,"Rating(1-10)  : %lf",&oneReview->rating);
        }
        else if(strncmp(inputline,"Detail",6) == 0)
        {
            sscanf(inputline,"Detail        : %[^\n]",oneReview->detail);
            listInsertEnd(oneMovie->review, oneReview);
        }	
    }
    fclose(pIn);
}


/**************************************************
 * Check if the searched user is in the list or not
 * return 1 if they are
 * 			  0 if not
***************************************************/
int checkFollowingFind(USER_T * user, char findname[])
{
    int result = 0;
    USER_T * followingName = NULL;
    listReset(user->followingUser);
    while (1)
    {
        followingName = listGetNext(user->followingUser);
        if (followingName == NULL)
        {
            break;
        }

        if(strcmp(followingName->username,findname) == 0)
        {
            result = 1;
            break;
        }
    }
    listReset(user->followingUser);
    return result;
}

