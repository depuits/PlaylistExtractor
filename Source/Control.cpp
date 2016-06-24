#include "Control.h"

//-----------------------------------------------------------------
// Control methods
//-----------------------------------------------------------------

Control::Control() : m_hWnd (0), m_x(0), m_y(0)
{
}

Control::~Control()
{
	// release the window resource
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;	
}

#pragma warning(disable:4311)
#pragma warning(disable:4312)
void Control::Create(HWND hWnd)
{
	m_hWnd = CreateHandle(hWnd);

	// Set de new WNDPROC for the button, and store the old one
	m_procOld = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)StaticProcedure);

	// Store 'this' as data for the Control object so that the static PROC can call the member proc
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG) this);
}
#pragma warning(default:4311)
#pragma warning(default:4312)

void Control::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWnd, x, y, width, height, true);
}

RECT Control::GetRect()
{
	RECT rc;

	GetClientRect(m_hWnd, &rc);
	
	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void Control::SetText(tstring const& textRef)
{
	SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM) textRef.c_str());
}

tstring Control::GetText()
{
	int textLength = (int) SendMessage(m_hWnd, (UINT) WM_GETTEXTLENGTH, 0, 0);
	
	TCHAR* buffer = new TCHAR[textLength + 1];

	SendMessage(m_hWnd, (UINT) WM_GETTEXT, (WPARAM) textLength + 1, (LPARAM) buffer);

	tstring newString(buffer);

	delete buffer;

	return newString;
}

void Control::SetFont(HFONT font)
{
	SendMessage(m_hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)true);
}

void Control::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWnd, bEnable);
}

void Control::Update()
{
	UpdateWindow(m_hWnd);
}

void Control::Show()
{
	// Show and update the button
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

void Control::Hide()
{
	// Show and update the button
	ShowWindow(m_hWnd, SW_HIDE);
	UpdateWindow(m_hWnd);
}

HWND Control::GetWindow()
{
	return m_hWnd;
}

LRESULT Control::Procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(m_procOld, hWnd, msg, wParam, lParam);
}

LRESULT Control::StaticProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<Control*>(GetWindowLong(hWnd, GWL_USERDATA))->Procedure(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}
