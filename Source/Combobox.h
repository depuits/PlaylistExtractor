#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// Combobox Class
//-----------------------------------------------------------------

class Combobox
{
public:
	Combobox(HWND hwnd);

	virtual ~Combobox();

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

	void AddItem(tstring const& str);
	void RemoveItem(int index);

	int GetSelItem();
	void SetSelItem(int index);

	void DeleteAll();
	int Count();

	HWND GetWindow();

private:
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
	Combobox(const Combobox& tbRef);
	Combobox& operator=(const Combobox& tbRef);
};
