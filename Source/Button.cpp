#include "Button.h"

//-----------------------------------------------------------------
// Button methods
//-----------------------------------------------------------------

#pragma warning(disable:4311)
#pragma warning(disable:4312)
Button::Button(HWND hWnd, tstring const& textRef) : Callable(), m_x(0), m_y(0), m_Font(0), m_OldFont(0), m_Armed(false)
{
	// Create the button object
	m_hWndButton = CreateWindow(TEXT("BUTTON"), textRef.c_str(), WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);

	// Set de new WNDPROC for the button, and store the old one
	m_procOldButton = (WNDPROC) SetWindowLong(m_hWndButton, GWL_WNDPROC, (LONG) ButtonProcStatic);

	// Store 'this' as data for the Button object so that the static PROC can call the member proc
	SetWindowLong(m_hWndButton, GWL_USERDATA, (LONG) this);

	this->SetFont(TEXT("Verdana"), false, false, false, 14);
}

Button::Button(HWND hWnd) : Callable(), m_x(0), m_y(0), m_Font(0), m_OldFont(0), m_Armed(false)
{
	// Create the button object
	m_hWndButton = CreateWindow(TEXT("BUTTON"), TEXT(""), WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);

	// Set de new WNDPROC for the button, and store the old one
	m_procOldButton = (WNDPROC) SetWindowLong(m_hWndButton, GWL_WNDPROC, (LONG) ButtonProcStatic);

	// Store 'this' as data for the Button object so that the static PROC can call the member proc
	SetWindowLong(m_hWndButton, GWL_USERDATA, (LONG) this);

	this->SetFont(TEXT("Verdana"), false, false, false, 14);
}
#pragma warning(default:4311)
#pragma warning(default:4312)

Button::~Button()
{
	// release the font if necessary
	if (m_Font != 0)
	{
		SelectObject(GetDC(m_hWndButton), m_OldFont);
		DeleteObject(m_Font);
		m_Font = m_OldFont = 0;
	}
		
	// release the window resource
	DestroyWindow(m_hWndButton);
	m_hWndButton = NULL;	
}

void Button::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWndButton, x, y, width, height, true);
}

RECT Button::GetRect()
{
	RECT rc;

	GetClientRect(m_hWndButton, &rc);
	
	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void Button::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWndButton, bEnable);
}

void Button::Update()
{
	UpdateWindow(m_hWndButton);
}

void Button::Show()
{
	// Show and update the button
	ShowWindow(m_hWndButton, SW_SHOW);
	UpdateWindow(m_hWndButton);
}

void Button::Hide()
{
	// Show and update the button
	ShowWindow(m_hWndButton, SW_HIDE);
	UpdateWindow(m_hWndButton);
}

tstring Button::GetText()
{
	int textLength = (int) SendMessage(m_hWndButton, (UINT) WM_GETTEXTLENGTH, 0, 0);
	
	TCHAR* buffer = new TCHAR[textLength + 1];

	SendMessage(m_hWndButton, (UINT) WM_GETTEXT, (WPARAM) textLength + 1, (LPARAM) buffer);

	tstring newString(buffer);

	delete buffer;

	return newString;
}

void Button::SetText(tstring const& textRef)
{
	SendMessage(m_hWndButton, WM_SETTEXT, 0, (LPARAM) textRef.c_str());
}

void Button::SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size)
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

	// create the new font. The WM_CTLCOLOREDIT message will set the font when the button is about to redraw
    m_Font = CreateFontIndirect(&ft);

	// redraw the button
	InvalidateRect(m_hWndButton, NULL, true);
}

LRESULT Button::ButtonProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<Button*>(GetWindowLong(hWnd, GWL_USERDATA))->ButtonProc(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}

LRESULT Button::ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CTLCOLOREDIT:
		if (m_Font != 0) 
		{
			if (m_OldFont == 0) m_OldFont = (HFONT) SelectObject((HDC) wParam, m_Font);
			else SelectObject((HDC) wParam, m_Font);
		}
		return 0;	
	case WM_LBUTTONDOWN :
	case WM_LBUTTONDBLCLK:					// clicking fast will throw LBUTTONDBLCLK's as well as LBUTTONDOWN's, you need to capture both to catch all button clicks
		m_Armed = true;
		break;
	case WM_LBUTTONUP :
		if (m_Armed)
		{
			RECT rc;
			POINT pt;
			GetWindowRect(hWnd, &rc);
			GetCursorPos(&pt);

			//if (PtInRect(&rc, pt) && m_Target) result = m_Target->CallAction(this);
			if (PtInRect(&rc, pt) && m_pCaller != 0) m_pCaller->CallAction(this);

			m_Armed = false;
		}
	}
	return CallWindowProc(m_procOldButton, hWnd, msg, wParam, lParam);
}


HWND Button::GetWindow()
{
	return m_hWndButton;
}
