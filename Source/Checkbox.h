#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Button.h"

//-----------------------------------------------------------------
// Checkbox Class
//-----------------------------------------------------------------
class Checkbox: public Button
{
public:
	Checkbox();
	virtual ~Checkbox();

	bool GetChecked ();
	void SetChecked (bool bChecked);

protected:
	virtual HWND CreateHandle(HWND hWnd) override;

private:		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Checkbox(const Checkbox& bRef);
	Checkbox& operator=(const Checkbox& bRef);
};
