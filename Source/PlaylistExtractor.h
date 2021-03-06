#pragma once

#include "Includes.h"

#include "Button.h"
#include "ListView.h"
#include "ComboBox.h"
#include "Checkbox.h"
#include "TextBox.h"
#include "ProgBar.h"
#include "Toolbar.h"

struct Song
{
	Song(tstring name, tstring dir): 
								strName(name), 
								strDir(dir){}
	tstring strName;
	tstring strDir;
};

class PlaylistExtractor: public CallerPtr
{
public:
	virtual ~PlaylistExtractor();

	static PlaylistExtractor* GetSingleton();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int Run();

	void InitApp(HINSTANCE hInstance, int showCmd);

	// app
	void AppStart();
	void AppPaint(HDC hdc);
	void AppEnd();
	void OnResize();

	void ToggleEncoding(bool old);
	void ReadInList(tstring str);

	unsigned long Totalsize();

	static DWORD WINAPI CopyThreadProc(void *param);
	int DoCopy();
	void StartCopy();
	void EndCopy();


	void OpenList();
	void CloseList();

	void CopyAbort();

	void EnableMainMenuItems(bool open, bool close);
	void SetMainMenuCopyText(LPCTSTR text);

	//start other windows
	void StartAbout();
	void StartBug();

	void SetHwnd(HWND hwnd);
	HWND GetHwnd();

	void ChangCopyDir();

	void SetClientHeight(int height);
	void SetClientWidth(int width);

	void CallAction(Callable* caller);

private:
	PlaylistExtractor();
	void InitMainWindow(int showCmd);

	// --------------------------------------------
	// vars
	// --------------------------------------------
	static PlaylistExtractor* m_pApp;

	HINSTANCE m_hAppInst;
	HWND      m_hMainWnd;

	int m_ClientWidth,
		m_ClientHeight;

	bool m_isCopie,
		 m_useOldEncoding;

	tstring m_sBugReport;
	tstring m_sAbout;

	ListView *m_pLBMusicList;

	TextBox *m_pTxtDirection;

	Button	*m_pBtnCopie,
			*m_pBtnCopieDir;

	Checkbox *m_pChkMove;

	Toolbar *m_pTBMain;

	ProgBar *m_pProgBarCopying;

	tstring m_sHardDrive;

	deque<Song> m_SongList;
};
