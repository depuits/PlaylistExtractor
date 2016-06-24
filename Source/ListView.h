#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// ListView Class
//-----------------------------------------------------------------

class ListView : public Control
{
public:
	ListView();
	virtual ~ListView();

	virtual void Create(HWND hWnd) override;
	
	void AddItem(tstring const& sName, tstring const& sSize);
	void RemoveItem(int index);

	int GetSelItem();
	void SetSelItem(int index);

	void DeleteAll();
	int Count();
	int SelectedCount();

	void SetCheckedState(int index, bool checked);
	bool GetCheckedState(int index);

	void SortOnIndex();

protected:
	virtual HWND CreateHandle(HWND hWnd) override;
	virtual LRESULT Procedure(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void AddCollumns(); //TODO make more dynamic
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ListView(const ListView& tbRef);
	ListView& operator=(const ListView& tbRef);
};
