#include "Button.h"

//-----------------------------------------------------------------
// Button methods
//-----------------------------------------------------------------
Button::Button() : Control (), Callable(), m_Armed(false)
{
}

Button::~Button()
{
}

HWND Button::CreateHandle(HWND hWnd)
{
	// Create the button object
	return CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

LRESULT Button::Procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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
		break;
	}

	return Control::Procedure(hWnd, msg, wParam, lParam);
}
