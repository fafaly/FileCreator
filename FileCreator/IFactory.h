#pragma once
#include "ICompany.h"
class IFactory
{
public:
	virtual ICompany *CreatCompany() = 0;

};

