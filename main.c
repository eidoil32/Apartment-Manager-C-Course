#include "declares.h"

void main()
{
	char *command = NULL, **shortCommand = (char**)calloc(NUM_SHORT_HISTORY,sizeof(char*));
	int countCommand = 0, commandSize = 0;
	HistoryList list;
	AptList aptGallery;
	
	makeEmptyHistoryList(&list);
	makeEmptyAptList(&aptGallery);
	
	readHistory(&shortCommand,&list,&countCommand);
	readBinaryApt(&aptGallery);
	
	OPENING_TEXT
	
	command = getFullCommand(&commandSize);
	
	while(strcmp(command,COM_EXIT) != 0)
	{
		commandType(command,&aptGallery,&shortCommand,&list,&countCommand,commandSize);
		command = getFullCommand(&commandSize);
	}
	
	writeHistory(shortCommand,list,countCommand);
	writeBinaryApt(aptGallery);
	
	printf("%s",GOOD_BYE);
	
	freeShortHistory(&shortCommand);
	freeAptList(&aptGallery);
	freeHistoryList(&list);
}