#include "declares.h"

char* getFullCommand(int *size)
{
	int i = 0;
	char *string = (char*)malloc(STRING_SIZE*sizeof(char)), ch = '\n';
	
	*size = STRING_SIZE;
	
	while(ch == '\n')
	{
		printf("%s ",PROMT);
		ch = getchar();
	}
	
	while(ch != '\n')
	{
		string[i++] = ch;
		if(i == (*size))
			string = makeBiggerString(string,size);
		ch = getchar();
	}
	
	string[i++] = '\0';
	*size = i;

	return (char*)realloc(string,i*sizeof(char));
}

char *makeBiggerString(char *string, int *size)
{
	(*size) *= DOUBLE_SIZE;
	
	return (char*)realloc(string,(*size)*sizeof(char));;
}

void commandType(char *str, AptList *list, char ***commandArray, HistoryList *Hlist, int *count, int size)
{
	char *temp, *f_command, *n_pointer, *strForTok = (char*)malloc(size*sizeof(char));
	
	strcpy(strForTok,str);
	
	if (str[0] != LAST_N_COMMAND && strcmp(str,SHORT_HISTORY) != STR_TRUE && strcmp(str,ALL_HISTORY) != STR_TRUE)
	{
		temp = strtok(strForTok, SPACE_SIGN);
		
		if(strcmp(temp,COM_GET) == STR_TRUE)
			commandGet(str+GET_SIZE,list->head,size-GET_SIZE);
		else if(strcmp(temp,COM_DELETE) == STR_TRUE)
			commandDelete(str+DELETE_SIZE,list);
		else if(strcmp(temp,COM_ADD) == STR_TRUE)
			commandAdd(str+ADD_SIZE,list);
		else if(strcmp(temp,COM_BUY) == STR_TRUE)
			commandBuy(atoi(str+BUY_SIZE),list);
		else
		{
			printf("%s",COMMAND_ILLEGAL);
			return;
		}
		
		saveCommand(str,commandArray,Hlist,count);
	}
	else
	{
		if(*count == 0)
			printf("No commands to execute!\n");
		else
		{
			temp = strtok(strForTok,SIGN_HISTORY);
			if (strcmp(str, LAST_COMMAND) == STR_TRUE && (*count) > 0)
			{
					size = strlen(((*commandArray)[*count - 1])) + 1;
					f_command = (char*)calloc(size, sizeof(char));
					strcpy(f_command, (*commandArray)[*count - 1]);
					commandType(f_command, list, commandArray, Hlist, count, size);
			}
			else if(strcmp(str,SHORT_HISTORY) == STR_TRUE)
				shortHistoryPrint(*commandArray,Hlist->countCom);
			else if(strcmp(str,ALL_HISTORY) == STR_TRUE)
				HistoryPrint(*commandArray,*Hlist);
			else if(str[0] == LAST_N_COMMAND && strcmp(temp,str) == STR_TRUE)
			{
				f_command = findCommand(atoi(temp+1),commandArray,Hlist);
				size = strlen(f_command) + 1;
				n_pointer = (char*)calloc(size, sizeof(char));
				strcpy(n_pointer,f_command);
				commandType(n_pointer,list,commandArray,Hlist,count,size);
			}
			else if(strcmp(temp,str) != STR_TRUE)
			{
				f_command = switchCommands(str+(strlen(temp)+1),findCommand(atoi(temp+1),commandArray,Hlist));
				size = strlen(f_command) + 1;
				n_pointer = (char*)calloc(size, sizeof(char));
				strcpy(n_pointer, f_command);
				commandType(n_pointer,list,commandArray,Hlist,count,size);
			}
			else
				printf("%s",INVAILD_COMMAND);
		}
	}
	
	free(strForTok);
}

char *findCommand(int num, char ***commandArray, HistoryList *list)
{
	HistoryNode *curr = list->head;
	
	if(list->countCom < num)
	{
		num = num-list->countCom;
		return (*commandArray)[num-1];
	}
	else
	{
		while(curr != NULL)
		{
			if(curr->numOfCom == num)
				return curr->command;
			curr = curr->next;
		}
	}
	
	return NULL;
}

char *switchCommands(char *command, char *commandOrignal)
{
	char *newCommand, *switchFrom, *switchTo, *pointer, *temp = commandOrignal;
	int size, sizeFrom, sizeTo;

	switchFrom = strtok(command, SIGN_HISTORY);
	switchTo = strtok(NULL, SIGN_HISTORY);

	sizeFrom = strlen(switchFrom);
	sizeTo = strlen(switchTo);
	size = strlen(commandOrignal);

	newCommand = (char*)calloc((size + sizeTo - sizeFrom + 1),sizeof(char));
	pointer = strstr(commandOrignal, switchFrom);
	strncat(newCommand, temp, pointer - commandOrignal);

	while (pointer != NULL)
	{
		strcat(newCommand, switchTo);
		newCommand = (char*)realloc(newCommand, (size + (sizeTo-sizeFrom)) * sizeof(char));
		temp = pointer;
		pointer = strstr(pointer + sizeFrom, switchFrom);
		if(pointer != NULL)
			strncat(newCommand, temp + sizeFrom, pointer - (temp + sizeFrom));
	}
	strncat(newCommand, temp + sizeFrom, size - (temp - newCommand));
	
	return newCommand;
}

