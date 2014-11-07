#pragma once
#include "IFactory.h"
#include "CiticsCompany.h"

class CiticsFactory : public IFactory
{
public:
	virtual ICompany *CreatCompany()
	{
		return new CiticsCompany();
	}

	CiticsFactory::CiticsFactory();

	CiticsFactory::~CiticsFactory();

	
};

