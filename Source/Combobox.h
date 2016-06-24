#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "ListBox.h"

//-----------------------------------------------------------------
// Combobox Class
//-----------------------------------------------------------------

class Combobox: public ListBox
{
public:
	Combobox();
	virtual ~Combobox();

	virtual void AddItem(tstring const& str) override;

protected:
	virtual HWND CreateHandle(HWND hWnd) override;
	virtual LRESULT Procedure(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) override;
	
private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Combobox(const Combobox& tbRef);
	Combobox& operator=(const Combobox& tbRef);
};