void commandAdd(char *command, AptList *list)
{
	char *address, *tempTok = (char*)malloc((strlen(command)+1)*sizeof(char));
	
	int price, day, month, year, rooms;

	strcpy(tempTok,command);
	address = strtok(tempTok,"\"");
	price = atoi(strtok(NULL,SPACE_SIGN));
	rooms = atoi(strtok(NULL, SPACE_SIGN));
	day = atoi(strtok(NULL, SPACE_SIGN));
	month = atoi(strtok(NULL, SPACE_SIGN));
	year = atoi(strtok(NULL, SPACE_SIGN));
	createNewAptNode(list,address,price,rooms,day,month,year);
	
	free(tempTok);
}

void commandGet(char *command, AptNode *listnode, int size)
{	
	char *temp, *num, *comTok = (char*)malloc((strlen(command)+1)*sizeof(char));
	int resultsSize = 0;
	AptNode **results = NULL;
	
	strcpy(comTok,command);

	temp = strtok(comTok,SPACE_SIGN);
	num = strtok(NULL,SPACE_SIGN);
	while(temp != NULL && (strcmp(temp,GET_S) != 0) && (strcmp(temp,GET_SR) != 0))
	{
		if(strcmp(temp,MAX_PRICE) == STR_TRUE)
			maxPriceFinder(listnode,&results,atoi(num),&resultsSize);
		else if(strcmp(temp,MIN_PRICE) == STR_TRUE)
			minPriceFinder(listnode,&results,atoi(num),&resultsSize);
		else if(strcmp(temp,MAX_ROOMS) == STR_TRUE)
			maxRoomsFinder(listnode,&results,atoi(num),&resultsSize);
		else if(strcmp(temp,MIN_ROOMS) == STR_TRUE)
			minRoomsFinder(listnode,&results,atoi(num),&resultsSize);
		else if(strcmp(temp,GET_DATE) == STR_TRUE)
			dateFinder(listnode,&results,num,&resultsSize);
		else if(strcmp(temp,GET_ENTER) == STR_TRUE)
			enterFinder(listnode,atoi(num),&resultsSize);
		temp = strtok(NULL,SPACE_SIGN);
		
		if (temp != NULL && temp[0] == '-')
			temp++;
		
		num = strtok(NULL,SPACE_SIGN);
	}
	if(temp != NULL)
		results = callToSortNcut(results,&resultsSize,strcmp(temp,GET_S));
	else //No need to sort
		results = callToSortNcut(results,&resultsSize,NO_SORT);	
	
	printsResults(results,resultsSize);
	
	//Free Memory
	free(results);
	free(comTok);
}

void commandDelete(char *command, AptList *list)
{
	int daysAgo;
	AptDate untilMe = currentDate();
	AptNode *temp, *curr = list->head;
	
	sscanf(command,"Enter %d",&daysAgo);
	dateCalc(&untilMe,daysAgo);
	
	while(curr != NULL)
	{
		if(compareDates(curr->addDate,untilMe.Day,untilMe.Month,untilMe.Year,DATE_AFTER) == TRUE)
		{
			temp = curr->next;
			deleteAptNode(list,curr);
			curr = temp;
		}
		else
			curr = curr->next;
	}
}

void commandBuy(int code, AptList *gallerylist)
{
	AptNode *curr = gallerylist->head;
	while (curr != NULL)
	{
		if (curr->code == code)
		{
			deleteAptNode(gallerylist, curr);
			return;
        }
		curr = curr->next;
	}
	printf("%s",APT_NOT_FOUND);
}

void deleteAptNode(AptList *list, AptNode *node)
{
	if (list->head == list->tail && list->head == node)
		list->head = list->tail = NULL;
	else if (list->tail == node)
	{
		list->tail = node->prev;
		node->prev->next = NULL;
	}
	else if (list->head == node)
	{
		node->next->prev = NULL;
		list->head = node->next;
	}
	else
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
	}
	free(node->address);
	free(node);
}

void freeAptList(AptList *list)
{
	AptNode *curr = list->head, *temp;
	
	while(curr != NULL)
	{
		temp = curr->next;
		free(curr->address);
		free(curr);
		curr = temp;	
	}
}

int isEmptyAptList(AptList *list)
{
	return (list->head == NULL);
}

void makeEmptyAptList(AptList *list)
{
	list->head = list->tail = NULL;
}

void createNewAptNode(AptList *list,char *address,unsigned int price, unsigned short rooms, int day, int month, int year)
{
	AptNode *newNode = (AptNode*)malloc(sizeof(AptNode));
	
	newNode->next = NULL;
	newNode->price = price;
	newNode->rooms = rooms;	
	newNode->address = (char*)malloc((strlen(address)+1)*sizeof(char));
	strcpy(newNode->address,address);
	newNode->enterDate.Day = day;
	newNode->enterDate.Month = month;
	newNode->enterDate.Year = year + YEARBASE2K;
	newNode->addDate = currentDate();
	
	if(isEmptyAptList(list))
	{
		list->head = list->tail = newNode;
		newNode->code = FIRST;
	}
	else
	{
		newNode->code = list->tail->code + FIRST;
		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;
	}
}

void createNewAptNodeFromFile(AptList *list,char *address,unsigned int price, unsigned short rooms, AptDate enterDate, AptDate addDate, short int code)
{
	AptNode *newNode = (AptNode*)malloc(sizeof(AptNode));
	
	newNode->next = NULL;
	newNode->price = price;
	newNode->rooms = rooms;	
	newNode->address = (char*)malloc((strlen(address)+1)*sizeof(char));
	strcpy(newNode->address,address);
	newNode->enterDate = enterDate;
	newNode->addDate = addDate;
	newNode->code = code;
	
	if(isEmptyAptList(list))
		list->head = list->tail = newNode;
	else
	{
		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;
	}
}