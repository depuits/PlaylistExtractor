#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// Control Class
//-----------------------------------------------------------------
class Control
{
public:
	Control();
	virtual ~Control();

	virtual void Create(HWND hWnd);

	void SetBounds(int x, int y, int width, int height);
	RECT GetRect();

	void SetText(tstring const& textRef);
	tstring GetText();

	void SetFont(HFONT font);
	
	void SetEnabled(bool bEnable);

	void Update();

	void Show();
	void Hide();

	HWND GetWindow();

protected:
	virtual HWND CreateHandle(HWND hWnd) = 0;

	virtual LRESULT Procedure (HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);	

private:
	// -------------------------
	// Datamembers
	// -------------------------
	HWND m_hWnd;
	int m_x, m_y;
	WNDPROC m_procOld;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK StaticProcedure(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Control(const Control& bRef);
	Control& operator=(const Control& bRef);
};
