/******************************************************
This is the main file that operation most of the work
from the login page to register to the main page

Create by : 
  Pechdanai Saepong (Nine) 63070503434 
  Fasai Sae-Tae (Atom) 63070503436 
  Chanon Khanijoh (Sean) 63070503408 
  Pokpong jitjomnongmate (Paul) 63070503425
*******************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "validation.h"
#include "linkedListUtil.h" 
#include "feature.h"
#include "read.h"

USER_T* currentUser; //current login user

/****************************************************
  this function is for user to go into the program
  return 1 for calling login function
         2 for register function
         3 for exit the program
*****************************************************/
int welcome()
{
    char stringInput[64];
    int welcomeChoice = -1;
	
    while(welcomeChoice<0)
        {
        printf("\n-------------------------------------------\n");
        printf("----- WELCOME TO MOVIE REVIEW NETWORK -----\n");
        printf("-------------------------------------------\n\n");
        printf("--------------- [1] LOGIN    --------------\n");
        printf("--------------- [2] REGISTER --------------\n");
        printf("--------------- [3] Exit     --------------\n\n");
        printf("    Select option : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%d",&welcomeChoice);
        if (welcomeChoice > 3 || welcomeChoice < 1)
            {
            printf("\n\tInvalid input, please try again\n");
            welcomeChoice = -1;
            }
        }
    return welcomeChoice;
}

/*************************************************************
  this function allow user to create new account
  -- return 0 there is an error in the registration
  -- 1 the account is completely registered without any error
**************************************************************/
int registerAcc()
{
    USER_T *oneUser = NULL;
    char stringInput[128];
    char username[128];
    char password[128];
    char email[128];
    int currentgenre;
    int genre[10];
    int pCollision = 0;
    int space = 0;
    int bOK = 0;
    printf("\e[1;1H\e[2J");

    oneUser = calloc(1,sizeof(USER_T));
    if(oneUser == NULL)
    {
        printf("oneUser calloc error");
        exit(0);
    }

    printf("**********************************************\n");
    printf("           WELCOME TO REGISTER PAGE\n");
    printf("**********************************************\n");

    while(bOK != 1)
    {
        memset(username, 0, 0);
        printf("Enter a username : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",username);
        bOK = usernameVal(username);

        if(hashTableLookup(1,username) != NULL)
        {
            printf("\n\tERROR, This username already exist\n");
            bOK = 0;
        }
    }
    bOK = 0;
    while(bOK != 1)
    {
        printf("\nEnter E-mail : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",email);
        bOK = checkEmail(email);
    }
    bOK = 0;
    while(bOK != 1)
    {
        printf("\nEnter Password : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",password);
        bOK = passwordVal(password);
    }
    bOK = 0;

    printf("\nEnter 3 favourite genre\n");
    printf("[1] Drama\n");
    printf("[2] Romance\n");
    printf("[3] Action\n");
    printf("[4] Comedy\n");
    printf("[5] Horror\n");
    printf("[6] Fantasy\n");
    while(bOK != 1)
    {
        printf("\nEnter Your Favourite Genre [number number number]: ");
        fgets(stringInput,sizeof(stringInput),stdin);
        if (strlen(stringInput) < 2)
        {
            printf("\tERROR, please enter your genre\n");
            bOK = 0;
        }
        else if (strlen(stringInput) > 6)
        {
            printf("\tERROR, allow only 3 genre\n");
            bOK = 0;
        }
        space = 0;
        for (int i = 0 ; i < strlen(stringInput); i++)
        {
            if (stringInput[i] == ' ')
            {
                space++;
            }
        }
        if (space == 0)
        {
            sscanf(stringInput,"%d",&genre[0]);
            genre[0] = genre[0] - 1;
            genre[1] = genre[0];
            genre[2] = genre[0];
            bOK = genreVal(genre);
        }
        else if (space == 1)
        {
            sscanf(stringInput,"%d %d",&genre[0] ,&genre[1]);
            genre[0] = genre[0] - 1;
            genre[1] = genre[1] - 1;
            genre[2] = genre[1];
            bOK = genreVal(genre);
        }
        else if (space == 2)
        {
            sscanf(stringInput,"%d %d %d",&genre[0] ,&genre[1], &genre[2]);
            genre[0] = genre[0] - 1;
            genre[1] = genre[1] - 1;
            genre[2] = genre[2] - 1;
            bOK = genreVal(genre);
        }
    }  
    bOK = 1;	//ทั้งหมดถูก = 1
	
    strcpy(oneUser->username, username);
    strcpy(oneUser->password, password);
    strcpy(oneUser->email, email);
    oneUser->favGenre[0] = genre[0];
    oneUser->favGenre[1] = genre[1];
    oneUser->favGenre[2] = genre[2];
    oneUser->likeCount = 0;
    oneUser->followingCount = 0;

    hashTableInsert(1,oneUser->username,oneUser,&pCollision);

    return bOK;
}

