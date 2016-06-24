#include "ListView.h"

//-----------------------------------------------------------------
// ListView methods
//-----------------------------------------------------------------
ListView::ListView() : Control ()
{
}

ListView::~ListView()
{
}
void ListView::Create(HWND hWnd)
{
	Control::Create(hWnd);

	AddCollumns();
	ListView_SetExtendedListViewStyleEx(GetWindow(), 0, LVS_EX_CHECKBOXES /*| LVS_EX_AUTOCHECKSELECT */ | LVS_EX_FULLROWSELECT);
}

HWND ListView::CreateHandle(HWND hWnd)
{
	return CreateWindow(WC_LISTVIEW, NULL, WS_BORDER | WS_CHILD | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL | LVS_REPORT, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
}

void ListView::AddCollumns()
{
	LVCOLUMN lvc;

	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_WIDTH | LVCF_MINWIDTH;

	lvc.iSubItem = 0;
	lvc.pszText = _T("Name");
	lvc.cx = 300;
	lvc.cxMin = 100;
	lvc.fmt = LVCFMT_LEFT;

	ListView_InsertColumn(GetWindow(), 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = _T("size");
	lvc.cx = 70;
	lvc.cxMin = 20;
	lvc.fmt = LVCFMT_CENTER;

	ListView_InsertColumn(GetWindow(), 1, &lvc);
}

LRESULT ListView::Procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC)wParam, RGB(255, 255, 255));
		SetTextColor((HDC)wParam, RGB(0, 0, 0));

		SelectObject((HDC)wParam, ENGINE->GetDefaultFont());

		return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));

	case WM_CHAR:
		if (wParam == VK_TAB) return 0;
		if (wParam == VK_RETURN) return 0;
		break;
	}

	return Control::Procedure(hWnd, msg, wParam, lParam);
}

void ListView::AddItem(tstring const& sName, tstring const& sSize)
{
	LVITEM lvI;

	lvI.mask = LVIF_TEXT | LVIF_STATE;
	lvI.state = 0;
	lvI.stateMask = 0;
	lvI.iItem = 0;
	lvI.iImage = 0;
	lvI.iSubItem = 0;
	lvI.lParam = (LPARAM)sName.c_str();
	lvI.pszText = (LPWSTR)sName.c_str();

	int index = SendMessage(GetWindow(),
		LVM_INSERTITEM,
		0,
		(LPARAM)&lvI);

	lvI.mask = LVIF_TEXT | LVIF_STATE;
	lvI.state = 0;
	lvI.stateMask = 0;
	lvI.iItem = index;
	lvI.iImage = 0;
	lvI.iSubItem = 1;
	lvI.lParam = (LPARAM)index;
	lvI.pszText = (LPWSTR)sSize.c_str();

	SendMessage(GetWindow(),
		LVM_SETITEM,
		0,
		(LPARAM)&lvI);

	SetCheckedState(index, true);
}

void ListView::RemoveItem(int index)
{
	SendMessage(GetWindow(), LVM_DELETEITEM, (LPARAM)index, 0);
}

int ListView::Count()
{
	return SendMessage(GetWindow(), LVM_GETITEMCOUNT, 0, 0);
}

int ListView::SelectedCount()
{
	return SendMessage(GetWindow(), LVM_GETSELECTEDCOUNT, 0, 0);
}

void ListView::DeleteAll()
{
	SendMessage(GetWindow(),
		LVM_DELETEALLITEMS,
		0,
		0);
}

int ListView::GetSelItem()
{
	return SendMessage(GetWindow(),
		LVM_GETNEXTITEM,
		(WPARAM)-1,
		(LPARAM)LVNI_SELECTED);
}
void ListView::SetSelItem(int index)
{
	/*SendMessage(GetWindow(),
	LVM_SETHOTITEM,
	(LPARAM)index,
	0);*/
}

void ListView::SetCheckedState(int index, bool checked)
{
	ListView_SetCheckState(GetWindow(), index, checked);
}
bool ListView::GetCheckedState(int index)
{
	UINT i = ListView_GetCheckState(GetWindow(), index);
	if (i != 0)
		return true;
	else
		return false;
}

void ListView::SortOnIndex()
{
	SendMessage(GetWindow(), LVM_SORTITEMS, 0, this->Count() - 1);
}


