#include "declares.h"

void writeHistory(char **short_history,HistoryList list, int countCommand)
{
	FILE *file;
	
	file = fopen(HISTORY_FILE,"wt");
	if(file == NULL)
	{
		printf("%s",ERROR_WRITE_TO_FILE);
		return;
	}
	
	for(int i = countCommand-1; i >= 0; i--)
		fprintf(file,"%s\n",short_history[i]);
	
	if(list.countCom != 0)
		recWrite(list.head,file);
	
	fclose(file);
}

void recWrite(HistoryNode *head, FILE *file)
{
	if(head == NULL)
		return;
	else
	{
		recWrite(head->next,file);
		fprintf(file,"%s\n",head->command);
	}
}

char *readString(FILE *file)
{
	int size = STRING_SIZE, i = 0;
	char *command = (char*)calloc(size,sizeof(char)), ch;
	
	ch = fgetc(file);
	
	while(ch != '\n')
	{
		command[i++] = ch;
		if(i == size)
			command = makeBiggerString(command,&size);
		ch = fgetc(file);
	}

	command[i++] = '\0';

	return (char*)realloc(command,i*sizeof(char));
}

void readHistory(char ***short_history,HistoryList *list, int *countCommand)
{
	FILE *file;
	int i = NUM_SHORT_HISTORY - 1, c;
	
	file = fopen(HISTORY_FILE,"r");
	if(file == NULL)
	{
		file = fopen(HISTORY_FILE,"wt");
		if(file == NULL)
			printf("%s",ERROR_WRITE_TO_FILE);
		return;
	}

	while (i >= 0 && (c = fgetc(file)) != EOF)
	{
		fseek(file, -1, SEEK_CUR);
		(*short_history)[i--] = readString(file);
	}
	
	if(i != -1)
		moveCommands(short_history,i+1);
	else
	{
		while ((c = fgetc(file)) != EOF)
		{
			fseek(file, -1, SEEK_CUR);
			insertNewCommandToHead(list, readString(file));
		}
		addCommandNumber(list);
	}
	
	*countCommand = (NUM_SHORT_HISTORY-1)-i;
	
	fclose(file);
}

void addCommandNumber(HistoryList *list)
{
	int i = 1;
	HistoryNode *curr = list->head;
	
	while(curr != NULL)
	{
		curr->numOfCom = i++;
		curr = curr->next;
	}
	
	list->countCom = i-1;	
}

void moveCommands(char ***short_history, int i)
{
	for(int k = 0; i < NUM_SHORT_HISTORY; k++, i++)
	{
		(*short_history)[k] = (*short_history)[i];
		(*short_history)[i] = NULL;
	}
}

void writeBinaryApt(AptList list)
{
	AptNode *curr = list.head;
	FILE *file;
	short int sizeOfCommand, convert;
	byte charArr[3];
	
	file = fopen(BINARY_FILE,"wb");
	if(file == NULL)
	{
		printf("%s",ERROR_WRITE_TO_FILE);
		return;
	}
	
	while(curr != NULL)
	{
		convert = curr->code;
		fwrite(&convert,sizeof(short int),ONE,file);
		sizeOfCommand = strlen(curr->address);
		fwrite(&sizeOfCommand,sizeof(short int),ONE,file);
		fwrite(curr->address,sizeof(char),sizeOfCommand,file);
		fwrite(&curr->price,sizeof(int),ONE,file);
		convertDateAndRooms(curr->enterDate,curr->rooms,charArr);
		fwrite(charArr,sizeof(byte),3,file);
		convertAddDate(curr->addDate,charArr);
		fwrite(charArr,sizeof(byte),2,file);
		curr = curr -> next;
	}
	
	return;
}

void convertDateAndRooms(AptDate date, unsigned short rooms, byte *charArr)
{
	byte convert1 = 0, convert2 = 0, convert3 = 0;
	
	convert1 = (rooms) << MOVE_FOUR;
	convert1 = (date.Day >> MOVE_ONE) | convert1;
	convert2 = (date.Day & MASK_ONE) << MOVE_SEVEN;
	convert2 = ((date.Month & MASK_FOUR) << MOVE_THREE) | convert2;
	convert2 = (date.Year-YEARBASE2K >> MOVE_FOUR) | convert2;
	convert3 = ((date.Year-YEARBASE2K & MASK_FOUR) << MOVE_FOUR) | convert3; 
	
	charArr[0] = convert1;
	charArr[1] = convert2;
	charArr[2] = convert3;	
}

void convertAddDate(AptDate date, byte *charArr)
{
	byte convert1 = 0, convert2 = 0;
	
	convert1 = date.Day << MOVE_THREE;
	convert1 = (date.Month >> MOVE_ONE) | convert1;
	convert2 = (date.Month & MASK_ONE) << MOVE_SEVEN;
	convert2 = (date.Year-YEARBASE2K) | convert2;
	
	charArr[0] = convert1;
	charArr[1] = convert2;
}

void readBinaryApt(AptList *list)
{
	FILE *file;
	long int endFile;
	short int sizeOfCommand, code;
	byte charArr[3];
	char *address;
	int price;
	AptDate enterDate, addDate;
	unsigned short rooms;
	
	file = fopen(BINARY_FILE,"rb");
	if(file == NULL)
	{
		file = fopen(BINARY_FILE,"wb");
		if(file == NULL)
			printf("%s",ERROR_WRITE_TO_FILE);
		return;
	}
	
	endFile = endOfFile(file);
	
	while(ftell(file) != endFile)
	{
		fread(&code,sizeof(short int),ONE,file);
		fread(&sizeOfCommand,sizeof(short int),ONE,file);
		address = (char*)calloc(sizeOfCommand+1,sizeof(char));
		fread(address,sizeof(char),sizeOfCommand,file);
		address[sizeOfCommand] = '\0';
		fread(&price,sizeof(int),ONE,file);
		fread(charArr,sizeof(byte),3,file);
		convertDateAndRoomsFromFile(&enterDate,&rooms,charArr);
		fread(charArr,sizeof(byte),2,file);
		convertDateFromFile(&addDate,charArr);
		createNewAptNodeFromFile(list,address,price,rooms,enterDate,addDate,code);
	}
	
	return;
}

void convertDateAndRoomsFromFile(AptDate *date, unsigned short *rooms, byte *charArr)
{
	*rooms = charArr[0] >> MOVE_FOUR;
	date->Day = (charArr[0] & MASK_FOUR << MOVE_ONE);
	date->Day += charArr[1] >> MOVE_SEVEN;
	date->Month = (charArr[1] & (MASK_FOUR << MOVE_THREE)) >> MOVE_THREE;
	date->Year = (charArr[1] & MASK_THREE) << MOVE_FOUR;
	date->Year = date->Year | (charArr[2] >> MOVE_FOUR);
	date->Year += YEARBASE2K;
}

void convertDateFromFile(AptDate *date, byte *charArr)
{
	date->Day = charArr[0] >> MOVE_THREE;
	date->Month = (charArr[0] & MASK_THREE) << MOVE_ONE;
	date->Month += charArr[1] >> MOVE_SEVEN;
	date->Year = charArr[1] & ~(MASK_ONE << MOVE_SEVEN);
	date->Year += YEARBASE2K;
}

long int endOfFile(FILE *fp)
{
	long int temp;
	
	fseek(fp,0,SEEK_END);
	temp = ftell(fp);
	rewind(fp);
	
	return temp;
}