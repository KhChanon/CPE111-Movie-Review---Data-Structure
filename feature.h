/* 
 *   abstractHashTable.h
 *
 *   Header file for a hash table that uses strings as keys
 *   and stores pointers to arbitrary structures as values
 *
 *   Created by Sally Goldin on 5 March 2012 for CPE 113
 * This file is being borrowed for use of project by
 * team : WHAT
 * member: 
 * Pechdanai Saepong (Nine) 63070503434 
 * Fasai Sae-Tae (Atom) 63070503436 
 * Chanon Khanijoh (Sean) 63070503408 
 * Pokpong jitjomnongmate (Paul) 63070503425
 */
#ifndef ABSTRACTHASHTABLE_H
#define ABSTRACTHASHTABLE_H

/* 
  Return the number of slots in the hash table.
*/
int userHashTableSize();

int reviewHashTableSize();

/* Return the number of items currently stored in the hash table.
 */
int userHashTableItemCount();

int reviewHashTableItemCount();


/* Initialize the hash table.
 * Arguments
 *    size                - How many slots in the table
 *                          Must be 1 or greater. We assume the caller
 *                          has checked this.
 *    hashFunction        - Function that takes a string and returns an int
 *                          which will be the index into the table.
 * Returns 1 if successful, 0 if some error occurred.
 */
int hashTableInit(int table, int size, unsigned int (*hashFunction)(char* key));


/* Free the hash table.
 */
void hashTableFree(int table);


/* Insert a value into the hash table.
 * Arguments 
 *    key                 - character string key
 *    data                - data to store in the table
 *    pCollision          - set to true if there was a collision storing
 *                          the data, else false 
 * Returns true (1) unless hash table has not been initialized or
 * we can't allocate memory, in which case returns false (0)
 */
int hashTableInsert(int table, char* key, void* data,int* pCollision);


/* Look up a value in the hash table.
 * Arguments 
 *    key                 - character string key
 * Returns the data associated with the key, or NULL if 
 * data associated with the key is not found.
 */
void* hashTableLookup(int table, char* key);

/* Robust hash function that uses bitwise operations to
 * modify string values. Adapted from Kyle Loudon,
 * "Mastering Algorithms with C"
 */
unsigned int userBitwiseOpHash(char* key);

/* Robust hash function that uses bitwise operations to
 * modify string values. Adapted from Kyle Loudon,
 * "Mastering Algorithms with C"
 */
unsigned int reviewBitwiseOpHash(char* key);

/*****************************************************
  This function will go through the User hash table
  and write all the data along with the new 
  information of user after register new account.
*****************************************************/
void writeUserHash();

/********************************************
this function is for writing review from the
struct into a text file
*********************************************/
void writeReview();

/*****************************************************
Find reviews of movies by movie name, movie type, 
review date or user who wrote the review.
******************************************************/
int searchUser(char* finduser, char* loginUsername);

/***************************************************
This function is for searching up the review by
movie title
****************************************************/
void searchMovie(char* findmovie, char* loginUsername);

/***************************************************
This function is for searching up the review by
genre of the movie
****************************************************/
void searchGenre(int findgenre, char* loginUsername);

/***************************************************
This function is for searching up the review by
date of the movie
****************************************************/
int searchDate(char* findDate, char* loginUsername);

/****************************************
This function is for displaying new feed 
to the user
*****************************************/
void newFeed(char* loginUsername);

/***********************************
	This function is for displaying the
	suggested people to follow
************************************/
void displayFollowSuggest(char* loginUsername);

/***********************************************
This function is to ask for the movie name
and pass that name to search and edit function
************************************************/
int editMovie(char * username);

/********************************************
 * This function is for writting new Review
 * return 1 if write review successfully
 *				0 if there are errors
 ********************************************/
int writeNewReview(char username[]);


#endif