#pragma once

#include "Includes.h"

class Search: public CallerPtr
{
public:
	virtual ~Search();
	static Search* GetSingleton();

	void OpenWnd();
	void CloseWnd();

	void CallAction(Callable* caller);

private:
	Search();
	void InitializeWindow();

	// -------------------------
	// Datamembers
	// -------------------------
	WNDPROC m_procOld;

	//datamembers
	static Search* m_pSearch;

	HWND m_hWnd;

	int m_ClientWidth,
		m_ClientHeight;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK ProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Proc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

};
