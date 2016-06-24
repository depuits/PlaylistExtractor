#include "TextBox.h"

//-----------------------------------------------------------------
// TextBox methods
//-----------------------------------------------------------------

#pragma warning(disable:4311)	
#pragma warning(disable:4312)
TextBox::TextBox(tstring const& textRef, HWND hwnd) : m_x(0), m_y(0), m_BgColor(RGB(255, 255, 255)), m_ForeColor(RGB(0, 0, 0)), m_BgColorBrush(0), m_Font(0), m_OldFont(0)
{
	// Create the edit box
	m_hWndEdit = CreateWindow(TEXT("EDIT"), textRef.c_str(), WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

	// Set de nieuwe WNDPROC voor de edit box, en houd de oude bij 
	m_procOldEdit = (WNDPROC) SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG) EditProcStatic);

	// Stel dit object in als userdata voor de statische wndproc functie van de edit box zodat deze members kan aanroepen
	SetWindowLong(m_hWndEdit, GWL_USERDATA, (LONG) this);

	this->SetFont(_T("Times New Roman"), false, false, false, 12);
}

TextBox::TextBox(HWND hwnd) : m_x(0), m_y(0), m_BgColor(RGB(255, 255, 255)), m_ForeColor(RGB(0, 0, 0)), m_BgColorBrush(0), m_Font(0), m_OldFont(0)
{
	// Create the edit box
	m_hWndEdit = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

	// Set de nieuwe WNDPROC voor de edit box, en houd de oude bij 
	m_procOldEdit = (WNDPROC) SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG) EditProcStatic);

	// Stel dit object in als userdata voor de statische wndproc functie van de edit box zodat deze members kan aanroepen
	SetWindowLong(m_hWndEdit, GWL_USERDATA, (LONG) this);

	SetFont(_T("Times New Roman"), false, false, false, 12);
}
#pragma warning(default:4311)
#pragma warning(default:4312)

TextBox::~TextBox()
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

void TextBox::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWndEdit, x, y, width, height, true);
}

RECT TextBox::GetRect()
{
	RECT rc;

	GetClientRect(m_hWndEdit, &rc);

	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void TextBox::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWndEdit, bEnable);
}

void TextBox::Update()
{
	UpdateWindow(m_hWndEdit);
}

void TextBox::Show()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);
}

void TextBox::Hide()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_HIDE);
	UpdateWindow(m_hWndEdit);
}

tstring TextBox::GetText()
{
	int textLength = (int) SendMessage(m_hWndEdit, (UINT) WM_GETTEXTLENGTH, 0, 0);
	
	TCHAR* buffer = new TCHAR[textLength + 1];

	SendMessage(m_hWndEdit, (UINT) WM_GETTEXT, (WPARAM) textLength + 1, (LPARAM) buffer);

	tstring newString(buffer);

	delete buffer;

	return newString;
}

void TextBox::SetText(tstring const& textRef)
{
	SendMessage(m_hWndEdit, WM_SETTEXT, 0, (LPARAM) textRef.c_str());
}

void TextBox::SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size)
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

	SendMessage(m_hWndEdit, WM_SETFONT, (WPARAM)m_Font, 0);
}

void TextBox::SetForecolor( COLORREF color )
{
	m_ForeColor = color;
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

void TextBox::SetBackcolor( COLORREF color )
{
	m_BgColor = color;
	
	if (m_BgColorBrush != 0) DeleteObject(m_BgColorBrush);
	m_BgColorBrush = CreateSolidBrush( color );
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

COLORREF TextBox::GetForecolor()
{
	return m_ForeColor;
}

COLORREF TextBox::GetBackcolor()
{
	return m_BgColor;
}

HBRUSH TextBox::GetBackcolorBrush()
{
	return m_BgColorBrush;
}

LRESULT TextBox::EditProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<TextBox*>(GetWindowLong(hWnd, GWL_USERDATA))->EditProc(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}

LRESULT TextBox::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
	}
	return CallWindowProc(m_procOldEdit, hWnd, msg, wParam, lParam);
}

void TextBox::SetPasswordChar(TCHAR const& txt)
{
	SendMessage(m_hWndEdit, EM_SETPASSWORDCHAR, (WPARAM) txt, 0);
}

HWND TextBox::GetWindow()
{
	return m_hWndEdit;
}
