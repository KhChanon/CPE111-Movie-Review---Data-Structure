/******************************************************
This file is for reading functions. For reading files
and use those information for later
created by:
Pechdanai Saepong (Nine) 63070503434 
Fasai Sae-Tae (Atom) 63070503436 
Chanon Khanijoh (Sean) 63070503408 
Pokpong jitjomnongmate (Paul) 63070503425
*******************************************************/

typedef struct _listuser
{
    char username[32];
    char password[32];
    char email[64];
    int favGenre[3];
    int lastLoginDay;
    int lastLoginMonth;
    int lastLoginYear;
    int likeCount;
    LIST_HANDLE likedUser;
    int followingCount;
    LIST_HANDLE followingUser;
} USER_T;

typedef struct _listmovie
{
    char moviename[32];
    LIST_HANDLE review;
} MOVIE_T;

typedef struct _listreview
{
    char moviename[32];
    char username[32];
    int day;
    int month;
    int year;
    int genre[6];
    int likeReview;
    double rating;
    char detail[1004];
} REVIEW_T;

/*****************************************************
  this function will read the userInformation.txt and 
  store the information into user hash table
******************************************************/
void readUser();

/***********************************************
  this function will read the likeuser.txt and 
  store the information into user hash table
************************************************/
void readLikeUser ();

/***************************************************
  this function will read the followinguser.txt and 
  store the information into user hash table
****************************************************/
void readFollowingUser ();

/**********************************
This function is for writing down
the liked user
***********************************/
void writeLikedUser(USER_T* user);

/**********************************
This function is for writing down
the following of a user
***********************************/
void writeFollowingUser(USER_T* user);

/*********************************
This function is for reading the
review text
**********************************/
void readReview();

/**************************************************
 * Check if the searched user is in the list or not
 * return 1 if they are
 * 			  0 if not
***************************************************/
int checkFollowingFind(USER_T * user, char findname[]);
