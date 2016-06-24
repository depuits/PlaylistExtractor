#include "Checkbox.h"

//-----------------------------------------------------------------
// Checkbox methods
//-----------------------------------------------------------------
Checkbox::Checkbox() : Button ()
{
}

Checkbox::~Checkbox()
{
}

HWND Checkbox::CreateHandle(HWND hWnd)
{
	// Create the Checkbox object
	return CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | BS_CHECKBOX, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

bool Checkbox::GetChecked ()
{
	int checked = SendMessage(GetWindow(), BM_GETCHECK, 0, 0);
	return (checked == BST_CHECKED);
}
void Checkbox::SetChecked (bool bChecked)
{
	int checked = bChecked ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(GetWindow(), BM_SETCHECK, (WPARAM)checked, 0);
}

