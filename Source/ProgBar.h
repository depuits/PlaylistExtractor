#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// ProgressBar Class
//-----------------------------------------------------------------
class ProgBar : public Control
{
public:
	ProgBar();
	virtual ~ProgBar();
	
	void SetRange(int r);
	void SetPos(int p);
	void AddToPos(int a);

	void SetStep(int s);
	void AddStep();

protected:
	virtual HWND CreateHandle(HWND hWnd) override;

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ProgBar(const ProgBar& bRef);
	ProgBar& operator=(const ProgBar& bRef);
};
