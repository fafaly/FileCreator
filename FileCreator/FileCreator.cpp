// FileCreator.cpp : �������̨Ӧ�ó������ڵ㡣
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
	strcpy(comp->fdate, "20141107");
	while (strcmp(comp->fdate, "20140925") != 0)
	{
		comp->GetDate(comp->fdate);
		comp->GetLastDate();
		//GetTrade();
		//GetPos();
		comp->GetPNL();
		comp->GetAccount();
		//comp->GetTcost();
		comp->GetLastDate();
		strcpy(comp->fdate, comp->lastdate);
		comp->Clearvector();
		break;
	}
	printf("Executed successfully!\n");
	return 0;
}

