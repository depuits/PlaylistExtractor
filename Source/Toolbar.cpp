#include "Toolbar.h"

//-----------------------------------------------------------------
// Toolbar methods
//-----------------------------------------------------------------

#pragma warning(disable:4311)	
#pragma warning(disable:4312)
Toolbar::Toolbar(HWND hwnd) : m_x(0), m_y(0), m_BgColor(RGB(255, 255, 255)), m_ForeColor(RGB(0, 0, 0)), m_BgColorBrush(0), m_Font(0), m_OldFont(0)
{
	// Create the edit box
	m_hWndEdit = CreateWindowEx(NULL, TOOLBARCLASSNAME, NULL, 
        WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TRANSPARENT,
        0, 0, 0, 0,
        hwnd, NULL, NULL, NULL);

	Initialize();

	// Set de nieuwe WNDPROC voor de edit box, en houd de oude bij 
	m_procOldEdit = (WNDPROC) SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG) EditProcStatic);

	// Stel dit object in als userdata voor de statische wndproc functie van de edit box zodat deze members kan aanroepen
	SetWindowLong(m_hWndEdit, GWL_USERDATA, (LONG) this);

	this->SetFont(TEXT("Verdana"), false, false, false, 14);
}
#pragma warning(default:4311)
#pragma warning(default:4312)

Toolbar::~Toolbar()
{
	// release the background brush if necessary
	if (m_BgColorBrush != 0) 
	{
		DeleteObject(m_BgColorBrush);
		m_BgColorBrush = 0;
	}

	// release the font if necessary
	if (m_Font != 0)
	{
		SelectObject(GetDC(m_hWndEdit), m_OldFont);
		DeleteObject(m_Font);
		m_Font = m_OldFont = 0;
	}
		
	// release the window resources
	DestroyWindow(m_hWndEdit);
	m_hWndEdit = NULL;
}

void Toolbar::Initialize()
{
	// Define some constants.
    const int numButtons = 9;
    const DWORD buttonStyles = BTNS_AUTOSIZE;
    const int bitmapSize = 16;


    // Create the imagelist.
	HIMAGELIST hImageList = ImageList_Create(
        bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
        ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
        numButtons, 0);

    // Set the image list.
    SendMessage(m_hWndEdit, TB_SETIMAGELIST, (WPARAM)0, 
      (LPARAM)hImageList);
 
	
    // Load the button images.
	SendMessage(m_hWndEdit, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, 
        (LPARAM)HINST_COMMCTRL);

    // Initialize button info.
    TBBUTTON tbButtons[numButtons] = 
    {
        { MAKELONG(STD_FILEOPEN, 0), IDM_OPEN, TBSTATE_ENABLED, 
          buttonStyles, {0}, 0, (INT_PTR)L"Open" },

        { MAKELONG(STD_DELETE, 0), IDM_CLOSE, TBSTATE_ENABLED, 
          buttonStyles, {0}, 0, (INT_PTR)L"Close"},

        { 0, 0, 0, 
          BTNS_SEP, {0}, 0, 0},
        
		{ MAKELONG(STD_FIND, 0), IDM_FIND, TBSTATE_ENABLED,
          buttonStyles, {0}, 0, (INT_PTR)L"Find..."},

        { 0, 0, 0, 
          BTNS_SEP, {0}, 0, 0},

        { MAKELONG(IDB_TB_CHECK1, 0), IDM_CLOSE, 0, 
          buttonStyles, {0}, 0, (INT_PTR)L"Select"},

        { MAKELONG(IDB_TB_CHECK0, 0), IDM_CLOSE, 0, 
          buttonStyles, {0}, 0, (INT_PTR)L"Deselect"},

        { 0, 0, 0, 
          BTNS_SEP, {0}, 0, 0},

        { MAKELONG(STD_COPY, 0), IDM_COPY, TBSTATE_ENABLED,
          buttonStyles, {0}, 0, (INT_PTR)L"Copy"}
    };

    // Add buttons.
    SendMessage(m_hWndEdit, TB_BUTTONSTRUCTSIZE, 
        (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(m_hWndEdit, TB_ADDBUTTONS, (WPARAM)numButtons, 
        (LPARAM)&tbButtons);
}

void Toolbar::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWndEdit, x, y, width, height, true);
}

RECT Toolbar::GetRect()
{
	RECT rc;

	GetClientRect(m_hWndEdit, &rc);

	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void Toolbar::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWndEdit, bEnable);
}

void Toolbar::Update()
{
	UpdateWindow(m_hWndEdit);
}

void Toolbar::Show()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);
}

void Toolbar::Hide()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_HIDE);
	UpdateWindow(m_hWndEdit);
}

tstring Toolbar::GetText()
{
	int textLength = (int) SendMessage(m_hWndEdit, (UINT) WM_GETTEXTLENGTH, 0, 0);
	
	TCHAR* buffer = new TCHAR[textLength + 1];

	SendMessage(m_hWndEdit, (UINT) WM_GETTEXT, (WPARAM) textLength + 1, (LPARAM) buffer);

	tstring newString(buffer);

	delete buffer;

	return newString;
}

