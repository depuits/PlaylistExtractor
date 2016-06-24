#pragma once

#include "CallerPtr.h"

class Callable
{
public:
	Callable(): m_pCaller(0)
	{}
	virtual ~Callable(){}
	
	void SetActionListener(CallerPtr* caller)
	{
		m_pCaller = caller;
	}

protected:
	CallerPtr* m_pCaller;
};
