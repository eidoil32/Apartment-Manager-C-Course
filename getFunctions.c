#include "declares.h"

void makeBiggerArray(AptNode ***results, int *size)
{
	*size *= DOUBLE_SIZE;

	*results = (AptNode**)realloc(*results, *size * sizeof(AptNode*));
}

void maxPriceFinder(AptNode *gallery, AptNode ***results, unsigned int num,int *size)
{
	AptNode *curr = gallery;
	int i = 0;

	if (*results == NULL)
	{
		*size = BASED_SIZE;
		(*results) = (AptNode**)calloc(*size,sizeof(AptNode*));
		while (curr != NULL)
		{
			if ((curr->price) <= num)
			{
				(*results)[i++] = curr;
				if (i == *size)
					makeBiggerArray(results, size);
			}
			curr = curr->next;
		}
		*size = i;
	}
	else
		for (i=0;i<*size; i++)
			if ((*results)[i] != NULL && (*results)[i]->price > num)
				(*results)[i] = NULL;
}

void minPriceFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size)
{
	AptNode* curr = gallery;
	int i = 0;

	if (*results == NULL)
	{
		*size = BASED_SIZE;
		(*results) = (AptNode**)calloc(*size,sizeof(AptNode*));
		while (curr != NULL)
		{
			if (curr->price >= num)
			{
				(*results)[i++] = curr;
				if (i == *size)
					makeBiggerArray(results, size);
			}
			curr = curr->next;
		}
		*size = i;
	}
	else
		for (i=0; i<*size; i++)
			if ((*results)[i] != NULL && (*results)[i]->price < num)
				(*results)[i] = NULL;
}

void maxRoomsFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size)
{
	AptNode* curr = gallery;
	int i = 0;

	if (*results == NULL)
	{
		*size = BASED_SIZE;
		(*results) = (AptNode**)calloc(*size,sizeof(AptNode*));
		while (curr != NULL)
		{
			if (curr->rooms <= num)
			{
				(*results)[i++] = curr;
				if (i == *size)
					makeBiggerArray(results, size);
			}
			curr = curr->next;
		}
		*size = i;
	}
	else
		for (i=0; i<*size; i++)
			if (((*results)[i] != NULL) && ((*results)[i]->rooms > num))
				(*results)[i] = NULL;
}

void minRoomsFinder(AptNode *gallery, AptNode ***results, unsigned int num, int *size)
{
	AptNode* curr = gallery;
	int i = 0;

	if (*results == NULL)
	{
		*size = BASED_SIZE;
		(*results) = (AptNode**)calloc(*size,sizeof(AptNode*));
		while (curr != NULL)
		{
			if (curr->rooms >= num)
			{
				(*results)[i++] = curr;
				if (i == *size)
					makeBiggerArray(results, size);
			}
			curr = curr->next;
		}
		*size = i;
	}
	else
		for (i=0; i<*size; i++)
			if (((*results)[i] != NULL) && ((*results)[i]->rooms < num))
				(*results)[i] = NULL;
}

int strToInteger(char *string, int size)
{
	int i, num = 0;
	
	for(i=0;i<size;i++)
		num =num*10 +(string[i]-'0');
	
	return num;
}

void dateFinder(AptNode *gallery,AptNode ***results, char *date, int *size)
{
	int day, month, year, i = 0;
	AptNode* curr = gallery;
	
	day = strToInteger(date,DMY_SIZE);
	month = strToInteger(date+DMY_SIZE,DMY_SIZE);
	year = strToInteger(date+(DMY_SIZE*DMY_SIZE),(DMY_SIZE*DMY_SIZE));
	
	if(*results == NULL)
	{
		*size = BASED_SIZE;
		(*results) = (AptNode**)calloc(*size,sizeof(AptNode*));
		while(curr != NULL)
		{
			if(compareDates(curr->enterDate,day,month,year,DATE_EARLY) == TRUE)
			{
						(*results)[i++] = curr;
						if (i == *size)
							makeBiggerArray(results, size);
			}
			curr = curr->next;
		}
	}
	else
		for (; i<*size; i++)
			if ((*results)[i] != NULL)
				if(compareDates((*results)[i]->enterDate,day,month,year,DATE_EARLY) == TRUE)
						(*results)[i] = NULL;	
}

int compareDates(AptDate compareTo, int day, int month, int year, int LOM)
{
	//LOM - less or more - if (until date) then 1 else -1
	if(LOM*compareTo.Year < LOM*year)
		return TRUE;
	else if(LOM*compareTo.Year == LOM*year)
	{
		if(LOM*compareTo.Month < LOM*month)
			return TRUE;
		else if(LOM*compareTo.Month == LOM*month)
		{
			if(LOM*compareTo.Day <= LOM*day)
				return TRUE;
		}
	}
	
	return FALSE;
}

void enterFinder(AptNode *gallery,int num, int *size)
{
	int day, month, year;
	AptNode* curr = gallery;
	AptDate currDate = currentDate();
	
	dateCalc(&currDate,num);
	
	day = currDate.Day;
	month = currDate.Month;
	year = currDate.Year;
	printf("%s%d.%d.%d: ",ALL_CODE_APT,day,month,year);	
	while(curr != NULL)
	{
		if(compareDates(curr->enterDate,day,month,year,DATE_AFTER) == TRUE)
		{
			if(curr->next == NULL)
			{
				printf("%d.\n",curr->code);
				break;
			}
			printf("%d ,",curr->code);

		}
		curr = curr->next;
	}
}

AptNode** callToSortNcut(AptNode** results,int *size, int sort)
{
	AptNode** newResults;

	newResults = cutAptArray(results,size);
	if(sort != NO_SORT)
		sortAptArray(&newResults,*size,sort);
	
	return newResults;
}

AptNode** cutAptArray(AptNode** array, int *size)
{
	AptNode **newArray = (AptNode**)calloc((*size),sizeof(AptNode*));
	int i, j;
	
	for(i = 0, j = 0; i < *size; i++)
		if( array[i] != NULL )
			newArray[j++] = array[i];
		
	*size = j;
	
	newArray = (AptNode**)realloc(newArray,j*sizeof(AptNode*));
	
	return newArray;
}

void swapApt(AptNode** a, AptNode** b)
{
	AptNode *temp = *a;
	
	*a = *b;
	*b = temp;
}

void sortAptArray(AptNode***gallery, int size, int sort)
{
	int upOrDown = isSort(sort);
	
	for (int i = 0; i < size-1; i++)
	{
		for (int j=0;j<size-1-i;j++)
		{
			if (upOrDown*((*gallery)[j]->price) > upOrDown*((*gallery)[j+1]->price))
				swapApt(&(*gallery)[j+1],&(*gallery)[j]);
		}
	}
}