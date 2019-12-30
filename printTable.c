#include "declares.h"

void printsResults(AptNode **results,int size)
{
	for(int i = 0; i < size; i++)
		printAptDetails(results[i]);
}

void printAptDetails(AptNode *apt) 
{
	printf("%s",TEXT_APT_DETAILS);
	printf("%s %d\n",TEXT_APT_CODE,apt->code);
	printf("%s %s\n",TEXT_APT_ADDRESS,apt->address);
	printf("%s %d\n",TEXT_APT_ROOMS,apt->rooms);
	printf("%s %d\n",TEXT_APT_PRICE,apt->price);
	printf("%s %d.%d.%d\n",TEXT_APT_DATE,apt->enterDate.Day,apt->enterDate.Month,apt->enterDate.Year);
	printf("%s %d.%d.%d\n",TEXT_APT_DB_DATE,apt->addDate.Day,apt->addDate.Month,apt->addDate.Year);
}

void shortHistoryPrint(char **shortHistory, int lastNCommand)
{
	for(int i = 1; i <= NUM_SHORT_HISTORY && (shortHistory[i-1] != NULL); i++)
		printf("%d: %s\n",i+lastNCommand,shortHistory[i-1]);
}

void HistoryPrint(char **shortHistory, HistoryList list)
{
	HistoryNode *curr = list.head;
	
	while(curr != NULL)
	{
		printf("%d: %s\n",curr->numOfCom,curr->command);
		curr = curr->next;
	}
	
	shortHistoryPrint(shortHistory,list.countCom);
}

