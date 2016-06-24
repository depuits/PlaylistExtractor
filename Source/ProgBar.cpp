#include "ProgBar.h"

#include <commctrl.h>
/*
//loading bar
		CreateWindowEx(0, PROGRESS_CLASS, NULL,
		               WS_CHILD | WS_VISIBLE,
			      10, 0, m_ClientWidth - 20, 20,
			      m_hMainWnd, NULL, NULL, NULL);*/

ProgBar::ProgBar(HWND hWnd): m_x(0), m_y(0)
{
	// Create the button object
	m_hWnd = CreateWindowEx(0, PROGRESS_CLASS, NULL,
		               WS_CHILD,
			      0, 0, 0, 0,
			      hWnd, NULL, NULL, NULL);
}

ProgBar::~ProgBar()
{
	// release the window resource
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;	
}

void ProgBar::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWnd, x, y, width, height, true);
}

RECT ProgBar::GetRect()
{
	RECT rc;

	GetClientRect(m_hWnd, &rc);
	
	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void ProgBar::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWnd, bEnable);
}

void ProgBar::Update()
{
	UpdateWindow(m_hWnd);
}

void ProgBar::Show()
{
	// Show and update the button
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

void ProgBar::Hide()
{
	// Show and update the button
	ShowWindow(m_hWnd, SW_HIDE);
	UpdateWindow(m_hWnd);
}



void ProgBar::SetRange(int r)
{
	SendMessage(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(0 ,r));
}
void ProgBar::SetPos(int p)
{
	SendMessage(m_hWnd, PBM_SETPOS, (WPARAM) p, 0);
}
void ProgBar::AddToPos(int a)
{
	SendMessage(m_hWnd, PBM_SETPOS, (WPARAM) a, 0);
}

void ProgBar::SetStep(int s)
{
	SendMessage(m_hWnd, PBM_SETSTEP, (WPARAM) s, 0);
}
void ProgBar::AddStep()
{
	SendMessage(m_hWnd, PBM_STEPIT, 0, 0);
}



HWND ProgBar::GetWindow()
{
	return m_hWnd;
}
