#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------
class Button: public Callable
{
public:
	Button(HWND hWnd, tstring const& textRef);
	Button(HWND hWnd);

	virtual ~Button();

	void SetBounds(int x, int y, int width, int height);
	tstring GetText();
	void SetText(tstring const& textRef);
	void SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size);
	RECT GetRect();
	void SetEnabled(bool bEnable);
	void Update(void);
	void Show();
	void Hide();

	HWND GetWindow();

private:
	// -------------------------
	// Datamembers
	// -------------------------
	int m_x, m_y;
	HWND m_hWndButton;
	WNDPROC m_procOldButton;
	HFONT m_Font, m_OldFont;

	bool m_Armed;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK ButtonProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT ButtonProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Button(const Button& bRef);
	Button& operator=(const Button& bRef);
};
