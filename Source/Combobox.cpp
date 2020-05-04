#include "Combobox.h"

//-----------------------------------------------------------------
// Combobox methods
//-----------------------------------------------------------------
Combobox::Combobox() : ListBox()
{
}

Combobox::~Combobox()
{
}

HWND Combobox::CreateHandle(HWND hWnd)
{
	// Create the button object
	return CreateWindow(TEXT("COMBOBOX"), NULL, WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_AUTOHSCROLL, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

LRESULT Combobox::Procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{		
	case WM_COMMAND:
		// It's our listbox, check the notification code
		if(HIWORD(wParam) == LBN_SELCHANGE)
		{
			/*int item;
			for (int index = 0; index < 7; ++index)
			{
				item = SendMessage(m_hWndEdit, LB_GETSEL, (WPARAM)index, 0);
				if (item == TRUE) 
				{
					item = index;
					break;
				}
			}
			ListboxItemChange(hWnd, item);*/
			//CallerPtrCombobox(this);
			//MessageBox(NULL, _T("TEST"), _T("TEST"), NULL);
		}
		break;
	}
	return ListBox::Procedure(hWnd, msg, wParam, lParam);
}

void Combobox::AddItem(tstring const& str)
{
	SendMessage(GetWindow(),
						CB_ADDSTRING,
						0,
						(LPARAM) str.c_str());
}

