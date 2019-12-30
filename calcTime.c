#include "declares.h"

AptDate currentDate()
{
	AptDate date;
	
	struct tm tm = *localtime(&(time_t){time(NULL)}); //get local time
	
	//Update new date into "date" structure
	updateDate(&date,tm.tm_mday,tm.tm_mon,tm.tm_year);
	
	return date;
}

void dateCalc(AptDate *date,int daysAgo)
{
    struct tm tm = *localtime(&(time_t){time(NULL)}); //get local time
	
	//Enter date from "date" structure to time "tm" structure
	tm.tm_mday = date->Day;
	tm.tm_mon = date->Month-MONCONV;
	tm.tm_year = (date->Year)-YEARCONV;
	
	//Decrease "daysAgo" from the date
    tm.tm_mday -= daysAgo;
	
	//Recalculate time
	mktime(&tm);
	
	//Update new date into "date" structure
	updateDate(date,tm.tm_mday,tm.tm_mon,tm.tm_year);
}

void updateDate(AptDate *date,int day, int month, int year)
{
	date->Day = day;
	date->Month = month+MONCONV;
	date->Year = year+YEARCONV;
}