/*****************************************************
  this function allow user to lock in to the program
 	-- return 0 there is an error in the login
	-- 1 the log in is successful without any error
*****************************************************/
int login()
{
    char username[128];
    char password[128];
    char stringInput[128];
    int bOK = 0;
    USER_T* nowUser = NULL;
    int option = 0;
    int bOK_REGISTER;
    int welcomeChoice;
    int loginval = 1;

    printf("\e[1;1H\e[2J");
	
    while(bOK != 1)
    {
        option = 0;
        printf("**********************************************\n");
        printf("              WELCOME TO LOGIN PAGE\n");
        printf("**********************************************\n");
        printf("Username : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",username);
        nowUser = hashTableLookup(1,username);
        printf("Password : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%s",password);
  
        if (nowUser == NULL)
        {
            printf("\tERROR, This username does not exist\n");
            loginval = 0;
        }
        else if (strcmp(password, nowUser->password) != 0)
        {
            printf("\tERROR, Wrong Password\n");
            loginval = 0;
        }
        else
        {
            loginval = 1;
        }
        if(loginval == 0)
        {
            printf("\nWhat do you want to do?\n");
            printf("[1] Login again\n");
            printf("[2] Create new account\n");
            printf("[3] Exit\n");
            while (option < 1 || option > 3)
            { 
                printf("\nEnter option : ");
                fgets(stringInput,sizeof(stringInput),stdin);
                sscanf(stringInput,"%d",&option);
                if (option < 1 || option > 3)
                {
                    printf("\tInvalid input, You must enter 1,2 or 3\n");
                }
                switch (option)
                {
                    case 1:
                    {
                        printf("\e[1;1H\e[2J");
                        break;
                    }
                    case 2:
                    {
                        bOK = 1;
                        registerAcc();
                        break;
                    }
                    case 3:
                    {
                        printf("\n    Goodbye \n");
                        exit (0);
                    }
			    }
            }
        }
        else 
        {
            currentUser = nowUser;
            bOK = 1;
        }
    }
    
    if(nowUser != NULL && strcmp(password, nowUser->password) == 0)
    {
        currentUser = nowUser;
        bOK = 1;
    }
	else
	{
        bOK = 0;
	}
	return bOK;
}

/********************************************************
  this function allow user to choose the option to do
  return 1 write review
         2 edit review
         3 view follow suggestion
         4 search review
         5 display follow review
				 6 display new feed
         7 logout
         8 exit program
********************************************************/
int menu()
{
	char stringInput[128];
	int menu = -1;

	while(menu < 0)
	{
        printf("\n**********************************************\n");
        printf("                Welcome! %s\n", currentUser->username);
        printf("**********************************************\n\n");
        printf("------ [1] WRITE NEW REVIEW             ------\n");
        printf("------ [2] EDIT A REVIEW                ------\n");
        printf("------ [3] VIEW USER SUGGESTION         ------\n");
        printf("------ [4] SEARCH FOR A REVIEW          ------\n");
        printf("------ [5] DISPLAY ALL FOLLOWING REVIEW ------\n");
        printf("------ [6] DISPLAY NEW FEED             ------\n");
        printf("------ [7] LOGOUT                       ------\n");
        printf("------ [8] EXIT PROGRAM                 ------\n");
        printf("\nSelect option : ");
        fgets(stringInput,sizeof(stringInput),stdin);
        sscanf(stringInput,"%d",&menu);
        if (menu > 8 || menu < 1)
        {
            printf("Please try again\n");
            menu = -1;
        }
	}
	return menu;
}

