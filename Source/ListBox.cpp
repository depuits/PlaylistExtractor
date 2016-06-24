#include "ListBox.h"

//-----------------------------------------------------------------
// ListBox methods
//-----------------------------------------------------------------
ListBox::ListBox() : Control()
{
}

ListBox::~ListBox()
{
}

HWND ListBox::CreateHandle(HWND hWnd)
{
	// Create the button object
	return CreateWindow(TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

LRESULT ListBox::Procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR: 
		if (wParam == VK_TAB) return 0;
		if (wParam == VK_RETURN) return 0;
		break;
	}
	return Control::Procedure(hWnd, msg, wParam, lParam);
}

void ListBox::AddItem(tstring const& str)
{
	SendMessage(GetWindow(),
                        LB_ADDSTRING,
                        0,
                        (LPARAM) str.c_str());
}

void ListBox::RemoveItem(int index)
{
	SendMessage(GetWindow(), LB_DELETESTRING, (LPARAM)index, 0);
}

int ListBox::Count()
{
	return SendMessage(GetWindow(), LB_GETCOUNT, 0, 0);
}

int ListBox::GetSelItem()
{
	return SendMessage(GetWindow(),
						LB_GETCURSEL,
                        0,
                        0);
}

void ListBox::SetSelItem(int index)
{
	SendMessage(GetWindow(),
						LB_SETCURSEL, 
						(LPARAM)index, 
						0);
}

void ListBox::DeleteAll()
{
	SendMessage(GetWindow(),
						LB_RESETCONTENT,
                        0,
                        0);
}


