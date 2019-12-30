#ifndef DECLARES

#define DECLARES

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable:4996)

//Debugging

//Commands
#define PROMT ">>"
#define NUM_SHORT_HISTORY 7
#define STRING_SIZE 20
#define DOUBLE_SIZE 2
#define COM_GET "get-an-apt"
#define COM_DELETE "delete-an-apt"
#define COM_ADD "add-an-apt"
#define COM_BUY "buy-an-apt"
#define COM_EXIT "exit"
#define YEARCONV 1900
#define YEARBASE2K 2000
#define MONCONV 1
#define HYP_SIGN "-"
#define SPACE_SIGN " "
#define BUY_SIZE 11
#define GET_SIZE 12
#define ADD_SIZE 11
#define DELETE_SIZE 15
#define FIRST 1
#define BASED_SIZE 5

//History Commands
#define LAST_COMMAND "!!"
#define LAST_COMMAND_PLACE 6
#define LAST_N_COMMAND '!'
#define SHORT_HISTORY "short_history"
#define ALL_HISTORY "history"
#define SIGN_HISTORY "^"

//Bool
#define TRUE 1
#define FALSE 0

//Get Commands
#define MAX_PRICE "MaximumPrice"
#define MIN_PRICE "MinimumPrice"
#define MAX_ROOMS "MaximumNumRooms"
#define MIN_ROOMS "MinimumNumRooms"
#define GET_DATE "Date"
#define GET_ENTER "Enter"
#define GET_SR "sr"
#define GET_S "s"
#define DMY_SIZE 2
#define NO_SORT -1
#define DATE_AFTER -1
#define DATE_EARLY 1
#define isSort(x) ((x==1)?(-1):1)
#define SWAP(type,a,b) {type temp = a; a = b; b = temp;}
#define ALL_CODE_APT "All apartments have been added until "
#define STR_TRUE 0
#define STR_FALSE 1

//Strings 
#define GOOD_BYE "Good Bye!\n"
#define COM_OPEN_TEXT "Please enter one of the following commands:\n"
#define COM_TYPE_TEXT "add-an-apt, get-an-apt, buy-an-apt or delete-an-apt\n"
#define RECOM_OPEN_TEXT "For reconstruction commands, please enter:\n"
#define RECOM_TYPE_TEXT "!!, !num, history, short_history or !num^str1^str2\n"
#define EXIT_TEXT "To exit, enter exit.\n"
#define OPENING_TEXT printf("%s%s%s%s%s",COM_OPEN_TEXT,COM_TYPE_TEXT,RECOM_OPEN_TEXT,RECOM_TYPE_TEXT,EXIT_TEXT);
#define INVAILD_COMMAND "You entered invaild command, Please try again.\n"	
#define TEXT_APT_DETAILS "Apt details:\n"
#define TEXT_APT_CODE "Code:"
#define TEXT_APT_ADDRESS "Address:"
#define TEXT_APT_ROOMS "Number of rooms:"
#define TEXT_APT_PRICE "Price:"
#define TEXT_APT_DATE "Entry date:"
#define TEXT_APT_DB_DATE "Database entry date:"
#define REALLOC_FAIL printf("Realloc Failed!\n")
#define APT_NOT_FOUND "The apartment is not in the database.\n"
#define COMMAND_ILLEGAL "Command is illegal!\n"
#define ERROR_WRITE_TO_FILE "Error! fail writing to file.\n"

//FILES
#define HISTORY_FILE "history.txt"
#define BINARY_FILE "binary.bin"
#define ONE 1
#define THREE 3

//MASKS
#define MASK_FOUR 0xF
#define MASK_THREE 0x7
#define MASK_ONE 0x1
#define MOVE_FOUR 4
#define MOVE_THREE 3
#define MOVE_ONE 1
#define MOVE_SEVEN 7

typedef unsigned char byte;

//Structures
typedef struct aptDate{
	short int Day;
	short int Month;
	short int Year;
} AptDate;

