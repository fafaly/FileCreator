// FileCreator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "FileCreator.h"
#include "ICompany.h"
#include "CiticsCompany.h"
#include "IFactory.h"
#include "CiticsFactory.h"
#include <string.h>

void Run(ICompany *comp)
{
	while (strcmp(comp->fdate, "xxxx") != 0)
	{
		comp->GetDate(comp->fdate);
		comp->GetLastDate();
		comp->GetTrade();
		comp->GetPos();
		comp->CheckPos();
		comp->GetPNL();
		comp->GetAccount();
		comp->GetTcost();
		comp->GetLastDate();
		strcpy(comp->fdate, comp->lastdate);
		comp->Clearvector();
		break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	IFactory *factory = new CiticsFactory();
	ICompany *comp = factory->CreatCompany(1);
	//strcpy(comp->fdate, "20141114");
	Run(comp);
	//comp = factory->CreatCompany(2);
	//strcpy(comp->fdate, "20141111");
	//Run(comp);
	printf("Executed successfully!\n");
	return 0;
}