/********************************************************
  this function allow user to choose to do search option
	return 1 search review by Username
         2 search review by date
         3 search review by Movie Name
         4 search review by genre
         5 Exit to main menu
********************************************************/
int search_feature()
{
	char inputline[64];
	int search_choice = 0; 

	printf("\e[1;1H\e[2J");
	printf("\n**********************************************\n");
	printf("               SEARCH REVIEW\n");
	printf("**********************************************");
	while (1)
	{
        printf("\nWhat do you want to search by\n");
        printf("[1] Username\n");
        printf("[2] Date\n");
        printf("[3] Movie name\n");
        printf("[4] Genre\n");
        printf("[5] Exit to main menu\n");
        printf("Enter here : ");
        fgets(inputline,sizeof(inputline),stdin);
        sscanf(inputline,"%d",&search_choice);
        if ((search_choice < 1 || search_choice > 5))
        {
            printf("Please enter your choice again\n");
        }
        else
        {	
            break;
        }
	}
	return search_choice;
}

/*********************************************************
  this function will get input from the user to search
********************************************************/
void getInputSearch(int userChoice)
{
	char inputline[128];
	USER_T* usersearch = NULL;
	MOVIE_T* moviesearch = NULL;
	char usernameSearch[64];
	char movieNameSearch[64];
	char dateSearch[64];
	int genre;
	int returnValue = 1;
	char yn;
	char stringInput[128];
	int dateval = -1;
	int dateReview;
	int genreval = 0;

	if (userChoice == 1)
	{
        while(1)
        {
            printf("\nEnter username : ");
            fgets(inputline,sizeof(inputline),stdin);
            sscanf(inputline,"%s",usernameSearch);
            usersearch = hashTableLookup(1, usernameSearch);
            if (usersearch != NULL)
            {
                returnValue = searchUser(usernameSearch, currentUser->username);
                yn = 'n';	
                if (returnValue == 0)
                {
                    printf("\tThe user %s has no review\n",usernameSearch);
                    while(1)
                    {
                        printf("Want to try again? (Y/N) : ");
                        fgets(stringInput,sizeof(stringInput),stdin);
                        sscanf(stringInput, "%c", &yn);
                        if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
                        {
                            break;
                        }
                        else
                        {
                            printf("please type 'Y' or 'N' only\n");
                        }
                    }
                }
            }
            else
            {
                printf("\tThe user %s does not exist\n",usernameSearch);
                while(1)
                {
                    printf("Want to try again? (Y/N) : ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput, "%c", &yn);
                    if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
                    {
                        break;
                    }
                    else
                    {
                        printf("please type 'Y' or 'N' only\n");
                    }
                }
            }
            
            if (yn == 'n' || yn == 'N')
            {
                break;
            }
        }
	}
	else if (userChoice == 2)
	{
        while(dateval != 1)
        {
            printf("\nEnter date (dd/mm/yyyy) : ");
            fgets(inputline,sizeof(inputline),stdin);
            sscanf(inputline,"%s",dateSearch);
            dateval = checkReviewDay(dateSearch);
            if (dateval == 0)
            {
                while(1)
                {
                    printf("Want to try again? (Y/N) : ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput, "%c", &yn);
                    if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
                    {
                        break;
                    }
                    else
                    {
                        printf("please type 'Y' or 'N' only\n");
                    }
                }
            }
            if (yn == 'n' || yn == 'N')
            {
                break;
            }
            if (dateval == 1)
            {
                dateReview =  searchDate(dateSearch,currentUser->username);
                if (dateReview == 0)
                {
                    printf("    No review in this date\n");
                    dateval = -1;
                    while(1)
                    {
                        printf("Want to try again? (Y/N) : ");
                        fgets(stringInput,sizeof(stringInput),stdin);
                        sscanf(stringInput, "%c", &yn);
                        if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
                        {
                            break;
                        }
                        else
                        {
                            printf("    please type 'Y' or 'N' only\n");
                        }
                    }
                }
                else
                {
                    break;
                }
                if (yn == 'n' || yn == 'N')
                {
                    break;
                }
            }
        }
	}
	else if (userChoice == 3)
	{
        while(1)
        {
            printf("\nEnter moive name : ");
            fgets(inputline,sizeof(inputline),stdin);
            sscanf(inputline,"%[^\n]",movieNameSearch);
            moviesearch = hashTableLookup(2, movieNameSearch);
            if (moviesearch != NULL)
            {
                searchMovie(movieNameSearch, currentUser->username);
                yn = 'n';	
            }
            else
            {
                printf("This movie %s does not exist\n",movieNameSearch);
                while(1)
                {
                    printf("Want to try again? (Y/N) : ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput, "%c", &yn);
                    if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
                    {
                        break;
                    }
                    else
                    {
                        printf("please type 'Y' or 'N' only\n");
                    }
                }
            }
            if (yn == 'n' || yn == 'N')
            {
                break;
            }
        }
    }
    else if (userChoice == 4)
    {
        while(genreval != 1)
        {
            printf("\nWhich genre do you want to search?\n");
            printf("[1] Drama\n");
            printf("[2] Romance\n");
            printf("[3] Action\n");
            printf("[4] Comedy\n");
            printf("[5] Horror\n");
            printf("[6] Fantasy\n");
            printf("\nEnter genre : ");
            fgets(inputline,sizeof(inputline),stdin);
            sscanf(inputline,"%d",&genre);
            if (genre < 1 || genre > 6)
            {
                printf("Please input number 1-6 only\n");
                while(1)
                {
                    printf("Want to try again? (Y/N) : ");
                    fgets(stringInput,sizeof(stringInput),stdin);
                    sscanf(stringInput, "%c", &yn);
                    if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
                    {
                        break;
                    }
                    else
                    {
                        printf("please type 'Y' or 'N' only\n");
                    }
                }
            }
            else
            {
                genreval = 1;
            }
			
            if (yn == 'n' || yn == 'N')
            {
                break;
            }
        }
        searchGenre(genre-1, currentUser->username);
    }
}

/*********************************************************
  this function will display all user's 
  following account's review
********************************************************/
void displayFollowingReview()
{
    USER_T * followingName = NULL;
    char stringInput[128];
    char yn;
    int i = 1;
    char searchUsername[128];
    int result;
    int checkfollowingval;

    printf("\n**********************************************\n");
    printf("            FOllOWING USER'S REVIEW \n");
    printf("**********************************************");

    if (currentUser->followingCount == 0)
    {
        printf("\tYou has no follower\n\n");
    }
    else
    {
        printf("\nYou have %d following\n",currentUser->followingCount);
        printf("\nYour following list: \n");
        while (1)
        {
            followingName = listGetNext(currentUser->followingUser);
            if (followingName == NULL)
                break;
            printf("       [%d] %s\n",i ,followingName->username);
            i++;
        }
        listReset(currentUser->followingUser);
        while(1)
        {
            printf("Do you want to search review (Y/N) : ");
            fgets(stringInput,sizeof(stringInput),stdin);
            sscanf(stringInput,"%c",&yn);
            if(yn == 'y'|| yn == 'Y'|| yn == 'n'|| yn == 'N')
            {
                break;
            }
            else
            {
                printf("    please type 'Y' or 'N' only\n");
            }
        }
        if (yn == 'Y' || yn == 'y')
        {
            while(1)
            {
                printf("Enter username : ");
                fgets(stringInput,sizeof(stringInput),stdin);
                sscanf(stringInput,"%s",searchUsername);
                if(checkFollowingFind(currentUser, searchUsername) == 0)
                {
                    printf("    User %s is not in the following list\n",searchUsername);
                }
                else if(hashTableLookup(1, searchUsername) == NULL)
                {
                    printf("    The User %s does not exist\n",searchUsername);
                }
                else
                {
                    searchUser(searchUsername, currentUser->username);
                    break;
                }
            } 
        }  
        printf("\e[1;1H\e[2J");
    }
}

int main()
{
    readUser();
    readReview();
    readLikeUser();
    readFollowingUser();

    int welcomeChoice = 0;
    int bOK_LOGIN;
    int bOK_REGISTER;
    int menuChoice;
    int search_choice = 0;
    int writeValue;
    int editvalue = 0;
    printf("\e[1;1H\e[2J");
    welcomeChoice = welcome();
    while (welcomeChoice != -1)
    {
        switch(welcomeChoice)
		{
            case 1:
				bOK_LOGIN = login();
				if (bOK_LOGIN == 1)
				{	
				    printf("\e[1;1H\e[2J");
				    menuChoice = menu();
				    while (menuChoice != -1)
				    {
				        switch (menuChoice)
				        {
				            case 1:      //WRITE
				                writeValue = writeNewReview(currentUser->username);
				                if(writeValue == 1)
				                {
				                    printf("\e[1;1H\e[2J");
				                    menuChoice = menu();
				                }
				                else
				                {
				                    writeValue = writeNewReview(currentUser->username);
				                    printf("\e[1;1H\e[2J");
				                    menuChoice = menu();
				                }
				                break;
				            case 2:      //EDIT AND DELETE
				                printf("\e[1;1H\e[2J");
				                editvalue = editMovie(currentUser->username);
				                if(editvalue == 1)
				                {
				                    printf("\e[1;1H\e[2J");
				                    menuChoice = menu();
				                }
				                else
				                {
				                    menuChoice = menu();
				                }
								break;
				            case 3:      //LIST follow SUGGESTION
								printf("\e[1;1H\e[2J");
								displayFollowSuggest(currentUser->username);
								printf("\e[1;1H\e[2J");
								menuChoice = menu();
								break;
				            case 4:      //SEARCH
				                {
				                    search_choice = search_feature();
				                    if (search_choice == 5)
				                    {
				                        printf("\e[1;1H\e[2J");
				                        menuChoice = menu();
				                    }
				                    else
				                    {
				                        getInputSearch(search_choice);
				                        printf("\e[1;1H\e[2J");
				                        menuChoice = menu();
				                    }
				                    break;
				                }
				            case 5:      //DISPLAY FOLLOWING 
				            {
				                printf("\e[1;1H\e[2J");
				                displayFollowingReview();
				                menuChoice = menu();
				                break;
				            }
				            case 6:      //DISPLAY NEW FEED
				            {
				                printf("\e[1;1H\e[2J");
				                newFeed(currentUser->username);
				                menuChoice = menu();
				                break;
				            }
				            case 7:      //LOGOUT
				            {
				                dateToday(&currentUser->lastLoginDay,&currentUser->lastLoginMonth,&currentUser->lastLoginYear);
				                currentUser = NULL;
				                break;
				            }
				            case 8:      //EXIT
				            {
				                printf("\n    Goodbye %s\n",currentUser->username);
				                dateToday(&currentUser->lastLoginDay,&currentUser->lastLoginMonth,&currentUser->lastLoginYear);
				                writeUserHash();
				                writeReview();
				                hashTableFree(1);
				                hashTableFree(2);
				                exit(0);
				            }
				        }
				        if(menuChoice == 7)
				        {
				            break;
				        }
				    }
				}
				break;
			case 2:
				bOK_REGISTER = registerAcc(); //Register new accout
				break;
			case 3:
				printf("\n    Goodbye\n");
				writeUserHash();
				writeReview();
				hashTableFree(1);
				hashTableFree(2);
				exit(0);
		}
		printf("\e[1;1H\e[2J");
		welcomeChoice = welcome();
    }
}