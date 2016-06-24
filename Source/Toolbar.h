#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// Toolbar Class
//-----------------------------------------------------------------

class Toolbar
{
public:
	Toolbar(HWND hwnd);

	virtual ~Toolbar();

	void SetBounds(int x, int y, int width, int height);
	tstring GetText();
	void SetText(tstring const& textRef);
	void SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size);
	void SetBackcolor( COLORREF color );
	void SetForecolor( COLORREF color );
	COLORREF GetForecolor();
	COLORREF GetBackcolor();
	HBRUSH GetBackcolorBrush();
	RECT GetRect();
	void SetEnabled(bool bEnable);
	void Update(void);
	void Show();
	void Hide();

	HWND GetWindow();

	void EnableBtnOpen();
	void DisableBtnOpen();
	void EnableBtnClose();
	void DisableBtnClose();

	void BtnCopyToAbort();
	void BtnAbortToCopy();

private:
	void Initialize();

	// -------------------------
	// Datamembers
	// -------------------------
	int m_x, m_y;
	HWND m_hWndEdit;
	WNDPROC m_procOldEdit;
	COLORREF m_BgColor, m_ForeColor;
	HBRUSH m_BgColorBrush;
	HFONT m_Font, m_OldFont;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK EditProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT EditProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Toolbar(const Toolbar& tbRef);
	Toolbar& operator=(const Toolbar& tbRef);
};
