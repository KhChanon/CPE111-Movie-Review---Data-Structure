/******************************************************
This is the main file to call for validation functions
Created by
Pechdanai Saepong (Nine) 63070503434 
Fasai Sae-Tae (Atom) 63070503436 
Chanon Khanijoh (Sean) 63070503408 
Pokpong jitjomnongmate (Paul) 63070503425
*******************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "validation.h"

#define TRUE 1
#define FALSE 0


/* Define a set of possible errors that can occur in checkDate */
#define  DATE_OK 0
#define  ERR_BADYEAR 1
  /* year must be between 1900 to 2100 inclusive */
#define ERR_BADMONTH  2
  /* month is something other than 1 to 12 */
#define ERR_BADDAY 3   
  /* day is outside of possible range for the month */
#define ERR_BADFORMAT 4 
  /* date string is not ten chars, or not all digits,*/
  /* or does not have  delimiters in the correct locations */

/* define a synonym for the int return value */
typedef int DATE_STATUS;

/* Number of days in each month. */
int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

/****************************************************
  this function is for validate genre.
  return 1 if the input is correct
         0 if the input is error
*****************************************************/
int genreVal(int genre[3]) 
{
    int status_genre = 0;
    for(int i = 0; i < 3; i++)
        {
        if (genre[i] >= 0 && genre[i] <= 5)
            {
            continue;
            }
        else
            {
            printf("\tERROR, can only type 1 to 6\n");
            return status_genre;    
            }
        }
    status_genre = 1;
    return status_genre;
}

/****************************************************
  this function is for validate username.
  return 1 if the input is correct
         0 if the input is error
*****************************************************/
int usernameVal(char name[128])
{
    int lengthName = 0;
    int status_username = 0;

    lengthName = strlen(name);
    if (lengthName < 6 || lengthName > 20)
        {
        printf("\tERROR, Invalid length (6 - 20)\n");
        }
    else 
        {    
        for (int i = 0; i < lengthName; ++i)
            {
            if (ispunct(name[i]))
                {
                printf("\tERROR, Special character are not allowed\n");
                break;
                }
            else 
                status_username = 1;
            }
        }

    return status_username;
}

/****************************************************
  this function is for validate password.
  return 1 if the input is correct
         0 if the input is error
*****************************************************/
int passwordVal(char inputPassword[128])
{
    int lengthPass = 0;
    int upper = 0;
    int lower = 0;
    int number = 0;
    int specialChar = 0;
    int password_status = 0;

    lengthPass = strlen(inputPassword);
    if (lengthPass < 7 || lengthPass > 13)
        {
        printf("\tError, Invalid length (8 - 12)\n");
        return 0;
        }
    for (int i = 0; i < lengthPass; ++i)
        {
        if (isupper(inputPassword[i]))
            {
            upper++;
            }
        if (islower(inputPassword[i]))
            {
            lower++;
            }
        if (isdigit(inputPassword[i]))
            {
            number++;
            }
        if (ispunct(inputPassword[i]))
            {
            if (inputPassword[i] == '?' || inputPassword[i] == '@' || inputPassword[i] == '$' || inputPassword[i] == '#')
                {
                specialChar++;
                }
            else
                {
                printf("\tERROR, Only '@', '?', '$' and '#' special character are allowed\n");
                break;
                }
            }
        }

    if (upper < 1)
        {
        printf("\tERROR, Must contain atleast 1 uppercase character\n");
        }
    else if (lower < 1)
        {
        printf("\tERROR, Must contain atleast 1 lowercase character\n");
        }
    else if (number < 2)
        {
        printf("\tERROR, Must contain atleast 2 digit character\n");
        }
    else if (specialChar < 1)
        {
        printf("\tERROR, Must contain atleast 1 special character\n");
        }
    else
        {
        password_status = 1;
        }
    return password_status;
}


/* Sets the pointer for day, month and year 
 * to the correct values for today. */
void dateToday(int* pDay, int *pMonth, int* pYear)
{
    time_t seconds = 0;    
    struct tm * pTimeStruct;

    seconds = time(&seconds);
    pTimeStruct = localtime(&seconds);
    *pDay = pTimeStruct->tm_mday;
    *pMonth = pTimeStruct->tm_mon + 1;
    *pYear = pTimeStruct->tm_year + 1900;
}


/* Compares two dates. Returns 1 if the 
 * first date is later than the second, -1 if the 
 * first date is earlier than the second, and 0 if
 * they are the same.
 * Arguments: day1   - day of first date
 *            month1 - month of first date
 *            year1  - year of first date
 *            day2   - day of second date
 *            month2 - month of second date
 *            year2  - year of second date
 */
