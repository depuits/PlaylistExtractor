#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------
class Button: public Control, public Callable
{
public:
	Button();
	virtual ~Button();

protected:
	virtual HWND CreateHandle(HWND hWnd) override;
	virtual LRESULT Procedure(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	// -------------------------
	// Datamembers
	// -------------------------
	bool m_Armed;
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Button(const Button& bRef);
	Button& operator=(const Button& bRef);
};
