#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Includes.h"

//-----------------------------------------------------------------
// ProgressBar Class
//-----------------------------------------------------------------
class ProgBar
{
public:
	ProgBar(HWND hWnd);

	virtual ~ProgBar();

	void SetBounds(int x, int y, int width, int height);
	RECT GetRect();
	void SetEnabled(bool bEnable);
	void Update(void);
	void Show();
	void Hide();

	void SetRange(int r);
	void SetPos(int p);
	void AddToPos(int a);

	void SetStep(int s);
	void AddStep();

	HWND GetWindow();

private:
	// -------------------------
	// Datamembers
	// -------------------------
	int m_x, m_y;
	HWND m_hWnd;
	WNDPROC m_procOldProgBar;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK ProgBarProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT ProgBarProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ProgBar(const ProgBar& bRef);
	ProgBar& operator=(const ProgBar& bRef);
};