int dateCompare(int day1,int month1,int year1,int day2,int month2,int year2)
{
    int compareValue = 0;
    if (year1 > year2)
        {
        compareValue = 1;
        }
    else if (year1 < year2)
        {
        compareValue = -1;
        } 
    else if (month1 > month2)
        {
        compareValue = 1;
        }
    else if (month1 < month2)
        {
        compareValue = -1;
        } 
    else if (day1 > day2)
        {
        compareValue = 1;
        }
    else if (day1 < day2)
        {
        compareValue = -1;
        } 
    /* otherwise they are the same */
    return compareValue;
}    
   

/*************************************************************************************
 * check email from user input.
 * Begin with an alphanumeric (letter or digit) character.
 * Exactly one occurrence of the at­sign (@).
 * At least one alphabetic or numeric character before the at­sign.
 * Address must end in one of: .com, .net, .ac.th, or .co.th
 * At least one alphanumeric character between the at­sign and the TLD.
 * No underscores after the at­sign.
 * At least one alphanumeric character between any periods that occur after the at­sign.
 * and print result.
 *************************************************************************************/
int checkEmail(char email_input[])
{
    char stringInput[128];
    int atNum = 0;
    char emailFront[128];
    char emailBack[128];
    int atlocation;
    int punctVal = TRUE;
    int frontAlpha = FALSE;
    int alphanumtld = FALSE;
    int noUScore = TRUE;
    int tldval = FALSE;
    int emailval  = FALSE;
    int dotNoStick = TRUE;
    int result = 0;
    
    for (int i = 0; i < strlen(email_input); i++)
        {
        if (ispunct(email_input[i]) && email_input[i] != '@' && email_input[i] != '_' && email_input[i] != '-' && email_input[i] != '.')
            {
            punctVal = FALSE;
            }    
        }
    if (isalnum(email_input[0]))
        {
        frontAlpha = TRUE;
        }
    for (int i = 0; i < strlen(email_input); i++)
        {
        if(email_input[i] == '@')
            {
            atNum++;
            atlocation = i;
            }
        }
    if (punctVal == TRUE )
        {
        if(atNum != 1)
            {
            printf("\tERROR, Only 1 @ allowed\n");
            }
        else
            {
            if(frontAlpha == TRUE)
                {
                if(isalnum(email_input[atlocation-1]) && isalnum(email_input[atlocation+1]))
                    {
                    email_input[atlocation] = 32;
                    sscanf(email_input, "%s %s", emailFront, emailBack);
                    for(int i = 0; i < strlen(emailBack); i++)
                        {
                        if(emailBack[i] == '_' || emailBack[i] == '-')
                            {
                            noUScore = FALSE;
                            }
                        }
                    for(int i = 0; i < strlen(emailBack); i++)
                        {
                        if(emailBack[i] == '.' && emailBack[i+1] == '.')
                            {
                            dotNoStick = FALSE;
                            }
                        }
                    int i = strlen(emailBack);
                    if(emailBack[i-1] == 'm' && emailBack[i-2] == 'o' && emailBack[i-3] == 'c' && emailBack[i-4] == '.')
                        {
                        tldval = TRUE;
                        }
                    else if(emailBack[i-1] == 't' && emailBack[i-2] == 'e' && emailBack[i-3] == 'n' && emailBack[i-4] == '.')
                        {
                        tldval = TRUE;
                        }
                    else if(emailBack[i-1] == 'h' && emailBack[i-2] == 't' && emailBack[i-3] == '.' && emailBack[i-4] == 'c' && emailBack[i-5] == 'a' && emailBack[i-6] == '.')
                        {
                        tldval = TRUE;
                        }
                    else if(emailBack[i-1] == 'h' && emailBack[i-2] == 't' && emailBack[i-3] == '.' && emailBack[i-4] == 'o' && emailBack[i-5] == 'c' && emailBack[i-6] == '.')
                        {
                        tldval = TRUE;
                        }
                    if(tldval == TRUE)
                        {
                        if(isalnum(emailBack[strlen(emailBack-4)]) || isalnum(emailBack[strlen(emailBack-6)]))
                            {
                            alphanumtld = TRUE;
                            if(noUScore == FALSE)
                                {
                                printf("\tERROR, There cannot be '_' or '-' after '@'\n");
                                }
                            else if( dotNoStick == FALSE)
                                {
                                printf("\tERROR, Cannot have '.' next to each other\n");
                                }
                            else if(noUScore == TRUE && tldval == TRUE && frontAlpha == TRUE && punctVal == TRUE && atNum  == 1 && alphanumtld == TRUE && dotNoStick == TRUE)
                                {
                                email_input[atlocation] = '@';
                                emailval = TRUE;
                                result = 1;
                                }
                            }
                        else 
                            {
                            printf("\tERROR, Should have alphabet or number before TLD \n");
                            }
                        }
                    else
                        {
                        printf("\tERROR, Should end with '.com', '.net', '.ac.th' or '.co.th' only\n");
                        }
                    }
                else
                    {
                    printf("\tERROR, Should have alphabet or number before and after '@' \n");
                    }
                }
            else
                {
                printf("\tERROR, Should start with alphanumeric\n");
                }
            }
        }
    else
        {
        printf("\tERROR, Only '@', '-', '_', '.' special character allowed\n");
        }
    return result;
}

