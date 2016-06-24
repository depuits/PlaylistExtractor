#include "ProgBar.h"

#include <commctrl.h>


ProgBar::ProgBar(): Control ()
{
}

ProgBar::~ProgBar()
{
}

HWND ProgBar::CreateHandle(HWND hWnd)
{
	return CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

void ProgBar::SetRange(int r)
{
	SendMessage(GetWindow(), PBM_SETRANGE, 0, MAKELPARAM(0 ,r));
}
void ProgBar::SetPos(int p)
{
	SendMessage(GetWindow(), PBM_SETPOS, (WPARAM) p, 0);
}
void ProgBar::AddToPos(int a)
{
	SendMessage(GetWindow(), PBM_SETPOS, (WPARAM) a, 0);
}

void ProgBar::SetStep(int s)
{
	SendMessage(GetWindow(), PBM_SETSTEP, (WPARAM) s, 0);
}
void ProgBar::AddStep()
{
	SendMessage(GetWindow(), PBM_STEPIT, 0, 0);
}
