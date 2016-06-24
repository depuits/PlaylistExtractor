#include "TextBox.h"

//-----------------------------------------------------------------
// TextBox methods
//-----------------------------------------------------------------
TextBox::TextBox() : Control ()
{
}

TextBox::~TextBox()
{
}

HWND TextBox::CreateHandle(HWND hWnd)
{
	return CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

LRESULT TextBox::Procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

void TextBox::SetPasswordChar(TCHAR const& txt)
{
	SendMessage(GetWindow(), EM_SETPASSWORDCHAR, (WPARAM) txt, 0);
}
