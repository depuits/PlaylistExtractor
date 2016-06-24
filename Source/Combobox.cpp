#include "Combobox.h"

//-----------------------------------------------------------------
// Combobox methods
//-----------------------------------------------------------------

#pragma warning(disable:4311)	
#pragma warning(disable:4312)
Combobox::Combobox(HWND hwnd) : m_x(0), m_y(0), m_BgColor(RGB(255, 255, 255)), m_ForeColor(RGB(0, 0, 0)), m_BgColorBrush(0), m_Font(0), m_OldFont(0)
{
	// Create the edit box
	m_hWndEdit = CreateWindow(TEXT("COMBOBOX"), NULL, WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_AUTOHSCROLL, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

	// Set de nieuwe WNDPROC voor de edit box, en houd de oude bij 
	m_procOldEdit = (WNDPROC) SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG) EditProcStatic);

	// Stel dit object in als userdata voor de statische wndproc functie van de edit box zodat deze members kan aanroepen
	SetWindowLong(m_hWndEdit, GWL_USERDATA, (LONG) this);

	this->SetFont(_T("Verdana"), false, false, false, 14);
}
#pragma warning(default:4311)
#pragma warning(default:4312)

Combobox::~Combobox()
{
	// release the background brush if necessary
	if (m_BgColorBrush != 0) 
	{
		DeleteObject(m_BgColorBrush);
		m_BgColorBrush = 0;
	}

	// release the font if necessary
	if (m_Font != 0)
	{
		SelectObject(GetDC(m_hWndEdit), m_OldFont);
		DeleteObject(m_Font);
		m_Font = m_OldFont = 0;
	}
		
	// release the window resources
	DestroyWindow(m_hWndEdit);
	m_hWndEdit = NULL;
}

void Combobox::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWndEdit, x, y, width, height, true);
}

RECT Combobox::GetRect()
{
	RECT rc;

	GetClientRect(m_hWndEdit, &rc);

	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void Combobox::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWndEdit, bEnable);
}

void Combobox::Update()
{
	UpdateWindow(m_hWndEdit);
}

void Combobox::Show()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);
}

void Combobox::Hide()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_HIDE);
	UpdateWindow(m_hWndEdit);
}

tstring Combobox::GetText()
{
	int textLength = (int) SendMessage(m_hWndEdit, (UINT) WM_GETTEXTLENGTH, 0, 0);
	
	TCHAR* buffer = new TCHAR[textLength + 1];

	SendMessage(m_hWndEdit, (UINT) WM_GETTEXT, (WPARAM) textLength + 1, (LPARAM) buffer);

	tstring newString(buffer);

	delete buffer;

	return newString;
}

void Combobox::SetText(tstring const& textRef)
{
	SendMessage(m_hWndEdit, WM_SETTEXT, 0, (LPARAM) textRef.c_str());
}

void Combobox::SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size)
{
	LOGFONT ft;

	_tcscpy_s(ft.lfFaceName, sizeof(ft.lfFaceName) / sizeof(TCHAR), fontNameRef.c_str());
	ft.lfStrikeOut = 0;
	ft.lfUnderline = underline?1:0;
	ft.lfHeight = size;
    ft.lfEscapement = 0;
	ft.lfWeight = bold?FW_BOLD:0;
	ft.lfItalic = italic?1:0;

	// clean up if another custom font was already in place
	if (m_Font != 0) { DeleteObject(m_Font); }

	// create the new font. The WM_CTLCOLOREDIT message will set the font when the textbox is about to redraw
    m_Font = CreateFontIndirect(&ft);

	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

void Combobox::SetForecolor( COLORREF color )
{
	m_ForeColor = color;
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

void Combobox::SetBackcolor( COLORREF color )
{
	m_BgColor = color;
	
	if (m_BgColorBrush != 0) DeleteObject(m_BgColorBrush);
	m_BgColorBrush = CreateSolidBrush( color );
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

COLORREF Combobox::GetForecolor()
{
	return m_ForeColor;
}

COLORREF Combobox::GetBackcolor()
{
	return m_BgColor;
}

HBRUSH Combobox::GetBackcolorBrush()
{
	return m_BgColorBrush;
}

LRESULT Combobox::EditProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<Combobox*>(GetWindowLong(hWnd, GWL_USERDATA))->EditProc(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}

LRESULT Combobox::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{		
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC) wParam, GetBackcolor() );
		SetTextColor((HDC) wParam, GetForecolor() );
		if (m_Font != 0) 
		{
			if (m_OldFont == 0) m_OldFont = (HFONT) SelectObject((HDC) wParam, m_Font);
			else SelectObject((HDC) wParam, m_Font);
		}
		return (LRESULT) GetBackcolorBrush();

	case WM_CHAR: 
		if (wParam == VK_TAB) return 0;
		if (wParam == VK_RETURN) return 0;
		break;
	case WM_COMMAND:
        // It's our listbox, check the notification code
        if(HIWORD(wParam) ==  LBN_SELCHANGE)
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
			//MessageBox(NULL, L"TEST", L"TEST", NULL);
        }
	    break;
	}
	return CallWindowProc(m_procOldEdit, hWnd, msg, wParam, lParam);
}

void Combobox::AddItem(tstring const& str)
{
	SendMessage(m_hWndEdit,
                        CB_ADDSTRING,
                        0,
                        (LPARAM) str.c_str());
}

void Combobox::RemoveItem(int index)
{
	SendMessage(m_hWndEdit, LB_DELETESTRING, (LPARAM)index, 0);
}

int Combobox::Count()
{
	return SendMessage(m_hWndEdit, LB_GETCOUNT, 0, 0);
}

int Combobox::GetSelItem()
{
	return SendMessage(m_hWndEdit,
						LB_GETCURSEL,
                        0,
                        0);
}

void Combobox::SetSelItem(int index)
{
	SendMessage(m_hWndEdit, 
						LB_SETCURSEL, 
						(LPARAM)index, 
						0);
}

void Combobox::DeleteAll()
{
	SendMessage(m_hWndEdit,
						LB_RESETCONTENT,
                        0,
                        0);
}


HWND Combobox::GetWindow()
{
	return m_hWndEdit;
}