/********************************************************************
 * this function will check number of @ sign in email that user input 
 * and return number of @ sign
 ********************************************************************/
int checkAT(char email[64])
{
    int checkAT = 0;
    int i;
    for (i=0;i<strlen(email);i++)
        {
        if(email[i]=='@')
            {
            checkAT++;
            }
        }
    return checkAT;
}

/********************************************************************
 * this function will check TLD of email that user input 
 * if TLD is FALSE this function will return value of TLD = 0 
 * but it's TRUE this function will return value of TLD = 1
 ********************************************************************/
int checkTLD(char email[64])
{
    int tld = 1;
    if ((email[strlen(email)-3] != 'c' || email[strlen(email)-2] != 'o' || email[strlen(email)-1] != 'm') &&
        (email[strlen(email)-3] != 'n' || email[strlen(email)-2] != 'e' || email[strlen(email)-1] != 't') &&
        (email[strlen(email)-5] != 'a' || email[strlen(email)-4] != 'c' || email[strlen(email)-3] != '.' || email[strlen(email)-2] != 't' || 
         email[strlen(email)-1] != 'h') &&
        (email[strlen(email)-5] != 'c' || email[strlen(email)-4] != 'o' || email[strlen(email)-3] != '.' || email[strlen(email)-2] != 't' || 
         email[strlen(email)-1] != 'h'))
        {
        tld = 0;
        }
    return tld;
}

/***********************************************************
 * This function will check every character of string 
 * that user input if found character that not a digit
 * this function will return 0 but if not, it will return 1
 ***********************************************************/
int checkDigit(char input[])
{
    int returnval = 1;
    
    for(int i = 0; i < strlen(input); i++)
        {
        if ((!isdigit(input[i]) && input[i] != '.') || isspace(input[i]))
            {
            returnval = 0;
            break;
            }
        }
    return returnval;
}

/******************************************************
 * this function will check date that user input 
 * if date is in the future this function will return 
 * result = 0 
 ******************************************************/
int checkFuture(int day1,int month1, int year1)
{
    int day2;
    int month2;
    int year2;
    int difference;
    int result = 1;
    dateToday(&day2,&month2,&year2);
    difference = dateCompare(day1,month1,year1,day2,month2,year2);
    if (difference == 1)
        {
        printf("        %d/%d/%d is in the future\n",day1,month1,year1);
        result = 0;
        }
    return result;
}

/*****************************************************
 * this function check length of string date 
 * if day and month is 1-9 it must have 0 at front of
 * and length must be 10
 *****************************************************/
int checkDateLength(char date[])
{
    int result = 1;
    if (strlen(date) != 10)
        {
        printf("   Input is too long or too short\n");
        result = 0;
        }
    else if ((isdigit(date[0])!=0 && isdigit(date[1])==0) || (isdigit(date[3])!=0 && isdigit(date[4])==0) || (isdigit(date[0])!=0 && isdigit(date[1]==0 && isdigit(date[2])!=0) && isdigit(date[3])==0))
        {
        printf("    missing leading zero\n");
        result = 0;
        }
    return result;
}

/*******************************************************
 * this function check delimiter, alphabetic, leap year
 * and range of day, month, year
 *******************************************************/
int checkReviewDay(char date[])
{
    int day_status = 0;
    int day;
    int month;
    int year;
    int result2;
    result2 = checkDateLength(date);
    if (result2 != 0)
        {
        if (strlen(date) == 10)
        {
            if (date[2] != '/' || date[5] != '/')
                {
                printf("    wrong delimiter, should be /\n");
                }
            else if (isdigit(date[0]) == 0 || isdigit(date[1]) == 0 || isdigit(date[3]) == 0 || isdigit(date[4]) == 0 || isdigit(date[6]) == 0 || isdigit(date[7]) == 0 || isdigit(date[8]) == 0 || isdigit(date[9]) == 0 )
                {
                printf("    alphabetic\n");
                }
            else
                {
                day = atoi(&date[0]);
                month = atoi(&date[3]);
                year = atoi(&date[6]);
                if ((day <= 0 || day > 31) || (month <= 0 || month > 12))
                    {
                    printf("    Not a date\n");
                    }
                else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day == 31))
                    {
                    printf("    illegal day of month\n");
                    }
                else if (year%4 != 0 && month == 2 && day >= 29)
                    {
                    printf("    not a leap year\n");
                    }
                else if (checkFuture(day,month,year) == 1)
                    {
                    day_status = 1;
                    }
                }
            }
        }
    return day_status;
}