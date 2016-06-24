#pragma once

class Callable;

class CallerPtr
{
public:
	CallerPtr(){}
	virtual ~CallerPtr(){}

	virtual void CallAction(Callable* caller) = 0;
};
