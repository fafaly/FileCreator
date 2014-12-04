// FileCreator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "FileCreator.h"
#include "ICompany.h"
#include "CiticsCompany.h"
#include "IFactory.h"
#include "CiticsFactory.h"
#include <string.h>
#include <time.h>

/*Check weekend and time */
int CheckTime()
{
	return 2;
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	if (timeinfo->tm_wday == 0 || timeinfo->tm_wday == 6)
	{
		return false;
	}
	else if (timeinfo->tm_hour < 8)
	{
		return 2;//run the tcost
	}
	else
	{
		return 1;//run normal program
	}
}

void Run(ICompany *comp)
{
	int type = CheckTime() ;
	comp->GetDate(comp->fdate);
	if (type==1)
	{
		printf("Start run ...\n");
		while (strcmp(comp->fdate, "xxxx") != 0)
		{

			comp->GetTrade();
			comp->GetPos();
			comp->CheckPos();
			comp->GetPNL();
			comp->GetLastDate();
			strcpy(comp->fdate, comp->lastdate);
			break;
		}
	}
	else if (type == 2)
	{
		comp->GetLastDate();
		//comp->GetAccount();
 		comp->GetTcost();
	}
	else
	{
		exit(1);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	IFactory *factory = new CiticsFactory();
	ICompany *comp = factory->CreatCompany(1);
	//strcpy(comp->fdate, "20141202");
	Run(comp);
	comp = factory->CreatCompany(2);
	//strcpy(comp->fdate, "20141119");
	Run(comp);
	printf("Executed successfully!\n");
	_gettch();
	return 0;
}

