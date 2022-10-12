/******************************************************
This is the main file.h to call for validation functions
Created by
Pechdanai Saepong (Nine) 63070503434 
Fasai Sae-Tae (Atom) 63070503436 
Chanon Khanijoh (Sean) 63070503408 
Pokpong jitjomnongmate (Paul) 63070503425
*******************************************************/

/****************************************************
 * this function is for validate genre.
 * return 1 if the input is correct
 *        0 if the input is error
 ****************************************************/
int genreVal(int genre[3]);

/****************************************************
 * this function is for validate username.
 * return 1 if the input is correct
 *        0 if the input is error
 ****************************************************/
int usernameVal(char name[128]);

/****************************************************
 * this function is for validate password.
 * return 1 if the input is correct
 *        0 if the input is error
 ****************************************************/
int passwordVal(char inputPassword[128]);

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

/******************************************* 
 * Sets the pointer for day, month and year
 * to the correct values for today.
 *******************************************/
void dateToday(int* pDay, int *pMonth, int* pYear);

/***************************************************
 * Compares two dates. Returns 1 if the 
 * first date is later than the second, -1 if the 
 * first date is earlier than the second, and 0 if
 * they are the same.
 * Arguments: day1   - day of first date
 *           month1 - month of first date
 *           year1  - year of first date
 *           day2   - day of second date
 *           month2 - month of second date
 *           year2  - year of second date
 **************************************************/
int dateCompare(int day1,int month1,int year1,int day2,int month2,int year2);

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
int checkEmail(char email[64]);

/********************************************************************
 * this function will check LTD of email that user input 
 * if ltd is FALSE this function will return value of ltd = 0 
 * but it's TRUE this function will return value of ltd = 1
 ********************************************************************/
int checkTLD(char email[64]);

/********************************************************************
 * this function will check number of @ sign in email that user input 
 * and return number of @ sign
 ********************************************************************/
int checkAT(char email[64]);

/***********************************************************
 * This function will check every character of string 
 * that user input if found character that not a digit
 * this function will return 0 but if not, it will return 1
 ***********************************************************/
int checkDigit(char input[]);

/*******************************************************
 * this function check delimiter, alphabetic, leap year
 * and range of day, month, year
 *******************************************************/
int checkReviewDay(char date[]);

/*****************************************************
 * this function check length of string date 
 * if day and month is 1-9 it must have 0 at front of
 * and length must be 10
 *****************************************************/
int checkDateLength(char date[]);

/******************************************************
 * this function will check date that user input 
 * if date is in the future this function will return 
 * result = 0 
 ******************************************************/
int checkFuture(int day1,int month1, int year1);