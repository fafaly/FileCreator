#pragma once
#include "IFactory.h"
#include "CiticsCompany.h"

class CiticsFactory : public IFactory
{
public:
	virtual ICompany *CreatCompany(int pro_no)
	{
		return new CiticsCompany(pro_no);
	}

	CiticsFactory::CiticsFactory();

	CiticsFactory::~CiticsFactory();

	
};

