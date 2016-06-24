#include "ListView.h"

//-----------------------------------------------------------------
// ListView methods
//-----------------------------------------------------------------

#pragma warning(disable:4311)	
#pragma warning(disable:4312)
ListView::ListView(HWND hwnd) : m_x(0), m_y(0), m_BgColor(RGB(255, 255, 255)), m_ForeColor(RGB(0, 0, 0)), m_BgColorBrush(0), m_Font(0), m_OldFont(0)
{
	// Create the edit box
	m_hWndEdit = CreateWindow(WC_LISTVIEW, _T(""), WS_BORDER | WS_CHILD | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL | LVS_REPORT, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

	AddCollumns();	

	// Set de nieuwe WNDPROC voor de edit box, en houd de oude bij 
	m_procOldEdit = (WNDPROC) SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG) EditProcStatic);

	// Stel dit object in als userdata voor de statische wndproc functie van de edit box zodat deze members kan aanroepen
	SetWindowLong(m_hWndEdit, GWL_USERDATA, (LONG) this);

	this->SetFont(TEXT("Verdana"), false, false, false, 14);

	ListView_SetExtendedListViewStyleEx(m_hWndEdit, 0, LVS_EX_CHECKBOXES /*| LVS_EX_AUTOCHECKSELECT */| LVS_EX_FULLROWSELECT);
}
#pragma warning(default:4311)
#pragma warning(default:4312)

ListView::~ListView()
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

void ListView::AddCollumns()
{
	LVCOLUMN lvc;

	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_WIDTH | LVCF_MINWIDTH;

	lvc.iSubItem	= 0;
	lvc.pszText		= _T("Name");
	lvc.cx			= 300;
	lvc.cxMin		= 100;
	lvc.fmt			= LVCFMT_LEFT;

	ListView_InsertColumn(m_hWndEdit, 0, &lvc);

	lvc.iSubItem	= 1;
	lvc.pszText		= _T("size");
	lvc.cx			= 70;
	lvc.cxMin		= 20;
	lvc.fmt			= LVCFMT_CENTER;

	ListView_InsertColumn(m_hWndEdit, 1, &lvc);
}

void ListView::SetBounds(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;

	MoveWindow(m_hWndEdit, x, y, width, height, true);
}

RECT ListView::GetRect()
{
	RECT rc;

	GetClientRect(m_hWndEdit, &rc);

	rc.left += m_x;
	rc.right += m_x;
	rc.top += m_y; 
	rc.bottom += m_y;

	return rc;
}

void ListView::SetEnabled(bool bEnable)
{
	EnableWindow(m_hWndEdit, bEnable);
}

void ListView::Update()
{
	UpdateWindow(m_hWndEdit);
}

void ListView::Show()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);
}

void ListView::Hide()
{
	// Show and update the edit box
	ShowWindow(m_hWndEdit, SW_HIDE);
	UpdateWindow(m_hWndEdit);
}

void ListView::SetFont(tstring const& fontNameRef, bool bold, bool italic, bool underline, int size)
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

void ListView::SetForecolor( COLORREF color )
{
	m_ForeColor = color;
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

void ListView::SetBackcolor( COLORREF color )
{
	m_BgColor = color;
	
	if (m_BgColorBrush != 0) DeleteObject(m_BgColorBrush);
	m_BgColorBrush = CreateSolidBrush( color );
	
	// redraw the textbox
	InvalidateRect(m_hWndEdit, NULL, true);
}

COLORREF ListView::GetForecolor()
{
	return m_ForeColor;
}

COLORREF ListView::GetBackcolor()
{
	return m_BgColor;
}

HBRUSH ListView::GetBackcolorBrush()
{
	return m_BgColorBrush;
}

LRESULT ListView::EditProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<ListView*>(GetWindowLong(hWnd, GWL_USERDATA))->EditProc(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}

LRESULT ListView::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

void ListView::AddItem(tstring const& sName, tstring const& sSize)
{
	LVITEM lvI;

	lvI.mask		= LVIF_TEXT | LVIF_STATE;
	lvI.state		= 0;
	lvI.stateMask	= 0;
	lvI.iItem		= 0;
	lvI.iImage		= 0;
	lvI.iSubItem	= 0;
	lvI.lParam		= (LPARAM)sName.c_str();
	lvI.pszText		= (LPWSTR)sName.c_str();

	int index = SendMessage(m_hWndEdit,
                        LVM_INSERTITEM,
                        0,
                        (LPARAM) &lvI);

	lvI.mask		= LVIF_TEXT | LVIF_STATE;
	lvI.state		= 0;
	lvI.stateMask	= 0;
	lvI.iItem		= index;
	lvI.iImage		= 0;
	lvI.iSubItem	= 1;
	lvI.lParam		= (LPARAM) index;
	lvI.pszText		= (LPWSTR)sSize.c_str();

	SendMessage(m_hWndEdit,
                        LVM_SETITEM,
                        0,
                        (LPARAM) &lvI);

	SetCheckedState(index, true);
}

void ListView::RemoveItem(int index)
{
	SendMessage(m_hWndEdit, LVM_DELETEITEM, (LPARAM)index, 0);
}

int ListView::Count()
{
	return SendMessage(m_hWndEdit, LVM_GETITEMCOUNT, 0, 0);
}

int ListView::SelectedCount()
{
	return SendMessage(m_hWndEdit, LVM_GETSELECTEDCOUNT, 0, 0);
}

void ListView::DeleteAll()
{
	SendMessage(m_hWndEdit,
						LVM_DELETEALLITEMS,
                        0,
                        0);
}

int ListView::GetSelItem()
{
	return SendMessage(m_hWndEdit,
						LVM_GETNEXTITEM,
						(WPARAM)-1,
						(LPARAM)LVNI_SELECTED);
}
void ListView::SetSelItem(int index)
{
	/*SendMessage(m_hWndEdit,
						LVM_SETHOTITEM,
                        (LPARAM)index,
                        0);*/
}

void ListView::SetCheckedState(int index, bool checked)
{
	ListView_SetCheckState(m_hWndEdit, index, checked);
}
bool ListView::GetCheckedState(int index)
{
	UINT i = ListView_GetCheckState(m_hWndEdit, index);
	if ( i != 0)
		return true;
	else
		return false;
}

void ListView::SortOnIndex()
{
	SendMessage(m_hWndEdit,LVM_SORTITEMS, 0, this->Count()-1);
}


HWND ListView::GetWindow()
{
	return m_hWndEdit;
}

