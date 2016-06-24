#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// ListBox Class
//-----------------------------------------------------------------

class ListBox : public Control
{
public:
	ListBox();
	virtual ~ListBox();

	virtual void AddItem(tstring const& str);
	virtual void RemoveItem(int index);

	virtual int GetSelItem();
	virtual void SetSelItem(int index);

	virtual void DeleteAll();
	virtual int Count();

protected:
	virtual HWND CreateHandle(HWND hWnd) override;
	virtual LRESULT Procedure(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ListBox(const ListBox& tbRef);
	ListBox& operator=(const ListBox& tbRef);
};
