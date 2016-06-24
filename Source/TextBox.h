#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// TextBox Class
//-----------------------------------------------------------------

class TextBox : public Control
{
public:
	TextBox();
	virtual ~TextBox();

	void SetPasswordChar(TCHAR const& txt);

protected:
	virtual HWND CreateHandle(HWND hWnd) override;
	virtual LRESULT Procedure(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	TextBox(const TextBox& tbRef);
	TextBox& operator=(const TextBox& tbRef);
};