typedef struct aptNode{
	short int code;
	char *address;
	unsigned int price;
	unsigned short rooms;
	AptDate enterDate;
	AptDate addDate;
	struct aptNode *next;
	struct aptNode *prev;
} AptNode;

typedef struct aptList{
	AptNode *head;
	AptNode *tail;
} AptList;

typedef struct historyNode{
	char *command;
	int numOfCom;
	struct historyNode *next;
} HistoryNode;

typedef struct historyList{
	int countCom; //hold the number of commands in long-term history list
	HistoryNode *head;
	HistoryNode *tail;
} HistoryList;

//AptList Functions
void makeEmptyAptList(AptList *list);
void deleteAptNode(AptList *list, AptNode *node);
int isEmptyAptList(AptList *list);
void createNewAptNode(AptList *list,char *address,unsigned int price, unsigned short rooms, int day, int month, int year);
void createNewAptNodeFromFile(AptList *list,char *address,unsigned int price, unsigned short rooms, AptDate enterDate, AptDate addDate,short int code);
void freeAptList(AptList *list);

//Commands functions
void commandType(char *str, AptList *list, char ***commandArray, HistoryList *Hlist, int *count, int size);
void commandGet(char *command, AptNode *listnode, int size);
void commandDelete(char *command, AptList *list);
void commandAdd(char *command, AptList *list);
void commandBuy(int code, AptList *list);
char* getFullCommand(int *size);
char *makeBiggerString(char *string, int *size);
char *findCommand(int num, char ***commandArray, HistoryList *list);
char *switchCommands(char *command, char *commandOrignal);

//Date Calculate functions
AptDate currentDate();
void dateCalc(AptDate *date,int daysAgo);
void updateDate(AptDate *date,int day, int month, int year);

//Print functions
void printAptDetails(AptNode *apt);
void shortHistoryPrint(char **shortHistory, int lastNCommand);
void HistoryPrint(char **shortHistory, HistoryList list);

//DataBase Functions
void insertNewCommand(HistoryList *l, char *string);
void freeHistoryList(HistoryList *list);
int emptyHistoryList(HistoryList *l);
void makeEmptyHistoryList(HistoryList *l);
void printsResults(AptNode **results,int size);
void evacuation(char *command, char ***commandArray, HistoryList *list);
void saveCommand(char *command, char ***commandArray, HistoryList *list, int *count);
void freeShortHistory(char ***array);
void insertNewCommandToHead(HistoryList *l, char *string);

//Get functions
void minPriceFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size);
void maxPriceFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size);
void minRoomsFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size);
void maxRoomsFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size);
void dateFinder(AptNode *gallery,AptNode ***results, char *date,int *size);
int compareDates(AptDate compareTo, int day, int month, int year, int LOM);
void enterFinder(AptNode *gallery, int num,int *size);
void makeBiggerArray(AptNode ***results, int *size);
void sortAptArray(AptNode***gallery, int size, int sort);
void swapApt(AptNode **a, AptNode **b);
AptNode** cutAptArray(AptNode **array, int *size);
AptNode** callToSortNcut(AptNode **results,int *size, int sort);
int strToInteger(char *string, int size);

//Write and Read from files
void writeHistory(char **short_history,HistoryList list, int countCommand);
void recWrite(HistoryNode *head, FILE *file);
void readHistory(char ***short_history,HistoryList *list, int *countCommand);
char *readString(FILE *file);
void moveCommands(char ***short_history, int i);
void addCommandNumber(HistoryList *list);
void writeBinaryApt(AptList list);
void readBinaryApt(AptList *list);
void convertDateAndRooms(AptDate date, unsigned short rooms, byte *charArr);
void convertAddDate(AptDate date, byte *charArr);
void createNewAptNodeFromFile(AptList *list,char *address,unsigned int price, unsigned short rooms, AptDate enterDate, AptDate addDate, short int code);
void convertDateAndRoomsFromFile(AptDate *date, unsigned short *rooms, byte *charArr);
void convertDateFromFile(AptDate *date, byte *charArr);
long int endOfFile(FILE *fp);

#endif