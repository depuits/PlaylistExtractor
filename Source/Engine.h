#pragma once

#include "Includes.h"

class Engine
{
public:
	virtual ~Engine();

	static Engine* GetSingleton();
	
	// string
	void DrawString(tstring str, int x, int y, HDC hdc);
	void SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size);

	// paint
	bool Repaint(HWND hWnd);

	//other
	bool FolderExists(tstring strFolderName);
	
	tstring OpenFileWnd(HWND hWnd);
	tstring SaveFileWnd(HWND hWnd, tstring Cur);
	BOOL GetFolderSelection(HWND hWnd, LPTSTR szBuf, LPCTSTR szTitle);

	void GetDesktopResolution(int& horizontal, int& vertical);

	HBRUSH GetBackGroundColor();

	HFONT GetDefaultFont();
	void SetDefaultFont(HWND hwnd);

	void DispErr(int nError);

	unsigned long GetFileSize(const tstring& fileName);

	unsigned long ByteToMegaBytes(unsigned long Bytes);
	unsigned long ByteToKiloBytes(unsigned long Bytes);
	unsigned long GetFreeDiskSpace(const tstring& path);

private:

	Engine();
	static Engine* m_pEngine;

	HFONT m_FontDraw;
};
