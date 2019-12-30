#include "declares.h"

void saveCommand(char *command, char ***commandArray, HistoryList *list, int *count)
{
	if((*count) == NUM_SHORT_HISTORY)
		evacuation(command,commandArray,list);
	else
	{
		(*commandArray)[(*count)] = command;
		(*count)++;
	}
}

void evacuation(char *command, char ***commandArray, HistoryList *list)
{
	int i;
	
	insertNewCommand(list,(*commandArray)[0]);
	free((*commandArray)[0]);
	
	for(i = 0; i < NUM_SHORT_HISTORY - 1; i++)
		(*commandArray)[i] = (*commandArray)[i+1];
	
	(*commandArray)[i] = command;
}

void insertNewCommand(HistoryList *l, char *string)
{
	HistoryNode *newNode = (HistoryNode*)calloc(1,sizeof(HistoryNode));
	l->countCom++;
	
	newNode->command = (char*)malloc((strlen(string)+1)*sizeof(char));
	strcpy(newNode->command,string);
	newNode->numOfCom = l->countCom;
	newNode->next = NULL;
	
	if(emptyHistoryList(l))
		l->head = l->tail = newNode;
	else
	{
		l->tail->next = newNode;
		l->tail = newNode;
	}
}

void insertNewCommandToHead(HistoryList *l, char *string)
{
	HistoryNode *newNode = (HistoryNode*)calloc(1,sizeof(HistoryNode));
	
	newNode->command = (char*)malloc((strlen(string)+1)*sizeof(char));
	strcpy(newNode->command,string);
	newNode->next = l->head;
	
	if(emptyHistoryList(l))
		l->head = l->tail = newNode;
	else
		l->head = newNode;
}

void makeEmptyHistoryList(HistoryList *l)
{
	l->head = l->tail = NULL;
	l->countCom = 0;
}

int emptyHistoryList(HistoryList *l)
{
	return (l->head == NULL);
}

void freeHistoryList(HistoryList *list)
{
	HistoryNode *curr = list->head, *temp;

	while(curr != NULL)
	{
		temp = curr->next;
		free(curr->command);
		free(curr);
		curr = temp;	
	}
}

void freeShortHistory(char ***array)
{
	for(int i = 0; i < NUM_SHORT_HISTORY; i++)
		free((*array)[i]);
	free(*array);
}