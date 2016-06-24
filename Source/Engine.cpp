#include "Engine.h"

// for folder selection
#include <shlobj.h>
#include <objbase.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

Engine* Engine::m_pEngine = 0;

Engine::Engine(): 
m_FontDraw(0)
{
}
Engine::~Engine()
{
	if (m_FontDraw == 0) delete m_FontDraw;
}

Engine* Engine::GetSingleton()
{
	if (m_pEngine == 0) m_pEngine = new Engine();
	return m_pEngine;
}

//string draws
void Engine::DrawString(tstring str, int x, int y, HDC hdc)
{	
	HFONT hOldFont;

	if (m_FontDraw != 0) hOldFont = (HFONT) SelectObject(hdc, m_FontDraw);

	SetBkMode(hdc, TRANSPARENT);

	int count = 0;
	while (str.c_str()[count] != '\0') count++;

	TextOut(hdc, x, y, str.c_str(), count);

	SetBkMode(hdc, OPAQUE);

	if (m_FontDraw != 0) SelectObject(hdc, hOldFont);

	//old code w/o font
	//TextOut(hdc, x, y, str.c_str(), _tcslen(str.c_str()));
}

void Engine::SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size)
{
	if (m_FontDraw != 0) DeleteObject(m_FontDraw);

	LOGFONT ft;
	ZeroMemory(&ft, sizeof(ft));

	_tcscpy_s(ft.lfFaceName, sizeof(ft.lfFaceName) / sizeof(TCHAR), fontNameRef.c_str());
	ft.lfStrikeOut = 0;
	ft.lfUnderline = underline?1:0;
	ft.lfHeight = size;
    ft.lfEscapement = 0;
	ft.lfWeight = bold?FW_BOLD:0;
	ft.lfItalic = italic?1:0;

    m_FontDraw = CreateFontIndirect(&ft);
}

// paint
bool Engine::Repaint(HWND hWnd) 
{
	return InvalidateRect(hWnd, NULL, true)?true:false;
}

//other
bool Engine::FolderExists(tstring strFolderName)
{
    return GetFileAttributes(strFolderName.c_str()) != INVALID_FILE_ATTRIBUTES;  
}

tstring Engine::OpenFileWnd(HWND hWnd)
{
	//Filename
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH] = _T("");

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = _T("M3U Playlist (*.m3u)\0*.m3u\0M3U8 (Unicode) Playlist (*.m3u8)\0*.m3u8\0Windows Media Playlist (*.wpl)\0*.wpl\0All Files (*.*)\0*.*\0");//_T("M3U Playlist (*.m3u)\0*.m3u\0PLS Playlist (*.pls)\0*.pls\0M3U8 (Unicode) Playlist (*.m3u8)\0*.m3u8\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | /*OFN_ALLOWMULTISELECT |*/ OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = _T("m3u");

	if (GetOpenFileName(&ofn)) return (szFileName);
	else return _T("");
}

tstring Engine::SaveFileWnd(HWND hWnd, tstring Cur)
{
	//Filename
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH] = _T("");

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = _T("M3U Playlist (*.m3u)\0*.m3u\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = _T("exe");

	if (GetOpenFileName(&ofn)) return (szFileName);
	else return _T("");
}

BOOL Engine::GetFolderSelection(HWND hWnd, LPTSTR szBuf, LPCTSTR szTitle)
{
	LPITEMIDLIST pidl     = NULL;
	BROWSEINFO   bi       = { 0 };
	BOOL         bResult  = FALSE;

	bi.hwndOwner      = hWnd;
	bi.pszDisplayName = szBuf;
	bi.pidlRoot       = NULL;
	bi.lpszTitle      = szTitle;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		bResult = SHGetPathFromIDList(pidl, szBuf);
		CoTaskMemFree(pidl);
	}

	return bResult;
}


void Engine::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

HBRUSH Engine::GetBackGroundColor()
{
	return CreateSolidBrush(RGB(255, 255, 255));
}

void Engine::SetDefaultFont (HWND hwnd)
{
    SendMessage(hwnd, WM_SETFONT, (WPARAM)m_FontDraw, (LPARAM)true);

    return;
}

void Engine::DispErr(int nError)
{
    static TCHAR szErrorText[1024];
    if( FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,NULL,nError,0,szErrorText,1024,NULL) ){
        MessageBox(NULL,szErrorText,_T("Error"),MB_OK|MB_ICONERROR);
    }
}

unsigned long Engine::GetFileSize(const tstring& fileName)
{
    BOOL                        fOk;
    WIN32_FILE_ATTRIBUTE_DATA   fileInfo;

    if (_T("") == fileName)
        return -1;

    fOk = GetFileAttributesEx(fileName.c_str(), GetFileExInfoStandard, (void*)&fileInfo);
    if (!fOk)
        return -1;
    //assert(0 == fileInfo.nFileSizeHigh);
    return fileInfo.nFileSizeLow;
}

unsigned long Engine::ByteToMegaBytes(unsigned long Bytes)
{
	unsigned long MB = 0;
	MB = Bytes/1000000;
	return MB;
}

unsigned long Engine::ByteToKiloBytes(unsigned long Bytes)
{
	unsigned long MB = 0;
	MB = Bytes/1000;
	return MB;
}

unsigned long Engine::GetFreeDiskSpace(const tstring& path)
{
	unsigned long bytes = 0;

	if(! GetDiskFreeSpaceEx( path.c_str(), NULL, NULL, (PULARGE_INTEGER)bytes)) 
		MessageBox(NULL, _T("Unable to calculate diskspace."), _T("ERROR"), MB_ICONERROR);

	return bytes;
}