void Toolbar::SetText(tstring const& textRef)
{
	SendMessage(m_hWndEdit, WM_SETTEXT, 0, (LPARAM) textRef.c_str());
}

void Toolbar::SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size)
{
	LOGFONT ft;

	_tcscpy_s(ft.lfFaceName, sizeof(ft.lfFaceName) / sizeof(TCHAR), fontNameRef.c_str());
	ft.lfStrikeOut = 0;
	ft.lfUnderline = underline?1:0;
	ft.lfHeight = size;
    ft.lfEscapement = 0;
	ft.lfWeight = bold?FW_BOLD:0;
	ft.lfItalic = italic?1:0;

	// clean up if another custom font was already in place
	if (m_Font != 0) { DeleteObject(m_Font); }

	// create the new font. The WM_CTLCOLOREDIT message will set the font when the textbox is about to redraw
    m_Font = CreateFontIndirect(&ft);

	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

void Toolbar::SetForecolor( COLORREF color )
{
	m_ForeColor = color;
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

void Toolbar::SetBackcolor( COLORREF color )
{
	m_BgColor = color;
	
	if (m_BgColorBrush != 0) DeleteObject(m_BgColorBrush);
	m_BgColorBrush = CreateSolidBrush( color );
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

COLORREF Toolbar::GetForecolor()
{
	return m_ForeColor;
}

COLORREF Toolbar::GetBackcolor()
{
	return m_BgColor;
}

HBRUSH Toolbar::GetBackcolorBrush()
{
	return m_BgColorBrush;
}

LRESULT Toolbar::EditProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<Toolbar*>(GetWindowLong(hWnd, GWL_USERDATA))->EditProc(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}

LRESULT Toolbar::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{		
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC) wParam, GetBackcolor() );
		SetTextColor((HDC) wParam, GetForecolor() );
		if (m_Font != 0) 
		{
			if (m_OldFont == 0) m_OldFont = (HFONT) SelectObject((HDC) wParam, m_Font);
			else SelectObject((HDC) wParam, m_Font);
		}
		return (LRESULT) GetBackcolorBrush();

	case WM_CHAR: 
		if (wParam == VK_TAB) return 0;
		if (wParam == VK_RETURN) return 0;
		break;
	}
	return CallWindowProc(m_procOldEdit, hWnd, msg, wParam, lParam);
}

HWND Toolbar::GetWindow()
{
	return m_hWndEdit;
}

void Toolbar::EnableBtnOpen()
{
	SendMessage(m_hWndEdit, TB_ENABLEBUTTON, (WPARAM)IDM_OPEN, 
        (LPARAM)true);
}
void Toolbar::DisableBtnOpen()
{
	SendMessage(m_hWndEdit, TB_ENABLEBUTTON, (WPARAM)IDM_OPEN, 
        (LPARAM)false);
}
void Toolbar::EnableBtnClose()
{
	SendMessage(m_hWndEdit, TB_ENABLEBUTTON, (WPARAM)IDM_CLOSE, 
        (LPARAM)true);
}
void Toolbar::DisableBtnClose()
{
	SendMessage(m_hWndEdit, TB_ENABLEBUTTON, (WPARAM)IDM_CLOSE, 
        (LPARAM)false);
}

void Toolbar::BtnCopyToAbort()
{
	//change text
	SendMessage(m_hWndEdit, TB_DELETEBUTTON, (WPARAM)8, 0);

	TBBUTTON tbButton = {MAKELONG(STD_DELETE, 0), IDM_COPY, TBSTATE_ENABLED,
	BTNS_AUTOSIZE, {0}, 0, (INT_PTR)_T("Abort")};

	SendMessage(m_hWndEdit, TB_INSERTBUTTON, (WPARAM)IDM_COPY, (LPARAM)&tbButton); 

	//change bmp
	SendMessage(m_hWndEdit, TB_CHANGEBITMAP, (WPARAM)IDM_COPY, 
        (LPARAM)MAKELONG(STD_DELETE, 0));
}
void Toolbar::BtnAbortToCopy()
{
	//change text
	SendMessage(m_hWndEdit, TB_DELETEBUTTON, (WPARAM)8, 0);

	TBBUTTON tbButton = {MAKELONG(STD_COPY, 0), IDM_COPY, TBSTATE_ENABLED,
	BTNS_AUTOSIZE, {0}, 0, (INT_PTR)_T("Copy")};

	SendMessage(m_hWndEdit, TB_INSERTBUTTON, (WPARAM)IDM_COPY, (LPARAM)&tbButton); 

	//change bmp
	SendMessage(m_hWndEdit, TB_CHANGEBITMAP, (WPARAM)IDM_COPY, 
        (LPARAM)MAKELONG(STD_COPY, 0));
}
