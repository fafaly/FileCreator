// FileCreator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "FileCreator.h"
#include "ICompany.h"
#include "CiticsCompany.h"
#include "IFactory.h"
#include "CiticsFactory.h"

int _tmain(int argc, _TCHAR* argv[])
{
	IFactory *factory = new CiticsFactory();
	ICompany *comp = factory->CreatCompany();
	strcpy(comp->fdate, "20140926");
	while (strcmp(comp->fdate, "20140925") != 0)
	{
		comp->GetDate(comp->fdate);
		comp->GetLastDate();
		//comp->GetTrade();
		//comp->GetPos();
		//comp->GetPNL();
		//comp->GetAccount();
		//comp->GetTcost();
		comp->CheckPos();
		comp->GetLastDate();
		strcpy(comp->fdate, comp->lastdate);
		comp->Clearvector();
		break;
	}
	printf("Executed successfully!\n");
	return 0;
}

